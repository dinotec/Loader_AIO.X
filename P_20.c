//#include <stdlib.h>
#include <string.h>
#include "ConstDef.h"                 // SFR declarations
#include "main.h"
#include "eeprom.h"
#include "bus.h"
#include "flash.h"

#include "enumerate.h"

extern char id[]; 
uint8_t	ret_adr;
void send_status(void);
bool status_req;
EMPF_BUF buf0;
UINT vek[wrteptr + 1];
bool update;

void P20_ini() {
    //~ FLOAT f; 
    update = false;	
    //~ f.ul = FLASH_ID;  
	//~ vek[l_adr2].b[0] =  1 + 0x20;
	//~ vek[l_adr1].b[0] =  2 + 0x20;
	//~ vek[l_adr0].b[0] =  3 + 0x20;
	//~ vek[sh_adr].b[0] = 223 + 0x20;
	//~ vek[hardware].ui = 2601;
    //~ vek[sofver].i = atoi(id + 21);
    //~ vek[wrteptr].ui = 0xffff;
    memset(&buf0, 0, sizeof(buf0));
}
void ident_daten(uint8_t* ptr) {
	#define DEVICETYPE (atoi(id))
	#define DEVICETYPEH ((DEVICETYPE / 224) + 32)
	#define DEVICETYPEL ((DEVICETYPE % 224) + 32)
	*ptr        = DEVICETYPEH;
	*(ptr + 1)  = DEVICETYPEL;
	*(ptr + 2)  = vek[l_adr2].b[0];
	*(ptr + 3)  = vek[l_adr1].b[0];
	*(ptr + 4)  = vek[l_adr0].b[0];
}

bool CHKWC(uint8_t val, uint8_t cmp) {	
	#define WILDCARD 0x17
	if (val == WILDCARD || val == cmp) 
		return 1;
	else
		return 0;
}

void senden(void) {
	buf0.cnt = 1;
	U3TXB    = buf0.buf[0];
	U3TXIE   = 1;
}


void bus_rt(void) {
	uint8_t c, sum;
	
	sum = 0;
	if(buf0.cnt > 0 ) 
		for(c = 0; c < buf0.cnt - 1; ++c)
			sum += buf0.buf[c];
	sum |= 0x20;
	
	if(sum == buf0.buf[buf0.cnt - 1]) {	
		uint8_t ident[5];
		if(buf0.buf[0] == 0x08) {	
			ident_daten(ident);
			if(!memcmp(ident, buf0.buf+1,5)) {
				vek[sh_adr].ui = buf0.buf[6];
                UINT tmp;
                tmp.ui = sh_adr;
                //~ eep_write(tmp, vek[sh_adr].b[0], 0);
                memset(&buf0, 0, sizeof(buf0));
                //~ buf0.buf[0] = 0x0d;	
                //~ U3TXB  = buf0.buf[0];
                //~ U3TXIE = 1;
                senden();
                //~ COMRESCTR = COMRESDEL;
			}
		} else if(buf0.buf[0] == 0x09) {
			ident_daten(ident);
			for(c = 1; c < 6; ++c) {
				if(!CHKWC(buf0.buf[c], ident[c - 1]))
					break;
            }
			if(c == 6) {
				ret_adr = buf0.buf[6];
				status_req = 1;
				buf0.cnt = 0;
			}
            //~ COMRESCTR = COMRESDEL;
		} else if(buf0.buf[1] == vek[sh_adr].ui) {
			switch(buf0.buf[0]) {
				case 0x11: // Register auslesen
					if(buf0.buf[2] >= 0x20) {	
						uint16_t wert;
						c = buf0.buf[2] - 0x20; // Register
                        wert = vek[c].ui;
						memset(&buf0,0,sizeof(buf0));
						for(c = 3; c <= 3; --c) {
							buf0.buf[c]  = 0x30 + (wert & 0x0f);
							buf0.buf[4] += buf0.buf[c];
							wert >>= 4;
						}
						buf0.buf[4] |= 0x20;
						buf0.buf[5]  = 0x0d;
                        //~ buf0.cnt = 1;
                        //~ U3TXB    = buf0.buf[0]; 
                        //~ U3TXIE   = 1;
                        senden();
					}
                    //~ COMRESCTR = COMRESDEL;
				break;	
				case 0x12: // 1.2.9	Registerinhalt verändern
					if(buf0.buf[2] >= 0x20) {	
						uint16_t wert;
						bool ok;
						ok = true;
						c = buf0.buf[2] - 0x20; // Register
						wert = buf0.buf[3] & 0x0f;
						wert <<= 4;
						wert  |= buf0.buf[4] & 0x0f;
						wert <<= 4;
						wert  |= buf0.buf[5] & 0x0f;
						wert <<= 4;
						wert  |= buf0.buf[6] & 0x0f;
						vek[c].ui = wert;
						switch(c) {
							case rst_bl:
								if(vek[c].ui == START_UPDATE) { // 43690
									update_start = true;
									//~ update = true;
									//~ page_cnt = 0;
									//~ memset(&buf0,0,sizeof(buf0));
									//~ sprintf(buf0.buf, "Block %lu\r\n", page_cnt);
									//~ goto SEND;	
								}
								break;
							default: 
								break;
						}
						memset(&buf0,0,sizeof(buf0));
						if(ok) {	
                            buf0.buf[0] = 0x0d;
							//~ buf0.buf[1] = 0x01;
                            SEND:
                            //~ U3TXIE = 1;
                            //~ U3TXB  = buf0.buf[0]; 
                            senden();
                        }
					}
					else
						memset(&buf0,0,sizeof(buf0));
				break;	
				default:
					memset(&buf0,0,sizeof(buf0));
				break;	
			}
		} else
			memset(&buf0,0,sizeof(buf0));
	} else
		memset(&buf0,0,sizeof(buf0));
}	

void send_status(void) {
	uint8_t n;
	memset(&buf0,0,sizeof(buf0));
	buf0.cnt = 0;
	buf0.buf[1] = 0x15;
	buf0.buf[2] = ret_adr;
	ident_daten(buf0.buf + 3);
	buf0.buf[8] = vek[sh_adr].b[0];
	strncat((char*)buf0.buf + 1, id + 5, 16);
	for(n = 0; n < 16; ++n)
		buf0.buf[25 + n] = vek[reg192 + n].b[0];// ee_rdb_8k(BK_OFFSET + n);
	buf0.buf[41] = 0x16;
	for(buf0.cnt = 0; buf0.cnt < 42; ++buf0.cnt)
		buf0.buf[42] += buf0.buf[buf0.cnt];
	buf0.buf[42] |= 0x20;
	buf0.buf[43]  = 0x0d;
	buf0.buf[44]  = 0x1f;
	//~ buf0.cnt = 1;
	//~ U3TXB    = buf0.buf[0]; 
    //~ U3TXIE   = 1;
    senden();
}
