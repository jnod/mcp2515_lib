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
  Sets the configuration registers (CNF1, CNF2, and CNF3) to the provided
  values. These values determine the baud rate for different oscillators. A few
  quick configurations are provided in the mcp2515.h header. More info can be
  found in the datasheet.
*******************************************************************************/
void config_timing(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3) {
  buffer[0] = SPI_WRITE;
  buffer[1] = CNF1;
  buffer[2] = cnf1;
  buffer[3] = cnf2;
  buffer[4] = cnf3;

  spi_transfer_mcp2515(buffer, 5);
}

/*******************************************************************************
  Reads the interrupt register (CANINTF) value into the *flags parameter. Each
  bit represents a different flag. The individual flags are defined in the
  mcp2515.h header.
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
void read_receive_buffer_n(uint8_t n, can_message_t *message) {

}

/*******************************************************************************
  Sets the filter associated with the provided filter address. Each filter
  actually consists of 4 registers, but this function simplifies the process.
  Each filter address is defined in the mcp2515.h header.
*******************************************************************************/
void set_filter(uint8_t filter_addr, uint16_t std_id, uint32_t ext_id) {

}
