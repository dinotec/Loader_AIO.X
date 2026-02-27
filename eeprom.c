#include "ConstDef.h"
#include "P_20.h"
#include "eeprom.h"
#include "enumerate.h"

extern char id[];

uint8_t eep_read(UINT adr, bool b) {
    adr.ui *= 2;
    if(b)
        ++adr.ui;
    // Load NVMADR with the desired byte address
    NVMADRU = 0x38;
    NVMADRH = adr.b[1];  
    NVMADRL = adr.b[0];        
    NVMCON1bits.NVMCMD = 0; // Read
    NVMCON0bits.GO = 1; // Start byte read
    while (NVMCON0bits.GO) ; // Wait for the read operation to complete
    return NVMDATL;
}
#if 0
bool eep_write(UINT adr, uint8_t val, bool b) {
    bool ret = false;
    uint8_t GIEBitValue = INTCON0bits.GIE;
    // Load NVMADR with the target address of the byte
    adr.ui *= 2;
    if(b)
        ++adr.ui;
    NVMCON1bits.WRERR = 0;
    
    NVMADRU = 0x38;
    NVMADRH = adr.b[1];  
    NVMADRL = adr.b[0];        
    NVM_UnlockKeySet(UNLOCK_KEY);
    NVMDATL = val; // Load NVMDAT with the desired value
    NVMCON1bits.CMD = 0x03; // Write 
    INTCON0bits.GIE = 0; // Disable interrupts
#if 0
    NVMLOCK = 0x55;
    //NVMLOCK = 0xAA;
    asm("MOVF (_unlockKeyHigh&0xFF),w,c"); //load 'unlockKeyHigh' into the W register
    asm("MOVWF (_NVMLOCK&0xFF),b"); //move the W register to NVMLOCK
    NVMCON0bits.GO = 1; // Start byte write
#endif 
    //Perform the unlock sequence 
    asm("asmopt push"); //save the current selection of optimizations
    asm("asmopt off"); //turn off assembler optimizations
    asm("banksel(_NVMLOCK)"); //select the bank of the NVMLOCK register
    NVM_UnlockKeySet(UNLOCK_KEY);
    NVMLOCK = 0x55; //assign 'unlockKeyLow' to NVMLOCK.
    asm("MOVF (_unlockKeyHigh&0xFF),w,c"); //load 'unlockKeyHigh' into the W register
    asm("MOVWF (_NVMLOCK&0xFF),b"); //move the W register to NVMLOCK
    //Start byte write
    asm("bsf (_NVMCON0bits&0xFF)," ___mkstr(_NVMCON0_GO_POSN) ",b"); //Set GO bit   
    asm("asmopt pop"); //restore assembler optimization settings

    while (NVMCON0bits.GO); // Wait for the write operation to complete
    // Verify byte write operation success and call the recovery function if needed
    NVMLOCK = 0;
    if (NVMCON1bits.WRERR){
        ret = true; //WRITE_FAULT_RECOVERY();
    }
    NVMCON1bits.CMD = 0; // Disable writes to memory
    INTCON0bits.GIE = GIEBitValue; // Restore interrupt enable bit value (if
    return ret;
}
#endif

void eep_ini() {
	UINT tmp;
    tmp.ui = l_adr2;
    vek[l_adr2].ui =  eep_read(tmp, 0);
    tmp.ui = l_adr1;
	vek[l_adr1].ui =  eep_read(tmp, 0);
	tmp.ui = l_adr0;
    vek[l_adr0].ui =  eep_read(tmp, 0);
    tmp.ui = sh_adr;
    vek[sh_adr].ui = eep_read(tmp, 0);
    if(vek[sh_adr].b[0] == 0xff)
		--vek[sh_adr].ui;
    tmp.ui = hardware;
	vek[hardware].b[0] = eep_read(tmp, 0);
	vek[hardware].b[1] = eep_read(tmp, 1);
	if(vek[hardware].ui == 0xffff)
		vek[hardware].ui = 2601;
    vek[sofver].i = atoi(id + 21);
   
    for(uint8_t ix = 0; ix < 16; ++ix){
        tmp.ui = 192 + ix;
        vek[tmp.ui].b[0] = eep_read(tmp, 0);
        if(vek[tmp.ui].b[0] == 0xff)
			vek[tmp.ui].b[0] = eep_read(tmp, 0);
    }
   
    //~ for(uint8_t ix = 192; ix < 208; ++ix){
        //~ tmp.ui = 192 + ix;
			//~ vek[tmp.ui].b[0] = eep_read(tmp, 0);
    //~ }
}
