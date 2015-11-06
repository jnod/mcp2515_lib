#include "mcp2515.h"

static spi_transfer_t transfer;
static spi_select_mcp2515_t select;
static spi_deselect_mcp2515_t deselect;
static uint8_t buffer[10] = {0};

void init_spi(spi_transfer_t st, spi_select_mcp2515_t ss, spi_deselect_mcp2515_t sd) {
  transfer = st;
  select = ss;
  deselect = sd;

  select();
  transfer(buffer, 2);
  deselect();
}

void check_interrupts(interrupts_t *interrupts) {

}

void read_receive_buffer_0(message_t *message) {
  
}

void read_receive_buffer_1(message_t *message) {

}
