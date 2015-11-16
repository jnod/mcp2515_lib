# mcp2515_lib

A platform-independent library that communicates with the [MCP2515](http://ww1.microchip.com/downloads/en/DeviceDoc/21801d.pdf) via SPI. Still under development.

The Arduino example has been tested in loopback mode, and will be tested on a CAN system soon. The Raspberry Pi example has not been tested yet.

To use this library:
1. Include the mcp2515.h header file.
2. Implement the mcp2515_spiTransfer(uint8_t *buf, uint8_t len) method. This method should select the mcp2515, transfer the buffer, and then deselect the mcp2515.
