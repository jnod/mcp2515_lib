// #include <bcm2835.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "../../mcp2515.h"

int messageFromStr();
void printJsonMessage();
void *relayInput(void*);

#define STR_SIZE 100

uint8_t run = 1;
CanMessage message;
pthread_t read_thread;
char str[STR_SIZE];
sem_t sem;
uint8_t has_input = 0;

int main() {
  // bcm2835_set_debug(1); // Test without using GPIO
  // if (!bcm2835_init()) return 1;
  //
  // bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_spi_begin();
  // bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing

  if (pthread_create(&read_thread, NULL, relayInput, NULL)) return 1;
  if (sem_init(&sem, 0, 1)) return 1;

  while(run) {
    // if (!digitalRead(9)) {
    //   Serial.println("Interrupt");
    // }

    if (has_input) {
      has_input = 0;
      if (messageFromStr() == 0) {
        printJsonMessage();
      }
      sem_post(&sem);
    }
  }

  // bcm2835_spi_end();
  // bcm2835_close();

  return 0;
}

void mcp2515_spiTransfer(uint8_t *buf, uint8_t len) {
  // bcm2835_spi_transfern(buf, len); // will select mcp2515 during transfer
}

int messageFromStr() {
  if (str[0] == 0 || str[0] == '\n') return 1;

  uint32_t buffer[12] = {0};
  uint16_t i = 0;
  uint8_t buf_i = 0;
  uint8_t char_val = 0;
  uint8_t flag = 0; // prevents incrementing buf_i multiple times for extra spaces

  while (str[i] != 0 && i < STR_SIZE) {
    char_val = str[i] - '0'; // for chars < '0', the result will be a large num

    if (char_val < 10) {
      if (buf_i >= 12) return 1;
      flag = 1;
      buffer[buf_i] = buffer[buf_i] * 10 + char_val;
    } else if (flag == 1){
      buf_i++;
      flag = 0;
    }

    i++;
  }

  message.mtype = buffer[0];
  message.std_id = buffer[1];
  message.ext_id = buffer[2];
  message.length = buffer[3];

  if (message.length > 8) return 1;

  for (i = 0; i < message.length; i++) {
    message.data[i] = buffer[i+4];
  }

  return 0;
}

void printJsonMessage() {
  printf("{\"mtype\":%u,\"std_id\":%u,\"ext_id\":%u,\"length\":%u,\"data\":[",
          message.mtype, message.std_id, message.ext_id, message.length);

  uint8_t i = 0;
  while (i < message.length) {
    printf("%u", message.data[i]);
    i++;

    if (i < message.length) {
      printf(",");
    }
  }

  printf("]}\n");
}

void *relayInput(void* v) {
  while(run) {
    sem_wait(&sem);
    fgets(str, 100, stdin);
    has_input = 1;
  }

  return NULL;
}
