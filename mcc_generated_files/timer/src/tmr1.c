/**
  * TMR1 Generated Timer Driver File
  *
  * @file tmr1.c
  *
  * @ingroup timerdriver
  *
  * @brief Driver implementation for the TMR1 Timer driver
  *
  * @version TMR1 Timer Driver Version 4.0.0
  * 
  * @version Package Version 5.1.0
  */
/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

/**
 * Section: Included Files
*/

#include <xc.h>
#include "../tmr1.h"
#include "../../../ConstDef.h"
#include "../../../enumerate.h"
#include "../../../main.h"

extern UINT vek[];
extern bool tmr1_flag;

static volatile uint16_t timer1ReloadVal;
static void TMR1_DefaultOverflowCallback(void);
static void (*TMR1_OverflowCallback)(void);

void TMR1_Initialize(void)
{
    T1CONbits.TMR1ON = 0;             // TMRON disabled

    T1GCON = (0 << _T1GCON_T1GGO_POSN)   // T1GGO done
        | (0 << _T1GCON_T1GSPM_POSN)   // T1GSPM disabled
        | (0 << _T1GCON_T1GTM_POSN)   // T1GTM disabled
        | (0 << _T1GCON_T1GPOL_POSN)   // T1GPOL low
        | (0 << _T1GCON_T1GE_POSN);  // T1GE disabled

    T1GATE = (0 << _T1GATE_GSS_POSN);  // GSS T1G_pin

    T1CLK = (1 << _T1CLK_CS_POSN);  // CS FOSC/4

    TMR1H = 0xF8;              // Period 1.0005ms; Timer clock 2000000 Hz
    TMR1L = 0x2F;

    timer1ReloadVal=((uint16_t)TMR1H << 8) | TMR1L;

    PIR3bits.TMR1IF = 0;   
    PIE3bits.TMR1IE = 1;
  
    T1CON = (1 << _T1CON_TMR1ON_POSN)   // TMR1ON enabled
        | (0 << _T1CON_T1RD16_POSN)   // T1RD16 disabled
        | (1 << _T1CON_nT1SYNC_POSN)   // nT1SYNC do_not_synchronize
        | (0 << _T1CON_CKPS_POSN);  // CKPS 1:1
}

void TMR1_Start(void)
{   
    T1CONbits.TMR1ON = 1;
}

void TMR1_Stop(void)
{ 
    T1CONbits.TMR1ON = 0U;
}

void __interrupt(irq(TMR1),base(IVT_BASE)) TMR1_OverflowISR()
{
    PIR3bits.TMR1IF = 0;
    tmr1_flag = true;
}
