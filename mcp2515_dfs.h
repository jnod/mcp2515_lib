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

#ifndef MCP2515_LIB_DFS_H_
#define MCP2515_LIB_DFS_H_

// Message types
#define MTYPE_STANDARD_DATA   0
#define MTYPE_STANDARD_REMOTE 1
#define MTYPE_EXTENDED_DATA   2
#define MTYPE_EXTENDED_REMOTE 3

// Macros that determine if a CANINTF flag is set
#define IS_MERRF_SET(canintf) (canintf & 0x80)
#define IS_WAKIF_SET(canintf) (canintf & 0x40)
#define IS_ERRIF_SET(canintf) (canintf & 0x20)
#define IS_TX2IF_SET(canintf) (canintf & 0x10)
#define IS_TX1IF_SET(canintf) (canintf & 0x08)
#define IS_TX0IF_SET(canintf) (canintf & 0x04)
#define IS_RX1IF_SET(canintf) (canintf & 0x02)
#define IS_RX0IF_SET(canintf) (canintf & 0x01)

// Modes of operation
#define MODE_NORMAL           0x00
#define MODE_SLEEP            0x20
#define MODE_LOOPBACK         0x40
#define MODE_LISTEN_ONLY      0x60
#define MODE_CONFIGURATION    0x80

// Clock timing configurations. These are just examples, many are possible.
// Refer to: http://www.kvaser.com/support/calculators/bit-timing-calculator/
#define CNF1_16MHZ_125KBIT    0x43
#define CNF2_16MHZ_125KBIT    0xA3
#define CNF3_16MHZ_125KBIT    0x05

#define CNF1_16MHZ_250KBIT    0x41
#define CNF2_16MHZ_250KBIT    0xA3
#define CNF3_16MHZ_250KBIT    0x05

#define CNF1_16MHZ_500KBIT    0x40
#define CNF2_16MHZ_500KBIT    0xA3
#define CNF3_16MHZ_500KBIT    0x05

#define CNF1_16MHZ_1000KBIT   0x40
#define CNF2_16MHZ_1000KBIT   0x89
#define CNF3_16MHZ_1000KBIT   0x02

#define CNF1_10MHZ_125KBIT    0x41
#define CNF2_10MHZ_125KBIT    0xAD
#define CNF3_10MHZ_125KBIT    0x06

#define CNF1_10MHZ_250KBIT    0x40
#define CNF2_10MHZ_250KBIT    0xAD
#define CNF3_10MHZ_250KBIT    0x06

#define CNF1_10MHZ_500KBIT    0x40
#define CNF2_10MHZ_500KBIT    0x91
#define CNF3_10MHZ_500KBIT    0x03

#define CNF1_10MHZ_1000KBIT   0x40
#define CNF2_10MHZ_1000KBIT   0x80
#define CNF3_10MHZ_1000KBIT   0x01

// mcp2515 register addresses
#define ADDR_CANINTE          0x2B // Interrupt Enable bits
#define ADDR_CANINTF          0x2C // Interrupt Flags
#define ADDR_CANCTRL          0x0F // Can Control
#define ADDR_CNF1             0x2A // Configuration
#define ADDR_CNF2             0x29
#define ADDR_CNF3             0x28
#define ADDR_EFLG             0x2D
#define ADDR_RXB0CTRL         0x60
#define ADDR_RXB1CTRL         0x70
#define ADDR_RXF0             0x00 // Filters for RXB0
#define ADDR_RXF1             0x04
#define ADDR_RXF2             0x08 // Filters for RXB1
#define ADDR_RXF3             0x10
#define ADDR_RXF4             0x14
#define ADDR_RXF5             0x18
#define ADDR_RXM0             0x20 // Mask for RXB0
#define ADDR_RXM1             0x24 // Mask for RXB1

// mcp2515 spi commands
#define SPI_BIT_MODIFY        0x05
#define SPI_LOAD_TX0          0x40
#define SPI_LOAD_TX1          0x42
#define SPI_LOAD_TX2          0x44
#define SPI_READ              0x03
#define SPI_READ_RX0          0x90
#define SPI_READ_RX1          0x92
#define SPI_RTS_TX0           0x81
#define SPI_RTS_TX1           0x82
#define SPI_RTS_TX2           0x84
#define SPI_WRITE             0x02

#endif
