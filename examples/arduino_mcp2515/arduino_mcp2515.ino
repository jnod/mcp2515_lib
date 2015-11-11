#include <SPI.h>
#include <mcp2515.h>

static can_message_t can_message;

void spi_transfer_mcp2515(uint8_t *buf, uint8_t len) {
  // TODO: Select the MCP2515 if SPI.transfer(buf,len) doesn't
  SPI.transfer(buf, len);
  // TODO: Deselect the MCP2515 if SPI.transfer(buf,len) doesn't
}

void print_can_message(can_message_t message) {
  Serial.println(message.mtype, HEX);
  Serial.println(message.std_id, DEC);
  Serial.println(message.ext_id, DEC);
  Serial.println(message.length, DEC);
  // TODO: Print data
}

void setup() {
  can_message = {MTYPE_EXTENDED_DATA, 5, 6, 2, {1,2}};
  pinMode(9, INPUT); // Used as the interrupt (INT) pin

  Serial.begin(115200);
  Serial.println("Start Serial");

  delay(50); // Allow the MCP2515 time to start. May not be necessary.

  config_interrupts(0x03); // Inturrupt when a message is received
  config_timing(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  config_receive_ctrl(0x60, 0x60); // Ignore filters, receive all messages
  set_mode(MODE_LOOPBACK); // Loopback sends messages to itself for testing
}

void loop() {
  // The INT pin will go low when there is an interrupt
  if (!digitalRead(9)) {
    Serial.println("Interrupt");
  }
}
