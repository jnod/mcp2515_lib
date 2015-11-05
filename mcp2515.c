#include "mcp2515.h"

spi_transfer_t transfer;
spi_select_mcp2515_t select;
spi_deselect_mcp2515_t deselect;

void init_spi(spi_transfer_t st, spi_select_mcp2515_t ss, spi_deselect_mcp2515_t sd) {
  transfer = st;
  select = ss;
  deselect = sd;

  select();
  transfer(1);
  deselect();
}
