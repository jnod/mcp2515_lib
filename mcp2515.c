#include "mcp2515.h"

static uint8_t buffer[14];

/*******************************************************************************
  Clears interrupt register (CANINTF) flags that correlate with a '1' in the
  bit_mask parameter.
*******************************************************************************/
void clear_interrupt_flags(uint8_t bit_mask) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_CANINTF;
  buffer[2] = bit_mask;
  buffer[3] = 0x00;

  spi_transfer_mcp2515(buffer, 4);
}

/*******************************************************************************
  Sets the configuration registers (CNF1, CNF2, and CNF3) to the provided
  values. These values determine the baud rate for different oscillators. A few
  quick configurations are provided in the mcp2515.h header. More info can be
  found in the datasheet.
*******************************************************************************/
void config_timing(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3) {
  buffer[0] = SPI_WRITE;
  buffer[1] = ADDR_CNF1;
  buffer[2] = cnf1;
  buffer[3] = cnf2;
  buffer[4] = cnf3;

  spi_transfer_mcp2515(buffer, 5);
}

/*******************************************************************************
  Reads the interrupt register (CANINTF) value into the *flags parameter. Each
  bit represents a different flag. The individual flags are defined in the
  mcp2515.h header.
*******************************************************************************/
void read_interrupt_flags(uint8_t *flags) {
  buffer[0] = SPI_READ;
  buffer[1] = ADDR_CANINTF;

  spi_transfer_mcp2515(buffer, 3);

  *flags = buffer[2];
}

/*******************************************************************************
  Reads and decodes the value of receive buffer 0 (RXB0) for n == 0 or receive
  buffer 1 (RXB1) for n != 0 and places the result into the *message parameter.
  For standard id messages, the ext_id data field will not be modified, and for
  data length < 8 messages, the trailing data bytes will not be modified.
*******************************************************************************/
#define RXB1SIDH  buffer[1]
#define RXB1SIDL  buffer[2]
#define RXB1EID8  buffer[3]
#define RXB1EID0  buffer[4]
#define RXB1DLC   buffer[5]
#define IDE       (RXB1SIDL & 0x08)
#define SRR       (RXB1SIDL & 0x10)
#define RTR       (RXB1DLC  & 0x40)
#define EID17_16  (RXB1SIDL & 0x03)
#define DLC3_0    (RXB1DLC  & 0x0F)

void read_receive_buffer_n(uint8_t n, can_message_t *message) {
  buffer[0] = (n == 0) ? SPI_READ_RX0 : SPI_READ_RX1;

  spi_transfer_mcp2515(buffer, 14);

  // check IDE bit for id type, IDE == 0 indicates standard id
  // message->type = 0x0R (std_id), 0x1R (ext_id), R = remote flag
  if (IDE == 0) {
    // standard id, shift standard remote flag SRR into bit 0
    message->type = SRR >> 4;
  } else {
    // extended id, shift extended remote flag RTR into bit 0
    message->type = 0x10 | (RTR >> 6);

    message->ext_id = (((uint32_t) EID17_16) << 16)
                        | (((uint32_t) RXB1EID8) << 8)
                        | ((uint32_t) RXB1EID0);
  }

  // there will always be a standard id
  message->std_id = (((uint16_t) RXB1SIDH) << 3) | (((uint16_t) RXB1SIDL) >> 5);

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
  Sets the filter associated with the provided filter address. Each filter
  actually consists of 4 registers, but this function simplifies the process.
  Each filter address is defined in the mcp2515.h header.
*******************************************************************************/
void set_filter(uint8_t filter_addr, uint16_t std_id, uint32_t ext_id) {

}

/*******************************************************************************
  Sets the mask associated with the provided mask address. Each mask actually
  consists of 4 registers, but this function simplifies the process. Each mask
  address is defined in the mcp2515.h header.
*******************************************************************************/
void set_mask(uint8_t filter_addr, uint16_t std_id, uint32_t ext_id) {

}

/*******************************************************************************
  Sets the mode of operation. Defaults is configuration on startup.
*******************************************************************************/
void set_mode(uint8_t mode) {
  buffer[0] = SPI_BIT_MODIFY;
  buffer[1] = ADDR_CANCTRL;
  buffer[2] = 0xE0;
  buffer[3] = mode;

  spi_transfer_mcp2515(buffer, 4);
}
