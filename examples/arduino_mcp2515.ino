#include <SPI.h>

extern "C" {
  #include "mcp2515.h"
}

#define AUTH  8
#define INT   9
#define SS    10
#define MOSI  11
#define MISO  12
#define SCK   13

#define DEVICE_ID 0x0005 // 1234 in decimal

void printCanMessage();

static CanMessage message = {MTYPE_STANDARD_DATA, 0x0400 | DEVICE_ID, 0, 1, {0}};

void setup() {
  pinMode(AUTH, OUTPUT);
  pinMode(INT, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);

  digitalWrite(AUTH, LOW);
  digitalWrite(SS, HIGH);
  SPI.begin();

  Serial.begin(115200);
  Serial.println("Start Serial");

  delay(100); // Allows the mcp2515 to initialize

  mcp2515_reset();
  mcp2515_configCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  // mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing
  mcp2515_setMode(MODE_NORMAL); // Normal mode allows communication over CAN

  Serial.print("Transmit: ");
  printCanMessage();
  mcp2515_clearCANINTF(0xFF);

  mcp2515_loadTX0(&message);
  mcp2515_rtsTX0();

  // message = {0}; // resets the message to 0 to verify readRX0 works properly
  // Serial.print("Message before readRX0: ");
  // printCanMessage();
}

void loop() {
  // The INT pin will go low when there is an interrupt
  if (!digitalRead(INT)) {
    Serial.print("Read: ");
    mcp2515_readRX0(&message);
    printCanMessage();
    mcp2515_clearCANINTF(0xFF);

    if (message.mtype == MTYPE_STANDARD_DATA && message.sid == 1) {
      uint8_t* data = &message.data[0];
      int device_id = data[0] << 3 | data[1] << 2 | data[2] << 1 | data[3];

      if (device_id == DEVICE_ID) {
        if (data[4] == 0) {
          digitalWrite(AUTH, LOW);
        } else {
          digitalWrite(AUTH, HIGH);
        }
      }
    }
  }
}

void mcp2515_spiTransfer(uint8_t *buf, uint8_t len) {
  digitalWrite(SS, LOW);
  SPI.transfer(buf, len);
  digitalWrite(SS, HIGH);
}

void printCanMessage() {
  Serial.print("{ Type: ");
  Serial.print(message.mtype, DEC);
  Serial.print(", sid: ");
  Serial.print(message.sid, DEC);
  Serial.print(", eid: ");
  Serial.print(message.eid, DEC);
  Serial.print(", length: ");
  Serial.print(message.length, DEC);
  Serial.print(", data: [");

  uint8_t i = 0;
  while (i < message.length) {
    Serial.print(message.data[i], DEC);
    i++;

    if (i < message.length) Serial.print(", ");
  }

  Serial.println("] }");
}
