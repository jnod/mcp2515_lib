#ifndef MCP2515_LIB_H_
#define MCP2515_LIB_H_

#include <stdint.h>

/*******************************************************************************
  Callback Function - implemented by the client. Must select MCP2515, transfer
  len bytes from *buf to MCP2515 and from MCP2515 to *buf simultaneously, and
  then deselect MCP2515.
*******************************************************************************/
void spi_transfer_mcp2515(uint8_t *buf, uint8_t len);

/*******************************************************************************
  CAN Message Data Type
*******************************************************************************/
#define TYPE_STANDARD_DATA   0
#define TYPE_EXTENDED_DATA   1
#define TYPE_STANDARD_REMOTE 2
#define TYPE_EXTENDED_REMOTE 3

typedef struct {
  uint8_t   type;
  uint16_t  standard_id; // Only lower 11 bits are used
  uint32_t  extended_id; // Only lower 18 bits are used
  uint8_t   length;
  uint8_t   data[8];
} can_message_t;

/*******************************************************************************
  Library API - detailed descriptions can be found in the mcp2515.c source file.
*******************************************************************************/
void clear_interrupt_flags(uint8_t bit_mask);
void read_interrupt_flags(uint8_t *flags);
void read_receive_buffer_0(can_message_t *message);
void read_receive_buffer_1(can_message_t *message);

/*******************************************************************************
  SPI Commands
*******************************************************************************/
#define SPI_READ        0x03
#define SPI_BIT_MODIFY  0x05

/*******************************************************************************
  Register Addresses - specific info about each register can be found in the
  datasheet.
*******************************************************************************/
#define CANINTF   0x2C

/*******************************************************************************
  Interrupt Flag Bit Masks - each bit mask aligns with a position in the CANINTF
  register. Specific info about each flag can be found in the datasheet.
*******************************************************************************/
#define MERRF     0x80
#define WAKIF     0x40
#define ERRIF     0x20
#define TX2IF     0x10
#define TX1IF     0x08
#define TX0IF     0x04
#define RX1IF     0x02
#define RX0IF     0x01

/*******************************************************************************
  Flag Test Macro - determines whether the specified flag is set.
    - flags:    a value read from the CANINTF register
    - bit_mask: one of the Interrupt Flag Bit Masks
*******************************************************************************/
#define IS_FLAG_SET(flags, bit_mask) (flags & bit_mask)

#endif
