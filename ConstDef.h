#ifndef CONSTDEF
#define CONSTDEF 
#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include <string.h>
#include <stdio.h>

//~ #define FLASH_ID 0x100 //130944

#define SBUF0_LAENGE 128

#define BUSDELMST 0x10
#define BUSDEL	  0x02
#define COMRESDEL 60

#define START_UPDATE 0xaaaa 

#define IVT_BASE  0x08
#define APP_START 0x2000
#define ID_START  0x08
#define MAX_LEN_FLASH (0x1FFFF - APP_START) 

typedef union FLOAT {                   // byte-addressable FLOAT
  float f;
  uint8_t b[4];
  uint32_t ul;
  uint16_t ui[2];
  int16_t  i[2];
} FLOAT;



typedef union UINT {                    // byte-addressable int
  uint8_t  b[2];
  uint16_t ui;
  int16_t   i;
} UINT;

typedef struct EMPF_BUF {
	uint8_t buf[SBUF0_LAENGE];
	uint8_t cnt;
} EMPF_BUF;


#endif
