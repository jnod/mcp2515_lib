#include <stdint.h>

typedef void (*spi_transfer_t)(uint8_t* buf, uint8_t len);
typedef void (*spi_select_mcp2515_t)();
typedef void (*spi_deselect_mcp2515_t)();

void init_spi(spi_transfer_t st, spi_select_mcp2515_t ss, spi_deselect_mcp2515_t sd);
