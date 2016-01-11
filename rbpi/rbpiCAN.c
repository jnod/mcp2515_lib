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
  sem_t data;
  CanMessage buffer[256];
  uint8_t head, tail;
} CanMessageBuffer;

CanMessageBuffer rxBuffer = {.head = 0, .tail = 0};
CanMessageBuffer txBuffer = {.head = 0, .tail = 0};

pthread_t thread;

uint8_t run = 1;

static void* communicate(void* arg) {
  while (run) {

  }

  sem_destroy(&rxBuffer.data);
  sem_destroy(&txBuffer.data);

  return arg;
}

void rbpiCAN_baud(uint16_t baudRate) {
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

void rbpiCAN_config() {
  mcp2515_setMode(MODE_CONFIGURATION);
}

void rbpiCAN_exit() {
  run = 0;
}

void rbpiCAN_init() {
  sem_init(&rxBuffer.data, 0, 0);
  sem_init(&txBuffer.data, 0, 0);

  pthread_create(&thread, NULL, &communicate, NULL);
}

void rbpiCAN_read(CanMessage* canMessage) {
  
}

void rbpiCAN_start() {
  mcp2515_setMode(MODE_NORMAL);
}

void rbpiCAN_write(CanMessage* canMessage) {

}
