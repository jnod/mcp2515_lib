#include <bcm2835.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "../../mcp2515.h"

/* The following are use for command line input *******************************/
#define STR_SIZE  100

void *relayInput(void*);

uint8_t run = 1;
pthread_t read_thread;
char str[STR_SIZE];
sem_t sem;
uint8_t has_commandline_input = 0;
/******************************************************************************/

#define INT       RPI_V2_GPIO_P1_22 // pin 22 (maps to RBPI GPIO 25)

int messageFromStr();
void printJsonCanMessage();

static CanMessage message;

void setup() {
  // bcm2835_set_debug(1); // Test without using GPIO
  if (!bcm2835_init()) return;

  bcm2835_gpio_fsel(INT, BCM2835_GPIO_FSEL_INPT); // set INT as input
  bcm2835_spi_begin();
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

  delay(100); // Allows the mcp2515 to initialize

  mcp2515_reset();
  mcp2515_configCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing
}

void loop() {
  if (bcm2835_gpio_lev(INT) == LOW) {
    printf("Interrupt\n");
    mcp2515_readRX0(&message);
    printJsonCanMessage();
    mcp2515_clearCANINTF(0xFF);
  }

  if (has_commandline_input) {
    has_commandline_input = 0;
    if (messageFromStr() == 0) {
      printJsonCanMessage();

      mcp2515_loadTX0(&message);
      mcp2515_rtsTX0();
    }
    sem_post(&sem);
  }
}

int main() {
  setup();

  if (pthread_create(&read_thread, NULL, relayInput, NULL)) return 1;
  if (sem_init(&sem, 0, 1)) return 1;

  while(run) {
    loop();
  }

  bcm2835_spi_end();
  bcm2835_close();

  return 0;
}

void mcp2515_spiTransfer(uint8_t *buf, uint8_t len) {
  bcm2835_spi_transfern(buf, len); // will select mcp2515 during transfer
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

  if (buf_i == 0) return 1;

  message.mtype = buffer[0];
  message.sid = buffer[1];
  message.eid = buffer[2];
  message.length = buffer[3];

  if (message.length > 8) return 1;

  for (i = 0; i < message.length; i++) {
    message.data[i] = buffer[i+4];
  }

  return 0;
}

void printJsonCanMessage() {
  printf("{\"mtype\":%u,\"sid\":%u,\"eid\":%u,\"length\":%u,\"data\":[",
          message.mtype, message.sid, message.eid, message.length);

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
    has_commandline_input = 1;
  }

  return NULL;
}
