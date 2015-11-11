#include <stdio.h>
#include <bcm2835.h>
#include "mcp2515.h"

char run = 1;

void mcp2515_spiTransfer(uint8_t *buf, uint8_t len) {
  bcm2835_spi_transfern(buf, len); // will select device during transfer
}

int main()
{
  // bcm2835_set_debug(1); // Test without using GPIO
  if (!bcm2835_init()) return 1;

  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_spi_begin();
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing

  while(run) {
    if (!digitalRead(9)) {
      Serial.println("Interrupt");
    }
  }

  bcm2835_spi_end();
  bcm2835_close();

  return 0;
}
