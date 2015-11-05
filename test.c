#include "mcp2515.h"
#include <stdio.h>

void spi_transfer(uint8_t* buf, uint8_t len) {
  puts("transfer");
}

void select_mcp2515() {
  puts("select");
}

void deselect_mcp2515() {
  puts("deselect");
}

int main() {
  init_spi(&spi_transfer, &select_mcp2515, &deselect_mcp2515);
  return 0;
}
