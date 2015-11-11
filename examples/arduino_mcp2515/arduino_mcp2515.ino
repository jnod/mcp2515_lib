#include <SPI.h>
#include <mcp2515.h>

static CanMessage can_message;

void mcp2515_spiTransfer(uint8_t *buf, uint8_t len) {
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

  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing
}

void loop() {
  // The INT pin will go low when there is an interrupt
  if (!digitalRead(9)) {
    Serial.println("Interrupt");
  }
}
