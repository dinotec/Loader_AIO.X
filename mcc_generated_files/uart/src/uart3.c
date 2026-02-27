#include "../uart3.h"
#include "../../../ConstDef.h"

//~ void UART3_TransmitISR (void);
//~ void UART3_ReceiveISR(void);
//~ void (*UART3_TxInterruptHandler)(void);
//~ void (*UART3_RxInterruptHandler)(void);

void UART3_Initialize(void)
{
    PIE9bits.U3RXIE = 0;   
//    UART3_RxInterruptHandler = UART3_ReceiveISR; 
    PIE9bits.U3TXIE = 0; 
//    UART3_TxInterruptHandler = UART3_TransmitISR; 

    // Set the UART3 module to the options selected in the user interface.

    //P1L 0x0; 
    U3P1L = 0x0;
    //P2L 0x0; 
    U3P2L = 0x0;
    //P3L 0x0; 
    U3P3L = 0x0;
    //MODE Asynchronous 8-bit mode; RXEN enabled; TXEN enabled; ABDEN disabled; BRGS high speed; 
    U3CON0 = 0xB0;
    //SENDB disabled; BRKOVR disabled; RXBIMD Set RXBKIF on rising RX input; WUE disabled; ON enabled; 
    U3CON1 = 0x80;
    //FLO off; TXPOL not inverted; STP Transmit 1Stop bit, receiver verifies first Stop bit; RXPOL not inverted; RUNOVF RX input shifter stops all activity; 
    U3CON2 = 0x0;
    //BRGL 207; 
    U3BRGL = 0xCF;
    //BRGH 0; 
    U3BRGH = 0x0;
    //TXBE empty; STPMD in middle of first Stop bit; TXWRE No error; 
    U3FIFO = 0x2E;
    //ABDIE disabled; ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; 
    U3UIR = 0x0;
    //TXCIF equal; RXFOIF not overflowed; RXBKIF No Break detected; FERIF no error; CERIF No Checksum error; ABDOVF Not overflowed; PERIF no parity error; TXMTIF empty; 
    U3ERRIR = 0x80;
    //TXCIE disabled; RXFOIE disabled; RXBKIE disabled; FERIE disabled; CERIE disabled; ABDOVE disabled; PERIE disabled; TXMTIE disabled; 
    U3ERRIE = 0x0;

    PIE9bits.U3RXIE = 1;
}


void UART3_Enable(void)
{
    U3CON1bits.ON = 1; 
}

void UART3_TransmitEnable(void)
{
    U3CON0bits.TXEN = 1;
}

void UART3_ReceiveEnable(void)
{
    U3CON0bits.RXEN = 1;
}

void __interrupt(irq(IRQ_U3RX), base(IVT_BASE)) UART3_Receive_Vector_ISR(void)
{   
    UART3_ReceiveISR();
}

void __interrupt(irq(IRQ_U3TX), base(IVT_BASE)) UART3_Transmit_Vector_ISR(void)
{   
    UART3_TransmitISR();
}
