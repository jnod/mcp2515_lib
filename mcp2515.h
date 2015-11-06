#ifndef MCP2515_LIB_H_
#define MCP2515_LIB_H_

#include <stdint.h>

// Callback methods that must be supplied by the client
typedef void (*spi_transfer_t)(uint8_t* buf, uint8_t len);
typedef void (*spi_select_mcp2515_t)();
typedef void (*spi_deselect_mcp2515_t)();

// Datatypes
#define type_standard_data   0
#define type_extended_data   1
#define type_standard_remote 2
#define type_extended_remote 3

typedef struct {
  uint8_t type;
  uint16_t standard_id; // Only lower 11 bits are used
  uint32_t extended_id; // Only lower 18 bits are used
  uint8_t length;
  uint8_t data[8];
} message_t;

typedef struct {
  uint8_t message_error;
  uint8_t wakeup;
  uint8_t error;
  uint8_t transmit_2_empty;
  uint8_t transmit_1_empty;
  uint8_t transmit_0_empty;
  uint8_t receive_1_full;
  uint8_t receive_0_full;
} interrupts_t;

// Library API functions
void init_spi(spi_transfer_t st, spi_select_mcp2515_t ss, spi_deselect_mcp2515_t sd);
void check_interrupts(interrupts_t *interrupts);
void read_receive_buffer_0(message_t *message);
void read_receive_buffer_1(message_t *message);

#endif
