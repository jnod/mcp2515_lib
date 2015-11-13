#include "mcp2515.h"

static void mcp2515_configRX(uint8_t addr,uint16_t sid,uint32_t eid,char exide);
static void mcp2515_loadTX(uint8_t load_command, CanMessage *message);
static void mcp2515_readRX(uint8_t read_command, CanMessage *message);
static void mcp2515_rtsTX(uint8_t rts_command);

static uint8_t buffer[14];

/*******************************************************************************
  Clears error register flags (EFLG) that correlate with a '1' in the bit_mask
  parameter. Only the first two bits (receive overflow 0 and 1) are clearable.
*******************************************************************************/
void mcp2515_clearEFLG(uint8_t bit_mask) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_EFLG;
  buffer[2] = bit_mask;
  buffer[3] = 0x00;

  mcp2515_spiTransfer(buffer, 4);
}

/*******************************************************************************
  Clears interrupt register (CANINTF) flags that correlate with a '1' in the
  bit_mask parameter.
*******************************************************************************/
void mcp2515_clearCANINTF(uint8_t bit_mask) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_CANINTF;
  buffer[2] = bit_mask;
  buffer[3] = 0x00;

  mcp2515_spiTransfer(buffer, 4);
}

/*******************************************************************************
  Configures receive filter n (RXFn). Only the 11 LSB of sid and 18 LSB of eid
  are valid, the rest will be ignored. For standard ID filters (exide == 0),
  the 16 LSB of eid will filter the first 2 data bytes.
*******************************************************************************/
void mcp2515_configRXF0(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF0, sid, eid, exide);
}

void mcp2515_configRXF1(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF1, sid, eid, exide);
}

void mcp2515_configRXF2(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF2, sid, eid, exide);
}

void mcp2515_configRXF3(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF3, sid, eid, exide);
}

void mcp2515_configRXF4(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF4, sid, eid, exide);
}

void mcp2515_configRXF5(uint16_t sid, uint32_t eid, char exide) {
  mcp2515_configRX(ADDR_RXF5, sid, eid, exide);
}

/*******************************************************************************
  Configures receive mask n (RXMn). Only the 11 LSB of sid and 18 LSB of eid
  are valid, the rest will be ignored. For standard ID messages, the 16 LSB of
  eid will mask the first 2 data bytes.
*******************************************************************************/
void mcp2515_configRXM0(uint16_t sid, uint32_t eid) {
  mcp2515_configRX(ADDR_RXM0, sid, eid, 0);
}

void mcp2515_configRXM1(uint16_t sid, uint32_t eid) {
  mcp2515_configRX(ADDR_RXM1, sid, eid, 0);
}

/*******************************************************************************
  Loads a message into transfer buffer n (TXBn). Does not automatically send
  the message. Only the 11 LSB of sid and 18 LSB of eid are valid, the
  rest will be ignored.
*******************************************************************************/
void mcp2515_loadTX0(CanMessage *message) {
  mcp2515_loadTX(SPI_LOAD_TX0, message);
}

void mcp2515_loadTX1(CanMessage *message) {
  mcp2515_loadTX(SPI_LOAD_TX1, message);
}

void mcp2515_loadTX2(CanMessage *message) {
  mcp2515_loadTX(SPI_LOAD_TX2, message);
}

/*******************************************************************************
  Reads the interrupt register (CANINTF) value into the *flags parameter. Each
  bit represents a different flag.
*******************************************************************************/
void mcp2515_readCANINTF(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = ADDR_CANINTF;

  mcp2515_spiTransfer(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads the error flag register (EFLG) value into the *flags parameter. Each
  bit represents a different flag.
*******************************************************************************/
void mcp2515_readEFLG(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = ADDR_EFLG;

  mcp2515_spiTransfer(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads and decodes the value of receive buffer n (RXBn) and places the result
  into the *message parameter. For standard id messages, the eid data field
  will not be modified, and for data length < 8 messages, the trailing data
  bytes will not be modified.
*******************************************************************************/
void mcp2515_readRX0(CanMessage *message) {
  mcp2515_readRX(SPI_READ_RX0, message);
}

void mcp2515_readRX1(CanMessage *message) {
  mcp2515_readRX(SPI_READ_RX1, message);
}

/*******************************************************************************
  Request to send the message residing in transmit buffer n (TXBn).
*******************************************************************************/
void mcp2515_rtsTX0() {
  mcp2515_rtsTX(SPI_RTS_TX0);
}

void mcp2515_rtsTX1() {
  mcp2515_rtsTX(SPI_RTS_TX1);
}

void mcp2515_rtsTX2() {
  mcp2515_rtsTX(SPI_RTS_TX2);
}

/*******************************************************************************
  Sets the value of the CANINTE register. Each bit of caninte enables or
  disables an interrupt. Refer to the datasheet for more detailed info.
*******************************************************************************/
void mcp2515_setCANINTE(uint8_t caninte) {
  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_CANINTE;
  buffer[2] = caninte;

  mcp2515_spiTransfer(buffer, 3);
}

/*******************************************************************************
  Sets the configuration registers (CNF1, CNF2, and CNF3) to the provided
  values. These values determine the baud rate for different oscillators. A few
  quick configurations are provided in the mcp2515_dfs.h header. Refer to the
  datasheet for more detailed info.
*******************************************************************************/
void mcp2515_setCNFn(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3) {
  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_CNF1;
  buffer[2] = cnf1;
  buffer[3] = cnf2;
  buffer[4] = cnf3;

  mcp2515_spiTransfer(buffer, 5);
}

/*******************************************************************************
  Sets the mode of operation. Defaults to configuration mode on startup. Each
  mode can be found in the mcp2515_dfs.h header.
*******************************************************************************/
void mcp2515_setMode(uint8_t mode) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_CANCTRL;
  buffer[2] = 0xE0;
  buffer[3] = mode;

  mcp2515_spiTransfer(buffer, 4);
}

/*******************************************************************************
  Configures the receive control registers RXB0CTRL and RXB1CTRL. Refer to the
  datasheet for more detailed info.
*******************************************************************************/
void mcp2515_setRXBnCTRL(uint8_t rxb0ctrl, uint8_t rxb1ctrl) {
  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_RXB0CTRL;
  buffer[2] = rxb0ctrl;

  mcp2515_spiTransfer(buffer, 3);

  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_RXB1CTRL;
  buffer[2] = rxb0ctrl;

  mcp2515_spiTransfer(buffer, 3);
}

/*******************************************************************************
  Configures the indicated receive filter or mask.
*******************************************************************************/
static void mcp2515_configRX(uint8_t addr,uint16_t sid,uint32_t eid,char exide){
  sid &= 0x07FF; // clear 5 MSB (sid is only 11 bits long)
  eid &= 0x0003FFFF; // clear 14 MSB (eid is only 18 bits long)
  exide = (exide) ? 0x08 : 0x00;

  buffer[0] = SPI_WRITE;
  buffer[1] = addr;
  buffer[2] = (uint8_t) (sid >> 3);
  buffer[3] = (((uint8_t) sid) << 5) | exide | ((uint8_t) (eid >> 16));
  buffer[4] = (uint8_t) (eid >> 8);
  buffer[5] = (uint8_t) eid;

  mcp2515_spiTransfer(buffer, 6);
}

/*******************************************************************************
  Loads a message into the indicated transfer buffer.
*******************************************************************************/
static void mcp2515_loadTX(uint8_t load_command, CanMessage *message) {
  #define TXBnSIDH  buffer[1]
  #define TXBnSIDL  buffer[2]
  #define TXBnEID8  buffer[3]
  #define TXBnEID0  buffer[4]
  #define TXBnDLC   buffer[5]
  #define TX_EXIDE  ((message->mtype & 0x02) << 2)
  #define TX_RTR    ((message->mtype & 0x01) << 6)

  buffer[0] = load_command;

  uint16_t sid = message->sid & 0x07FF; // 11 LSB
  uint32_t eid = message->eid & 0x0003FFFF; // 18 LSB
  uint8_t length = (message->length < 8) ? message->length : 8;

  TXBnSIDH = (uint8_t) (sid >> 3);
  TXBnSIDL = (((uint8_t) sid) << 5) | TX_EXIDE | ((uint8_t) (eid >> 16));
  TXBnEID8 = (uint8_t)(eid >> 8);
  TXBnEID0 = (uint8_t) eid;
  TXBnDLC  = TX_RTR | length;

  uint8_t i = 0;
  uint8_t *transmit_data = &buffer[6];
  uint8_t *message_data = &message->data[0];

  while (i < length) {
    transmit_data[i] = message_data[i];
    i++;
  }

  mcp2515_spiTransfer(buffer, (6 + length));
}

/*******************************************************************************
  Requests that the indicated transfer buffer be sent.
*******************************************************************************/
static void mcp2515_rtsTX(uint8_t rts_command) {
  buffer[0] = rts_command;

  mcp2515_spiTransfer(buffer, 1);
}

/*******************************************************************************
  Reads the indicated receive buffer.
*******************************************************************************/
static void mcp2515_readRX(uint8_t read_command, CanMessage *message) {
  #define RXBnSIDH  buffer[1]
  #define RXBnSIDL  buffer[2]
  #define RXBnEID8  buffer[3]
  #define RXBnEID0  buffer[4]
  #define RXBnDLC   buffer[5]
  #define IDE       (RXBnSIDL & 0x08)
  #define SRR       (RXBnSIDL & 0x10)
  #define RTR       (RXBnDLC  & 0x40)
  #define EID17_16  (RXBnSIDL & 0x03)
  #define DLC3_0    (RXBnDLC  & 0x0F)

  buffer[0] = read_command;

  mcp2515_spiTransfer(buffer, 14);

  // check IDE bit for id type, IDE == 0 indicates standard id
  // message->type = 0x0R (sid), 0x1R (eid), R = remote flag
  if (IDE == 0) {
    // standard id, shift standard remote flag SRR into bit 0
    message->mtype = SRR >> 4;
  } else {
    // extended id, shift extended remote flag RTR into bit 0
    message->mtype = 0x02 | (RTR >> 6);
    message->eid = (((uint32_t) EID17_16) << 16)
                        | (((uint32_t) RXBnEID8) << 8)
                        | ((uint32_t) RXBnEID0);
  }

  // there will always be a standard id
  message->sid = (((uint16_t) RXBnSIDH) << 3) | (((uint16_t) RXBnSIDL) >> 5);

  uint8_t length = DLC3_0;
  if (length > 8) length = 8; // ensure length is in the valid 0-8 byte range
  message->length = length;

  uint8_t i = 0;
  uint8_t *receive_data = &buffer[6];
  uint8_t *message_data = &message->data[0];

  while (i < length) {
    message_data[i] = receive_data[i];
    i++;
  }
}
