#include "ConstDef.h"
#include "enumerate.h"

extern UINT vek[];
uint16_t crc;
FLOAT	len;
uint32_t page_cnt;

__at(0x3700) uint8_t flashBuffer[256];

void FLASH_Page_read(uint32_t page);

void Calc_CRC(uint8_t a) {
	bool b;
	crc ^= ((uint16_t)a << 8);
	for(uint8_t n = 0; n < 8; ++n) {
		b = (bool)(crc & 0x8000);
		crc = crc << 1;
		if(b == true) {
			crc ^= 0x1021;
		}
	}
}

void test_flash(void) {
	crc = 0;
	page_cnt = 0;
	len.ul = 256;
	
	while(page_cnt < len.ul) {
		FLASH_Page_read(page_cnt);
		if(page_cnt==0) {
			len.b[3] = 0;	
			len.b[2] = flashBuffer[ID_START + 25];
			len.b[1] = flashBuffer[ID_START + 26];	
			len.b[0] = flashBuffer[ID_START + 27];
			vek[reg_2].ui = len.ui[0]; 
			
			if( len.ul >= MAX_LEN_FLASH) {
				crc = 1;
				break;
			}
		}

		uint8_t ix = 0;
		do {
			Calc_CRC(flashBuffer[ix]);
			++ix;
		} while(ix > 0);
		
		vek[reg_3].b[0] = flashBuffer[254]; 
		vek[reg_3].b[1] = flashBuffer[255]; 
		page_cnt += 256;
		vek[reg_4].ui = page_cnt;
	} 
}	

void FLASH_Page_read(uint32_t page) {
    // Code sequence to read one page of PFM to Buffer Ram
    // PFM target address is specified by PAGE_ADDR
    // Load NVMADR with the base address of the memory page
    NVMADR = APP_START + page; //PAGE_ADDR;
    NVMCON1bits.CMD = 0x02; // Set the page read command
    NVMCON0bits.GO = 1; // Start page read
    while (NVMCON0bits.GO); // Wait for the read operation to complete
}

void FLASH_Page_write(uint32_t page) {
	// Code sequence to write a page of PFM
	// Input[] is the user data that needs to be written to PFM
	// PFM target address is specified by PAGE_ADDR
	//~ #define PAGESIZE 128 // PFM page size
	// Save Interrupt Enable bit Value
	uint8_t GIEBitValue = INTCON0bits.GIE;
	// Defining a pointer to the first location of the Buffer RAM
//	uint16_t *bufferRamPtr = (uint16_t*) & bufferRAM;
	//Copy application buffer contents to the Buffer RAM
	//~ for (uint8_t i = 0; i < PAGESIZE; ++i) {
		//~ *bufferRamPtr++ = Input[i];
	//~ }
	// Load NVMADR with the base address of the memory page
	NVMADR = page + APP_START;
	NVMCON1bits.CMD = 0x06; // Set the page erase command
	INTCON0bits.GIE = 0; // Disable interrupts
	//––––––––– Required Unlock Sequence –––––––––
	NVMLOCK = 0x55;
	NVMLOCK = 0xAA;
	NVMCON0bits.GO = 1; // Start page erase
	//–––––––––––––––––––––––––––––––––––––––––––––
	while (NVMCON0bits.GO); // Wait for the erase operation to complete
	// Verify erase operation success and call the recovery function if needed
	if (NVMCON1bits.WRERR){
		//~ ERASE_FAULT_RECOVERY();
	}
	// NVMADR is already pointing to target page
	NVMCON1bits.CMD = 0x05; // Set the page write command
	//––––––––– Required Unlock Sequence –––––––––
	NVMLOCK = 0x55;
	NVMLOCK = 0xAA;
	NVMCON0bits.GO = 1; // Start page write
	//–––––––––––––––––––––––––––––––––––––––––––––
	while (NVMCON0bits.GO); // Wait for the write operation to complete
	// Verify write operation success and call the recovery function if needed
	if (NVMCON1bits.WRERR){
		//~ WRITE_FAULT_RECOVERY();
	}
	INTCON0bits.GIE = GIEBitValue; // Restore interrupt enable bit value
	NVMCON1bits.CMD = 0x00; // Disable writes to memory

}

//char FLASH_ReadByte(FLOAT address) {
//    // 24-bit Program Memory Address
//    TBLPTRU = address.b[2];
//    TBLPTRH = address.b[1];
//    TBLPTRL = address.b[0];
//    asm("TBLRD*+"); // liest Byte aus Flash in TABLAT und inkrementiert TBLPTR
//    return TABLAT;
//}


