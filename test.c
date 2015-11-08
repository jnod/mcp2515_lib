#include <stdio.h>
#include "mcp2515.h"

void print_array(uint8_t *arr, uint8_t len);
void print_can_message(can_message_t message);
void test_read_interrupt_flags();
void test_read_message();


uint8_t can_intr_flags;
can_message_t can_message;

uint8_t fake_spi[14] = {0,1,0x08,3,4,0x43,1,2,3};

int main() {
  test_read_interrupt_flags();
  read_receive_buffer_n(1, &can_message);
  print_can_message(can_message);

  return 0;
}

void print_array(uint8_t *arr, uint8_t len) {
  uint8_t i = 0;
  printf("[");
  while (i < len) {
    if (i + 1 < len) {
      printf("%02hhx,  ", arr[i]);
    } else {
      printf("%02hhx", arr[i]);
    }
    i++;
  }
  puts("]");
}

void print_can_message(can_message_t message) {
  printf("type:   %02hhx\n", message.mtype);
  printf("std_id: %02hhx\n", message.std_id);
  printf("ext_id: %02hhx\n", message.ext_id);
  printf("length: %02hhx\n", message.length);
  printf("data:   ");
  print_array(message.data, message.length);
}

/*******************************************************************************
  Declared in mcp2515.h, but isn't implemented by the library. This particular
  implemenation simulates an SPI interface and prints the results.
*******************************************************************************/
void spi_transfer_mcp2515(uint8_t *buf, uint8_t len) {
  uint8_t i = 0;

  printf("MOSI: [");
  while (i < len) {
    if (i + 1 < len) {
      printf("%02hhx,  ", buf[i]);
    } else {
      printf("%02hhx]\n", buf[i]);
    }
    buf[i] = fake_spi[i];
    i++;
  }

  printf("MISO: ");
  print_array(buf, len);
}

void test_read_interrupt_flags() {
  read_interrupt_flags(&can_intr_flags);

  if (IS_FLAG_SET(can_intr_flags, MASK_RX1IF)) {
    puts("RX1IF is set");
  }

  if (IS_FLAG_SET(can_intr_flags, MASK_RX0IF)) {
    puts("RX0IF is set");
  }
}
