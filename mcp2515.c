#include "mcp2515.h"

static void mcp2515_configRXF(uint8_t addr, uint32_t id, char is_ext_id);
static void mcp2515_configRXM(uint8_t addr, uint16_t std_id, uint32_t ext_id);
static void mcp2515_loadTX(uint8_t load_command, CanMessage *message);
static void mcp2515_readRX(uint8_t read_command, CanMessage *message);
static void mcp2515_rtsTX(uint8_t rts_command);

static uint8_t buffer[14];

/*******************************************************************************
  Clears error register (EFLG) flags that correlate with a '1' in the bit_mask
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
  Each of the following configures one of the receive filters (RXFn) to filter
  for the provided standard or extended id. The parameter is_ext_id determines
  whether id is considered to be standard or extended. Only the 11 LSB of a
  standard id or 18 LSB of an extended id are valid, the rest will be ignored.
*******************************************************************************/
void mcp2515_configRXF0(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF0, id, is_ext_id);
}

void mcp2515_configRXF1(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF1, id, is_ext_id);
}

void mcp2515_configRXF2(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF2, id, is_ext_id);
}

void mcp2515_configRXF3(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF3, id, is_ext_id);
}

void mcp2515_configRXF4(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF4, id, is_ext_id);
}

void mcp2515_configRXF5(uint32_t id, char is_ext_id) {
  mcp2515_configRXF(ADDR_RXF5, id, is_ext_id);
}

static void mcp2515_configRXF(uint8_t addr, uint32_t id, char is_ext_id) {
  if (is_ext_id) {
    id &= 0x0003FFFF; // clear 14 MSB (ext_id is only 18 bits long)

    buffer[0] = SPI_WRITE;
    buffer[1] = addr + 1;
    buffer[2] = 0x08 | ((uint8_t) (id >> 16));
    buffer[3] = (uint8_t) (id >> 8);
    buffer[4] = (uint8_t) id;

    mcp2515_spiTransfer(buffer, 5);
  } else {
    id &= 0x000007FF; // clear 21 MSB (std_id is only 11 bits long)

    buffer[0] = SPI_WRITE;
    buffer[1] = addr;
    buffer[2] = (uint8_t) (id >> 3);
    buffer[3] = (((uint8_t) id) << 5);

    mcp2515_spiTransfer(buffer, 4);
  }
}

/*******************************************************************************
  Each of the following configures one of the receive masks (RXMn). Only the 11
  LSB of std_id and 18 LSB of ext_id are valid, the rest will be ignored.
*******************************************************************************/
void mcp2515_configRXM0(uint16_t std_id, uint32_t ext_id) {
  mcp2515_configRXM(ADDR_RXM0, std_id, ext_id);
}

void mcp2515_configRXM1(uint16_t std_id, uint32_t ext_id) {
  mcp2515_configRXM(ADDR_RXM1, std_id, ext_id);
}

static void mcp2515_configRXM(uint8_t addr, uint16_t std_id, uint32_t ext_id) {
  std_id &= 0x07FF; // clear 5 MSB (std_id is only 11 bits long)
  ext_id &= 0x0003FFFF; // clear 14 MSB (ext_id is only 18 bits long)

  buffer[0] = SPI_WRITE;
  buffer[1] = addr;
  buffer[2] = (uint8_t) (std_id >> 3);
  buffer[3] = (((uint8_t) std_id) << 5) | ((uint8_t) (ext_id >> 16));
  buffer[4] = (uint8_t)(ext_id >> 8);
  buffer[5] = (uint8_t) ext_id;

  mcp2515_spiTransfer(buffer, 6);
}

/*******************************************************************************
  Loads a message into transfer buffer n (TXBn). Does not automatically send
  the message. If either id field is longer than it should be (std_id > 11 bits
  or ext_id > 18 bits), the overflow bits will be ignored.

  Returns 0 if successful, 1 otherwise.
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

#define TXBnSIDH  buffer[1]
#define TXBnSIDL  buffer[2]
#define TXBnEID8  buffer[3]
#define TXBnEID0  buffer[4]
#define TXBnDLC   buffer[5]
#define TX_EXIDE  ((message->mtype & 0x02) << 2)
#define TX_RTR    ((message->mtype & 0x01) << 6)

static void mcp2515_loadTX(uint8_t load_command, CanMessage *message) {
  buffer[0] = load_command;

  uint8_t std_id = message->std_id & 0x07FF; // 11 LSB
  uint8_t ext_id = message->ext_id & 0x0003FFFF; // 18 LSB
  uint8_t length = (message->length < 8) ? message->length : 8;

  TXBnSIDH = (uint8_t) (std_id >> 3);
  TXBnSIDL = (((uint8_t) std_id) << 5) | TX_EXIDE | ((uint8_t) (ext_id >> 16));
  TXBnEID8 = (uint8_t)(ext_id >> 8);
  TXBnEID0 = (uint8_t) ext_id;
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
  Reads the interrupt register (CANINTF) value into the *flags parameter. Each
  bit represents a different flag. The individual flags are defined in the
  mcp2515.h header.
*******************************************************************************/
void mcp2515_readCANINTF(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = ADDR_CANINTF;

  mcp2515_spiTransfer(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads the error flag register (EFLG) value into the *flags parameter. Each
  bit represents a different flag. The individual flags are defined in the
  mcp2515.h header.
*******************************************************************************/
void mcp2515_readEFLG(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = ADDR_EFLG;

  mcp2515_spiTransfer(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads and decodes the value of receive buffer 0 (RXB0) for n == 0 or receive
  buffer 1 (RXB1) for n != 0 and places the result into the *message parameter.
  For standard id messages, the ext_id data field will not be modified, and for
  data length < 8 messages, the trailing data bytes will not be modified.
*******************************************************************************/
void mcp2515_readRX0(CanMessage *message) {
  mcp2515_readRX(SPI_READ_RX0, message);
}

void mcp2515_readRX1(CanMessage *message) {
  mcp2515_readRX(SPI_READ_RX1, message);
}

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
  // message->type = 0x0R (std_id), 0x1R (ext_id), R = remote flag
  if (IDE == 0) {
    // standard id, shift standard remote flag SRR into bit 0
    message->mtype = SRR >> 4;
  } else {
    // extended id, shift extended remote flag RTR into bit 0
    message->mtype = 0x10 | (RTR >> 6);
    message->ext_id = (((uint32_t) EID17_16) << 16)
                        | (((uint32_t) RXBnEID8) << 8)
                        | ((uint32_t) RXBnEID0);
  }

  // there will always be a standard id
  message->std_id = (((uint16_t) RXBnSIDH) << 3) | (((uint16_t) RXBnSIDL) >> 5);

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

/*******************************************************************************
  Request to send transmit buffer n (TXBn).

  Returns 0 if successful, 1 otherwise.
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

static void mcp2515_rtsTX(uint8_t rts_command) {
  buffer[0] = rts_command;

  mcp2515_spiTransfer(buffer, 1);
}

/*******************************************************************************
  Sets the value of the CANINTE register. Each bit of caninte_value enables or
  disables an interrupt. See the datasheet for more detailed info.
*******************************************************************************/
void mcp2515_setCANINTE(uint8_t caninte_value) {
  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_CANINTE;
  buffer[2] = caninte_value;

  mcp2515_spiTransfer(buffer, 3);
}

/*******************************************************************************
  Sets the configuration registers (CNF1, CNF2, and CNF3) to the provided
  values. These values determine the baud rate for different oscillators. A few
  quick configurations are provided in the mcp2515.h header. More info can be
  found in the datasheet.
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
  mode can be found in the mcp2515.h header.
*******************************************************************************/
void mcp2515_setMode(uint8_t mode) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_CANCTRL;
  buffer[2] = 0xE0;
  buffer[3] = mode;

  mcp2515_spiTransfer(buffer, 4);
}

/*******************************************************************************
  Configures the receive control registers RXB0CTRL and RXB1CTRL. These
  registers determine the buffer operating mode (whether filters apply to
  standard/extended identifiers, or whether all messages are received).
  RXB0CTRL also has a bit to allow messages to overflow from RXB0 to RXB1. See
  the datasheet for more detailed info.
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
