//#include "bcm2835.h"
#include "mcp2515.h"
#include <pthread.h>
#include "rpiCAN.h"
#include <semaphore.h>
#include <stdio.h>

#define STR_SIZE  100

int main();
static int commandMessageFromStr();
static void printJsonCanMessage(CanMessage*);
static void* messageReader(void*);

static CanMessage commandMessage;
static char str[STR_SIZE];
static pthread_t readThread;
static uint8_t run = 1;

int main() {
  pthread_create(&readThread, NULL, messageReader, NULL);

  rpiCAN_init(RPICAN_GPIO_25);
  rpiCAN_setBaud(RPICAN_BAUD_125MHZ);
  rpiCAN_start();

  while(run) {
    fgets(str, STR_SIZE, stdin);
    if (commandMessageFromStr() == 0) {
      printJsonCanMessage(&commandMessage);
      rpiCAN_write(&commandMessage);
    }
  }

  return 0;
}

static int commandMessageFromStr() {
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

  commandMessage.mtype = buffer[0];
  commandMessage.sid = buffer[1];
  commandMessage.eid = buffer[2];
  commandMessage.length = buffer[3];

  if (commandMessage.length > 8) return 1;

  for (i = 0; i < commandMessage.length; i++) {
    commandMessage.data[i] = buffer[i+4];
  }

  return 0;
}

static void printJsonCanMessage(CanMessage* message) {
  printf("{\"mtype\":%u,\"sid\":%u,\"eid\":%u,\"length\":%u,\"data\":[",
          message->mtype, message->sid, message->eid, message->length);

  uint8_t i = 0;
  while (i < message->length) {
    printf("%u", message->data[i]);
    i++;

    if (i < message->length) {
      printf(",");
    }
  }

  printf("]}\n");
}

static void* messageReader(void* arg) {
  CanMessage message;

  while(run) {
    rpiCAN_read(&message);
    printJsonCanMessage(&message);
  }

  pthread_exit(0);
}
