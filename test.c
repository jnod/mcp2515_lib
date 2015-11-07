#include <stdio.h>
#include "mcp2515.h"

uint8_t can_intr_flags;
can_message_t can_message;

int main() {
  read_interrupt_flags(&can_intr_flags);

  if (IS_FLAG_SET(can_intr_flags, MASK_RX1IF)) {
    puts("RX1IF is set");
  }

  if (IS_FLAG_SET(can_intr_flags, MASK_RX0IF)) {
    puts("RX0IF is set");
  }

  return 0;
}

/*******************************************************************************
  Declared in mcp2515.h, but isn't implemented by the library.
*******************************************************************************/
void spi_transfer_mcp2515(uint8_t *buf, uint8_t len) {
  buf[2] = 0xF2;

  puts("transfer");
}
