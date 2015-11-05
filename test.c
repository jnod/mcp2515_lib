#include "mcp2515.h"
#include <stdio.h>

uint8_t transfer(uint8_t value) {
  puts("transfer");
  return 0;
}

void select_mcp2515() {
  puts("select");
}

void deselect_mcp2515() {
  puts("deselect");
}

int main() {
  init_spi(&transfer, &select_mcp2515, &deselect_mcp2515);
  return 0;
}
