#ifndef MCP2515_LIB_DEFINITIONS_H_
#define MCP2515_LIB_DEFINITIONS_H_

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
