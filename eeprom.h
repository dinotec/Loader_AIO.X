/* 
 * File:   eeprom.h
 * Author: TsybulinI
 *
 * Created on 7. August 2025, 11:42
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t eep_read(UINT adr, bool);
//~ bool eep_write(UINT adr, uint8_t val, bool b);
void eep_ini(void);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

