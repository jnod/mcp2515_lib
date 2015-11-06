#include "mcp2515.h"

static uint8_t buffer[15];

/*******************************************************************************
  Clears interrupt register (CANINTF) flags that correlate with a '1' in the
  bit_mask parameter.
*******************************************************************************/
void clear_interrupt_flags(uint8_t bit_mask) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = CANINTF;
  buffer[2] = bit_mask;
  buffer[3] = 0x00;

  spi_transfer_mcp2515(buffer, 4);
}

/*******************************************************************************
  Reads the interrupt register (CANINTF) value into the *flags parameter. Each
  bit represents a different flag. The individual flags are defined in the
  mcp2515_dfs.h header.
*******************************************************************************/
void read_interrupt_flags(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = CANINTF;

  spi_transfer_mcp2515(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads and decodes the value of receive buffer 0 (RXB0) and places the result
  into the *message parameter.
*******************************************************************************/
void read_receive_buffer_0(can_message_t *message) {

}

/*******************************************************************************
  Reads and decodes the value of receive buffer 1 (RXB1) and places the result
  into the *message parameter.
*******************************************************************************/
void read_receive_buffer_1(can_message_t *message) {

}
