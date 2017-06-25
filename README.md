# mcp2515_lib

A platform-independent library that communicates with the [MCP2515](http://ww1.microchip.com/downloads/en/DeviceDoc/21801d.pdf) via SPI.

This library has been successfully tested with Arduino and Raspberry Pi.

To use:
* Include the mcp2515.h header file.
* Implement the mcp2515_spiTransfer(uint8_t *buf, uint8_t len) method. This method should select the mcp2515, transfer the buffer, and then deselect the mcp2515.
