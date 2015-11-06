#include "mcp2515.h"

/*******************************************************************************
  Callback Function Pointers
*******************************************************************************/
static spi_deselect_t deselect;
static spi_select_t select;
static spi_transfer_t transfer;

/*******************************************************************************
  SPI Transfer Buffer
*******************************************************************************/
static uint8_t buffer[15] = {0};

/*******************************************************************************
  Initializes the SPI interface callback functions. This allows clients to
  define a custom SPI interface for the platform and layout being used.
*******************************************************************************/
void init_spi(spi_deselect_t desel, spi_select_t sel, spi_transfer_t transf) {
  transfer = transf;
  select = sel;
  deselect = desel;
}

/*******************************************************************************
  Reads the interrupt register (CANINTF) value into the flags parameter.
*******************************************************************************/
void read_interrupt_flags(uint8_t *flags) {
  select();

  buffer[0] = SPI_READ;
  buffer[1] = CANINTF;

  transfer(buffer, 3);
  deselect();

  *flags = buffer[2];
}

/*******************************************************************************
  Clears interrupt register (CANINTF) flags that correlate with a '1' in the
  bit_mask parameter.
*******************************************************************************/
void clear_flags(uint8_t bit_mask) {

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
