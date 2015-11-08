#ifndef MCP2515_LIB_H_
#define MCP2515_LIB_H_

#include <stdint.h>

/*******************************************************************************
  Callback Function

  Implemented by the client. Must select MCP2515, transfer len bytes from *buf
  to MCP2515 and from MCP2515 to *buf simultaneously, and then deselect MCP2515.
*******************************************************************************/
void spi_transfer_mcp2515(uint8_t *buf, uint8_t len);

/*******************************************************************************
  CAN Message Data Type

*******************************************************************************/
#define MTYPE_STANDARD_DATA   0x00
#define MTYPE_STANDARD_REMOTE 0x01
#define MTYPE_EXTENDED_DATA   0x10
#define MTYPE_EXTENDED_REMOTE 0x11

typedef struct {
  uint8_t   mtype;
  uint16_t  std_id; // Only least significant 11 bits are used
  uint32_t  ext_id; // Only least significant 18 bits are used
  uint8_t   length;
  uint8_t   data[8];
} can_message_t;

/*******************************************************************************
  Flag Test Macro

  Determines whether the specified flag is set.
    - flags:    a value read from the CANINTF register
    - bit_mask: one of the Interrupt Flag Bit Masks (see below)
*******************************************************************************/
#define IS_FLAG_SET(flags, bit_mask) (flags & bit_mask)

/*******************************************************************************
  Interrupt Flag Bit Masks

  Each bit mask aligns with a position in the CANINTF register. Specific info
  about each flag can be found in the datasheet.
*******************************************************************************/
#define MASK_MERRF  0x80
#define MASK_WAKIF  0x40
#define MASK_ERRIF  0x20
#define MASK_TX2IF  0x10
#define MASK_TX1IF  0x08
#define MASK_TX0IF  0x04
#define MASK_RX1IF  0x02
#define MASK_RX0IF  0x01

/*******************************************************************************
  Library API

  Detailed descriptions can be found in the mcp2515.c source file. Many of the
  parameters have been listed as definitions in this header file.
*******************************************************************************/
void clear_error_flags(uint8_t bit_mask);
void clear_interrupt_flags(uint8_t bit_mask);
void config_interrupts(uint8_t caninte_value);
void config_timing(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3);
void read_error_flags(uint8_t *flags);
void read_interrupt_flags(uint8_t *flags);
void read_receive_buffer_n(uint8_t n, can_message_t *message);
void set_ext_filter(uint8_t addr, uint32_t ext_id);
void set_std_filter(uint8_t addr, uint16_t std_id);
void set_mask(uint8_t addr, uint16_t std_id, uint32_t ext_id);
void set_mode(uint8_t mode);

/*******************************************************************************
  Modes of Operation

*******************************************************************************/
#define MODE_NORMAL         0x00
#define MODE_SLEEP          0x20
#define MODE_LOOPBACK       0x40
#define MODE_LISTEN_ONLY    0x60
#define MODE_CONFIGURATION  0x80

/*******************************************************************************
  Register Addresses

  Specific info about each register can be found in the datasheet.
*******************************************************************************/
#define ADDR_CANINTE  0x2B // Interrupt Enable bits
#define ADDR_CANINTF  0x2C // Interrupt Flags

#define ADDR_CANCTRL  0x0F // Can Control

#define ADDR_CNF1     0x2A // Configuration
#define ADDR_CNF2     0x29
#define ADDR_CNF3     0x28

#define ADDR_EFLG     0x2D

#define ADDR_RXF0     0x00 // Filters for RXB0
#define ADDR_RXF1     0x04

#define ADDR_RXF2     0x08 // Filters for RXB1
#define ADDR_RXF3     0x10
#define ADDR_RXF4     0x14
#define ADDR_RXF5     0x18

#define ADDR_RXM0     0x20 // Mask for RXB0

#define ADDR_RXM1     0x24 // Mask for RXB1

/*******************************************************************************
  Quick Configurations

  CNF1, CNF2, and CNF3 can be configured in many ways to accomodate for
  different clock and baud rates. The following are just a few examples. A CAN
  timing calculator can be found at:
      http://www.kvaser.com/support/calculators/bit-timing-calculator/
*******************************************************************************/
#define CNF1_16MHZ_125KBIT    0x43 // 16 MHz clock
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

#define CNF1_10MHZ_125KBIT    0x41 // 10 MHz clock
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

/*******************************************************************************
  SPI Commands

*******************************************************************************/
#define SPI_BIT_MODIFY  0x05
#define SPI_READ        0x03
#define SPI_READ_RX0    0x90
#define SPI_READ_RX1    0x92
#define SPI_WRITE       0x02

#endif
