#ifndef MCP2515_LIB_H_
#define MCP2515_LIB_H_

/*******************************************************************************
  Fixed Size Data Type Definitions
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
  MCP2515 Technical Definitions
*******************************************************************************/
#include "mcp2515_dfs.h"

/*******************************************************************************
  Callback Function - must be implemented by the client
*******************************************************************************/
void spi_transfer_mcp2515(uint8_t* buf, uint8_t len);

/*******************************************************************************
  CAN Message Data Type
*******************************************************************************/
#define type_standard_data   0
#define type_extended_data   1
#define type_standard_remote 2
#define type_extended_remote 3

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
void read_interrupt_flags(uint8_t *flags);
void read_receive_buffer_0(can_message_t *message);
void read_receive_buffer_1(can_message_t *message);

#endif
