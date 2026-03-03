#include "ConstDef.h"
#include "P_20.h"
#include "main.h"


extern bool rx_ready;
extern bool upd_ready;

bool BUSFREE = 0;	

// High-priority ISR (only for UART1 RX)
void UART3_ReceiveISR(void) {
    uint8_t c;
    c = U3RXB;  // Read data (clears RC1IF automatically)
    if(update == true) {
		buf0.buf[buf0.cnt] = c;
		if(++buf0.cnt == 0)
			upd_ready = true;
	} 
	else if(c > 0 && c != 0x1f) {
        buf0.buf[buf0.cnt] = c; 
        if(buf0.buf[buf0.cnt] == 0x0d)
            rx_ready = true;
        else {	
			if(++buf0.cnt >= sizeof(buf0.buf)) 
            	memset(&buf0,0,sizeof(buf0));
		}
    }
//    BUSFREE = 0;
//    BUSDELCTR = BUSDEL;
}

void UART3_Init(void) {
    U3CTSPPS = 0x2c;  // b 101 100 RF4
    //--------------------------0
    U3CON1bits.ON = 1; 
    U3CON0bits.TXEN = 1;
    U3CON0bits.RXEN = 1;
//    PIE4bits.U1EIE = 1;

    ANSELAbits.ANSELA0 = 0; // RX
    ANSELAbits.ANSELA1 = 0; // TX
    ANSELAbits.ANSELA2 = 0; // TXDE
    ODCONAbits.ODCA0   = 1;  // open drain  
    ODCONAbits.ODCA2   = 0;  // open drain  
    PIE9bits.U3TXIE    = 0; // UART3 TX Interrupt aus
    IPR9bits.U3TXIP    = 0; // Low Priority
    IPR9bits.U3RXIP    = 1; // Low Priority
    U3CON2bits.FLO     = 2; 
    //SEND_TX(0);
}

void UART3_TransmitISR(void) {
    if (buf0.buf[buf0.cnt]) {
        U3TXB = buf0.buf[buf0.cnt];
        if (buf0.buf[++buf0.cnt] == 0) {
            ENDE:
            U3TXIE = 0; // Disable TX interrupt
            memset(&buf0, 0, sizeof(buf0));
        }
    } else
		goto ENDE;
//    BUSFREE = 0;
//    BUSDELCTR |= BUSDEL;
}

