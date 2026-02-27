#include "ConstDef.h"

extern uint16_t crc;
extern FLOAT	len;
extern uint32_t page_cnt;

extern __at(0x3700) uint8_t flashBuffer[256];

void FLASH_Page_read(uint32_t page);
void FLASH_Page_write(uint32_t page);
void test_flash(void);

