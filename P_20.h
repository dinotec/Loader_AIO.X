#ifndef P_20_H
#define	P_20_H
void send_status(void);
extern EMPF_BUF buf0;
extern UINT vek[];
extern bool status_req;
extern bool update;
void P20_ini(void);
void ident_daten(char* ptr);
bool CHKWC(uint8_t val, uint8_t cmp);
void bus_rt(void);


#endif	/* P_20_H */

