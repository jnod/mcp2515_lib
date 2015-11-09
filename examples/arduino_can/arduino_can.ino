#include <SPI.h>
#include <mcp2515.h>

static can_message_t can_message;

void spi_transfer_mcp2515(uint8_t *buf, uint8_t len) {
  SPI.transfer(buf, len);
}

void print_can_message(can_message_t message) {
  Serial.println(message.mtype, HEX);
  Serial.println(message.std_id, DEC);
  Serial.println(message.ext_id, DEC);
  Serial.println(message.length, DEC);
}

void setup() {
  can_message = {MTYPE_EXTENDED_DATA, 5, 6, 2, {1,2}};
  pinMode(9, INPUT);
  
  Serial.begin(115200);
  Serial.println("Start Serial");
  
  delay(50);
  
  config_interrupts(0x03);
  config_timing(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  config_receive_ctrl(0x60, 0x60);
  set_mode(MODE_LOOPBACK);
}

void loop() {
  if (!digitalRead(9)) {
    Serial.println("Interrupt");
  }
}
