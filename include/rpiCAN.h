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

#ifndef RPICAN_H_
#define RPICAN_H_

#include <bcm2835.h>
#include <stdint.h>
#include "mcp2515.h"

#define RPICAN_BAUD_125MHZ   125
#define RPICAN_BAUD_250MHZ   250
#define RPICAN_BAUD_500MHZ   500
#define RPICAN_BAUD_1000MHZ  1000

// GPIO pin mappings to the bcm2835 for rpi b+ and rpi 2 40 pin headers
#define RPICAN_GPIO_2        RPI_BPLUS_GPIO_J8_03
#define RPICAN_GPIO_3        RPI_BPLUS_GPIO_J8_05
#define RPICAN_GPIO_4        RPI_BPLUS_GPIO_J8_07
#define RPICAN_GPIO_5        RPI_BPLUS_GPIO_J8_29
#define RPICAN_GPIO_6        RPI_BPLUS_GPIO_J8_31
#define RPICAN_GPIO_7        RPI_BPLUS_GPIO_J8_26
#define RPICAN_GPIO_8        RPI_BPLUS_GPIO_J8_24
#define RPICAN_GPIO_9        RPI_BPLUS_GPIO_J8_21
#define RPICAN_GPIO_10       RPI_BPLUS_GPIO_J8_19
#define RPICAN_GPIO_11       RPI_BPLUS_GPIO_J8_23
#define RPICAN_GPIO_12       RPI_BPLUS_GPIO_J8_32
#define RPICAN_GPIO_13       RPI_BPLUS_GPIO_J8_33
#define RPICAN_GPIO_14       RPI_BPLUS_GPIO_J8_08
#define RPICAN_GPIO_15       RPI_BPLUS_GPIO_J8_10
#define RPICAN_GPIO_16       RPI_BPLUS_GPIO_J8_36
#define RPICAN_GPIO_17       RPI_BPLUS_GPIO_J8_11
#define RPICAN_GPIO_18       RPI_BPLUS_GPIO_J8_12
#define RPICAN_GPIO_19       RPI_BPLUS_GPIO_J8_35
#define RPICAN_GPIO_20       RPI_BPLUS_GPIO_J8_38
#define RPICAN_GPIO_21       RPI_BPLUS_GPIO_J8_40
#define RPICAN_GPIO_22       RPI_BPLUS_GPIO_J8_15
#define RPICAN_GPIO_23       RPI_BPLUS_GPIO_J8_16
#define RPICAN_GPIO_24       RPI_BPLUS_GPIO_J8_18
#define RPICAN_GPIO_25       RPI_BPLUS_GPIO_J8_22
#define RPICAN_GPIO_26       RPI_BPLUS_GPIO_J8_37
#define RPICAN_GPIO_27       RPI_BPLUS_GPIO_J8_13


void rpiCAN_close();
void rpiCAN_config();
void rpiCAN_init(uint8_t bcm2835_interruptPin);
void rpiCAN_read(CanMessage* canMessage);
void rpiCAN_setBaud(uint16_t baudRate);
void rpiCAN_start();
void rpiCAN_write(CanMessage* canMessage);


#endif
