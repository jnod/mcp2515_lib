#include <stdio.h>
#include <bcm2835.h>
#include "mcp2515.h"

void spi_transfer_mcp2515(uint8_t *buf, uint8_t len) {
  bcm2835_spi_transfern(buf, len); // will select device during transfer
}

int main()
{
  // bcm2835_set_debug(1); // Test without using GPIO
  if (!bcm2835_init()) return 1;

  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_spi_begin();
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

  config_interrupts(0x03); // Inturrupt when a message is received
  config_timing(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  config_receive_ctrl(0x60, 0x60); // Ignore filters, receive all messages
  set_mode(MODE_LOOPBACK); // Loopback sends messages to itself for testing

  while(true) {
    if (!digitalRead(9)) {
      Serial.println("Interrupt");
    }
  }

  bcm2835_spi_end();
  bcm2835_close();

  return 0;
}
