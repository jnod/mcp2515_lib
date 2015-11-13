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
    uint16_t  sid; // Only least significant 11 bits are used
    uint32_t  eid; // Only least significant 18 bits are used
    uint8_t   length;
    uint8_t   data[8];
  } CanMessage;

  void mcp2515_clearEFLG(uint8_t bit_mask);
  void mcp2515_clearCANINTF(uint8_t bit_mask);
  // config commands only work in configuration mode
  void mcp2515_configCNFn(uint8_t cnf1, uint8_t cnf2, uint8_t cnf3);
  void mcp2515_configRXF0(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXF1(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXF2(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXF3(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXF4(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXF5(uint16_t sid, uint32_t eid, char exide);
  void mcp2515_configRXM0(uint16_t sid, uint32_t eid);
  void mcp2515_configRXM1(uint16_t sid, uint32_t eid);
  void mcp2515_loadTX0(CanMessage *message);
  void mcp2515_loadTX1(CanMessage *message);
  void mcp2515_loadTX2(CanMessage *message);
  void mcp2515_readCANINTF(uint8_t *flags);
  void mcp2515_readEFLG(uint8_t *flags);
  void mcp2515_readRX0(CanMessage *message);
  void mcp2515_readRX1(CanMessage *message);
  void mcp2515_rtsTX0();
  void mcp2515_rtsTX1();
  void mcp2515_rtsTX2();
  // set commands work in every mode
  void mcp2515_setCANINTE(uint8_t caninte);
  void mcp2515_setMode(uint8_t mode);
  void mcp2515_setRXBnCTRL(uint8_t rxb0ctrl, uint8_t rxb1ctrl);
#ifdef __cplusplus
}
#endif

#endif
