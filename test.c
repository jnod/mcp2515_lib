#include <stdio.h>
#include "mcp2515.h"

/*******************************************************************************
  The most recently read value from the MCP2515 interrupt register (CANINTF)
  Each bit represents a different flag. The individual flags are defined in the
  mcp2515_dfs.h header.
*******************************************************************************/
uint8_t can_intr_flags;

/*******************************************************************************
  Temporary storage for incoming CAN messages.
*******************************************************************************/
can_message_t can_message;

/*******************************************************************************
  Deselects the MCP2515 by setting the slave select GPIO pin to logic high.
*******************************************************************************/
void spi_deselect_mcp2515() {
  puts("deselect");
}

/*******************************************************************************
  Selects the MCP2515 by setting the slave select GPIO pin to logic low.
*******************************************************************************/
void spi_select_mcp2515() {
  puts("select");
}

/*******************************************************************************
  Transfers data from the buffer to the slave and from the slave to the buffer
  at the same time via SPI.
*******************************************************************************/
void spi_transfer(uint8_t *buf, uint8_t len) {
  buf[2] = 0xFF;

  puts("transfer");
}

/*******************************************************************************
  Tests some of the mcp2515_lib functionality.
*******************************************************************************/
int main() {
  init_spi(spi_deselect_mcp2515, spi_select_mcp2515, spi_transfer);

  read_interrupt_flags(&can_intr_flags);

  if (IS_FLAG_SET(can_intr_flags, RX1IF)) {
    puts("RX1IF is set");
  }

  if (IS_FLAG_SET(can_intr_flags, RX0IF)) {
    puts("RX0IF is set");
  }

  return 0;
}
