#include "ConstDef.h"
//~ -Wl,-PmyID=2008h
//~ -Wl,-Psmallconst=0x2140
//~  hexmate r2000-8fff,aim4.hex loader2.hex -oaim4_loader.hex

const char __section("myID") id[29] = "06024Bootloader AIO  2601LLL"; // 

__at(0x3700) uint8_t flashBuffer[256];

//Beispiel: -Wl,-Pcinit=0x2000 (platziert die cinit-Sektion ab Adresse 0x2000).

