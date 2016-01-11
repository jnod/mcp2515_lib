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

#ifndef RBPI_CAN_H_
#define RBPI_CAN_H_

#include <stdint.h>
#include "../hal/mcp2515.h"

#define BAUD_125MHZ   125
#define BAUD_250MHZ   250
#define BAUD_500MHZ   500
#define BAUD_1000MHZ  1000


void rbpiCAN_config();
void rbpiCAN_init();
void rbpiCAN_read(CanMessage* canMessage);
void rbpiCAN_setBaud(uint16_t baudRate);
void rbpiCAN_start();
void rbpiCAN_write(CanMessage* canMessage);


#endif
