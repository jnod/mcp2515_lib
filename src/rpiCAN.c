/*
  Copyright 2015 Jordan Nodland

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <pthread.h>
#include "rpiCAN.h"
#include <semaphore.h>
#include <unistd.h>

typedef struct {
  sem_t dataSem;
  CanMessage messages[256];
  uint8_t head, tail;
} CanMessageBuffer;

static void pushRx(CanMessage*);
static void pushTx(CanMessage*);
static void popRx(CanMessage*);
static void popTx(CanMessage*);
static void* reader(void*);
static void* writer(void*);

static CanMessageBuffer rxBuffer;
static CanMessageBuffer txBuffer;
static pthread_t readThread;
static pthread_t writeThread;
static sem_t spiAccessSem;
static uint8_t run = 0;
static uint8_t intPin = 0;

void mcp2515_spiTransfer(uint8_t* buf, uint8_t len) {
  sem_wait(&spiAccessSem);

  bcm2835_spi_transfern((char*)buf, len);

  sem_post(&spiAccessSem);
}

void rpiCAN_close() {
  if (run) {
    run = 0;

    // Sends a message to the write thread so that it becomes unblocked. This
    // message will never be sent over the CAN, though, because run == 0.
    CanMessage message;
    pushTx(&message);

    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);

    sem_destroy(&rxBuffer.dataSem);
    sem_destroy(&txBuffer.dataSem);
    sem_destroy(&spiAccessSem);

    bcm2835_spi_end();
    bcm2835_close();
  }
}

void rpiCAN_config() {
  mcp2515_setMode(MODE_CONFIGURATION);
}

void rpiCAN_init(uint8_t bcm2835_interruptPin) {
  if (run == 0) {
    run = 1;
    intPin = bcm2835_interruptPin;

    if (bcm2835_init() == 0) return;

    bcm2835_gpio_fsel(intPin, BCM2835_GPIO_FSEL_INPT); // set INT as input
    bcm2835_spi_begin();
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    rxBuffer.head = 0; rxBuffer.tail = 0;
    txBuffer.head = 0; txBuffer.tail = 0;

    sem_init(&rxBuffer.dataSem, 0, 0);
    sem_init(&txBuffer.dataSem, 0, 0);
    sem_init(&spiAccessSem, 0, 0);

    pthread_create(&readThread, NULL, &reader, NULL);
    pthread_create(&writeThread, NULL, &writer, NULL);

    mcp2515_reset();
    mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
    mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  }
}

void rpiCAN_read(CanMessage* canMessage) {
  popRx(canMessage);
}

void rpiCAN_setBaud(uint16_t baudRate) {
  switch (baudRate) {
    case RPICAN_BAUD_125MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
      break;
    case RPICAN_BAUD_250MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_250KBIT, CNF2_10MHZ_250KBIT, CNF3_10MHZ_250KBIT);
      break;
    case RPICAN_BAUD_500MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_500KBIT, CNF2_10MHZ_500KBIT, CNF3_10MHZ_500KBIT);
      break;
    case RPICAN_BAUD_1000MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_1000KBIT, CNF2_10MHZ_1000KBIT, CNF3_10MHZ_1000KBIT);
      break;
  }
}

void rpiCAN_start() {
  mcp2515_setMode(MODE_NORMAL);
}

void rpiCAN_write(CanMessage* canMessage) {
  pushTx(canMessage);
}

static void pushRx(CanMessage* message) {
  rxBuffer.messages[rxBuffer.tail] = *message;
  rxBuffer.tail++;
  sem_post(&rxBuffer.dataSem);
}

static void pushTx(CanMessage* message) {
  txBuffer.messages[txBuffer.tail] = *message;
  txBuffer.tail++;
  sem_post(&txBuffer.dataSem);
}

static void popRx(CanMessage* message) {
  sem_wait(&rxBuffer.dataSem);
  *message = rxBuffer.messages[rxBuffer.head];
  rxBuffer.head++;
}

static void popTx(CanMessage* message) {
  sem_wait(&txBuffer.dataSem);
  *message = txBuffer.messages[txBuffer.head];
  txBuffer.head++;
}

static void* reader(void* arg) {
  uint8_t status;
  CanMessage message;

  while (run) {
    if (bcm2835_gpio_lev(intPin) == LOW) {
      mcp2515_readStatus(&status);

      if (status & 0x01) {
        mcp2515_readRX0(&message);
        pushRx(&message);
      }

      if (status & 0x02) {
        mcp2515_readRX1(&message);
        pushRx(&message);
      }

      mcp2515_clearCANINTF(0xFF);
    }
  }

  pthread_exit(0);
}

static void* writer(void* arg) {
  uint8_t status;
  CanMessage message;

  while (run) {
    popTx(&message);

    // Continually reads the status until there is an available tx register.
    // Will abort if run == 0.
    while (run) {
      mcp2515_readStatus(&status);

      if ((status & 0x04) == 0) {
        mcp2515_loadTX0(&message);
        mcp2515_rtsTX0();
        break;
      } else if ((status & 0x10) == 0) {
        mcp2515_loadTX1(&message);
        mcp2515_rtsTX1();
        break;
      } else if ((status & 0x40) == 0) {
        mcp2515_loadTX2(&message);
        mcp2515_rtsTX2();
        break;
      }

      // Wait a little while before trying again
      usleep(10);
    }
  }

  pthread_exit(0);
}
