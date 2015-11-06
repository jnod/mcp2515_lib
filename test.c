#include "mcp2515.h"
#include <stdio.h>

uint8_t flags;
can_message_t message;

void deselect() {
  puts("deselect");
}

void select() {
  puts("select");
}

void transfer(uint8_t *buf, uint8_t len) {
  buf[2] = 0xFF;

  puts("transfer");
}

int main() {
  init_spi(deselect, select, transfer);

  read_interrupt_flags(&flags);

  if (IS_FLAG_SET(flags, RX1IF)) {
    puts("RX1IF is set");
  }

  if (IS_FLAG_SET(flags, RX0IF)) {
    puts("RX0IF is set");
  }

  return 0;
}
