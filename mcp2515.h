#ifndef MCP2515_LIB_H_
#define MCP2515_LIB_H_

#include <stdint.h>
#include "mcp2515_dfs.h"

#ifdef __cplusplus
extern "C" {
#endif
  /* Client must implement this function. Select the mcp2515, transfer the
  buffer, and then deselect the mcp2515 */
  void mcp2515_spiTransfer(uint8_t *buf, uint8_t len);

  typedef struct {
    uint8_t   mtype; // Refer to mcp2515_dfs for type definitions
    uint16_t  std_id; // Only least significant 11 bits are used
    uint32_t  ext_id; // Only least significant 18 bits are used
    uint8_t   length;
    uint8_t   data[8];
  } can_message_t;

  void mcp2515_clearEFLG(uint8_t bit_mask);
  void mcp2515_clearCANINTF(uint8_t bit_mask);
  void mcp2515_configRXF0(uint32_t id, char is_ext_id);
  void mcp2515_configRXF1(uint32_t id, char is_ext_id);
  void mcp2515_configRXF2(uint32_t id, char is_ext_id);
  void mcp2515_configRXF3(uint32_t id, char is_ext_id);
  void mcp2515_configRXF4(uint32_t id, char is_ext_id);
  void mcp2515_configRXF5(uint32_t id, char is_ext_id);
  void mcp2515_configRXM0(uint16_t std_id, uint32_t ext_id);
  void mcp2515_configRXM1(uint16_t std_id, uint32_t ext_id);
  void mcp2515_loadTX0(can_message_t *message);
  void mcp2515_loadTX1(can_message_t *message);
  void mcp2515_loadTX2(can_message_t *message);
  void mcp2515_readCANINTF(uint8_t *flags);
  void mcp2515_readEFLG(uint8_t *flags);
  void mcp2515_readRX0(can_message_t *message);
  void mcp2515_readRX1(can_message_t *message);
  void mcp2515_rtsTX0();
  void mcp2515_rtsTX1();
  void mcp2515_rtsTX2();
  void mcp2515_setCANINTE(uint8_t caninte_value);
  void mcp2515_setCNFn(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3);
  void mcp2515_setMode(uint8_t mode);
  void mcp2515_setRXBnCTRL(uint8_t rxb0ctrl, uint8_t rxb1ctrl);
#ifdef __cplusplus
}
#endif

#endif
