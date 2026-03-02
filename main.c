#include "ConstDef.h"
#include "bus.h"
#include "P_20.h"
#include "eeprom.h"
#include "enumerate.h"
#include "flash.h"

#include <stdint.h> 
bool rx_ready  = false,
     tmr1_flag = false,
     app_start = false;
     
int main(void) {
    uint16_t zlr = 0, cnt = 0;
    #pragma config LVP = ON
    SYSTEM_Initialize();
    #pragma config LVP = ON
    // Enable the Global High Interrupts 
    INTERRUPT_GlobalInterruptHighEnable(); 
    // Enable the Global Low Interrupts 
    INTERRUPT_GlobalInterruptLowEnable(); 

    // Disable the Global Low Interrupts 
    //INTERRUPT_GlobalInterruptLowDisable(); 
    UART3_Enable();
    UART3_TransmitEnable();
    UART3_ReceiveEnable();
    UART3_Init();
    P20_ini();
    eep_ini();
    
    TMR1_Start();
//    test_flash();
//	vek[reg_1].ui = crc;
//    page_cnt = 0;
    
    while(1) {
        if(++cnt > 1000)
            cnt = 0;
        if(tmr1_flag) {
            tmr1_flag = false;
            ++zlr;
			if(status_req) {
				status_req = 0;
				send_status();
			}
        }
        if (rx_ready) {
            rx_ready = 0;
            if(update == false)
				bus_rt();
			else {
				if(!memcmp(buf0.buf, "FINISH", 6)) {
					update  = false;
					page_cnt = 0;
					RESET();
				} else {
					memcpy(flashBuffer, buf0.buf, 256);
					FLASH_Page_write(page_cnt);
					memset(&buf0, 0, sizeof(buf0));
					page_cnt += 256;
					cnt = 0;
					if(page_cnt > (MAX_LEN_FLASH - 256)) {
						update = false;
						RESET();
					} else {	
						SEND_BLOCK:
						sprintf(buf0.buf, "Block %lu\r\n", page_cnt);
						U1TXIE = 1;
					}
				}	
			}	
        }
            
        if (zlr >= 62) { // Sekundenroutinen
            if(vek[rst_bl].ui == 0xaa55 && !crc) {
                STARTAPP:
                GIE = 0;
				TBLPTRU = 0;
				TBLPTRH = 0;
				TBLPTRL = 0;
				STKPTR = 0; 
				WDTCON0bits.SEN = 0; // WDT ausschalten (falls per Software steuerbar)
				asm("CLRWDT");  
				IVTBASEU = 0x00; 
				IVTBASEH = 0x20; 
				IVTBASEL = 0x36;
                TMR1_Stop();				
				asm("goto " ___mkstr(APP_START));
			} 
            zlr = 0;
			++cnt;
			vek[reg_5].b[0] = cnt; 
			if(update == true) {
				if(!(cnt % 3)) {
					goto SEND_BLOCK; // wenn Übertragung stehenblebt
				} else if(cnt > 10)
					RESET();
			} else 
            {
//				if(cnt > 10 && !crc) // nach 10 Sek.
//					goto STARTAPP;
			}
		}
    }    
}

//~ void CHK_BUSFREE(void) {
	//~ if (BUSFREE == 0) { 	
        //~ --BUSDELCTR;
		//~ if (BUSDELCTR == 0) 
            //~ BUSFREE = 1;
    //~ }  
//~ }	

//~ void COMRES_RT(void) {
	//~ if (COMRESCTR > 0)
		//~ --COMRESCTR;
    //~ else    
		//~ asm("RESET"); //RSTSRC |= 0x10; //SWRSF=1;

//~ }


