#include "mcp2515.h"
#include "mcp2515_dfs.h"

static spi_transfer_t transfer;
static spi_select_mcp2515_t select;
static spi_deselect_mcp2515_t deselect;
static uint8_t buffer[15] = {0};

void init_spi(spi_transfer_t st, spi_select_mcp2515_t ss, spi_deselect_mcp2515_t sd) {
  transfer = st;
  select = ss;
  deselect = sd;

  select();
  transfer(buffer, 2);
  deselect();
}

void check_interrupts(interrupts_t *interrupts) {
  select();

  buffer[0] = SPI_READ;
  buffer[1] = CANINTF;

  transfer(buffer, 3);
  deselect();

  uint8_t flags = buffer[2];

  interrupts->message_error     = (0x80 & flags) != 0;
  interrupts->wakeup            = (0x40 & flags) != 0;
  interrupts->error             = (0x20 & flags) != 0;
  interrupts->transmit_2_empty  = (0x10 & flags) != 0;
  interrupts->transmit_1_empty  = (0x08 & flags) != 0;
  interrupts->transmit_0_empty  = (0x04 & flags) != 0;
  interrupts->receive_1_full    = (0x02 & flags) != 0;
  interrupts->receive_0_full    = (0x01 & flags) != 0;
}

void read_receive_buffer_0(message_t *message) {

}

void read_receive_buffer_1(message_t *message) {

}
