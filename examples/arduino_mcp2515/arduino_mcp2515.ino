#include <SPI.h>
#include <mcp2515.h>

#define INT   9
#define SS    10
#define MOSI  11
#define MISO  12
#define SCK   13

void printCanMessage();

static CanMessage message = {MTYPE_EXTENDED_DATA, 5, 6, 2, {1,2}};

void setup() {
  pinMode(INT, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();

  Serial.begin(115200);
  Serial.println("Start Serial");

  delay(100); // Allows the mcp2515 to initialize

  mcp2515_reset();
  mcp2515_configCNFn(CNF1_10MHZ_125KBIT, CNF2_10MHZ_125KBIT, CNF3_10MHZ_125KBIT);
  mcp2515_setCANINTE(0x03); // Inturrupt when a message is received
  mcp2515_setRXBnCTRL(0x60, 0x60); // Ignore filters, receive all messages
  mcp2515_setMode(MODE_LOOPBACK); // Loopback sends messages to itself for testing

  Serial.print("Transmitted message:    ");
  printCanMessage();
  mcp2515_clearCANINTF(0xFF);
  
  mcp2515_loadTX0(&message);
  mcp2515_rtsTX0();

  message = {0}; // resets the message to 0 to verify readRX0 works properly
  Serial.print("Message before readRX0: ");
  printCanMessage();
}

void loop() {
  // The INT pin will go low when there is an interrupt
  if (!digitalRead(INT)) {
    Serial.print("Interrupt\nMessage after readRX0:  ");
    mcp2515_readRX0(&message);
    printCanMessage();
    mcp2515_clearCANINTF(0xFF);
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
