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

#include "rbpiCAN.h"
#include <semaphore.h>
#include <pthread.h>

typedef struct {
  sem_t dataSem;
  CanMessage messages[256];
  uint8_t head, tail;
} CanMessageBuffer;

CanMessageBuffer rxBuffer;
CanMessageBuffer txBuffer;

pthread_t readThread;
pthread_t writeThread;

sem_t spiAccessSem;

uint8_t run = 0;

static void* read(void* arg) {
  while (run) {

  }

  pthread_exit(0);
}

static void* write(void* arg) {
  while (run) {

  }

  pthread_exit(0);
}

void mcp2515_spiTransfer(uint8_t* buf, uint8_t len) {
  sem_wait(&spiAccessSem);

  sem_post(&spiAccessSem);
}

void rbpiCAN_config() {
  mcp2515_setMode(MODE_CONFIGURATION);
}

void rbpiCAN_exit() {
  if (run) {
    run = 0;

    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);

    sem_destroy(&rxBuffer.dataSem);
    sem_destroy(&txBuffer.dataSem);
    sem_destroy(&spiAccessSem);
  }
}

void rbpiCAN_init() {
  if (run == 0) {
    run = 1;

    rxBuffer.head = 0; rxBuffer.tail = 0;
    txBuffer.head = 0; txBuffer.tail = 0;

    sem_init(&rxBuffer.dataSem, 0, 0);
    sem_init(&txBuffer.dataSem, 0, 0);
    sem_init(&spiAccessSem, 0, 0);

    pthread_create(&readThread, NULL, &read, NULL);
    pthread_create(&writeThread, NULL, &write, NULL);
  }
}

void rbpiCAN_read(CanMessage* canMessage) {
  sem_wait(&rxBuffer.dataSem);
  *canMessage = rxBuffer.messages[rxBuffer.head];
  rxBuffer.head++;
}

void rbpiCAN_setBaud(uint16_t baudRate) {
  switch (baudRate) {
    case BAUD_125MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
      break;
    case BAUD_250MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_250KBIT, CNF2_10MHZ_250KBIT, CNF3_10MHZ_250KBIT);
      break;
    case BAUD_500MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_500KBIT, CNF2_10MHZ_500KBIT, CNF3_10MHZ_500KBIT);
      break;
    case BAUD_1000MHZ:
      mcp2515_configCNFn(CNF1_10MHZ_1000KBIT, CNF2_10MHZ_1000KBIT, CNF3_10MHZ_1000KBIT);
      break;
  }
}

void rbpiCAN_start() {
  mcp2515_setMode(MODE_NORMAL);
}

void rbpiCAN_write(CanMessage* canMessage) {
  txBuffer.messages[txBuffer.tail] = *canMessage;
  txBuffer.tail++;
  sem_post(&txBuffer.dataSem);
}
