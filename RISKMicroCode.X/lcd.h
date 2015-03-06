/* 
 * File:   lcd.h
 * Author: jacob
 *
 * Created on February 24, 2015, 1:00 PM
 */

#include <p32xxxx.h>

#ifndef LCD_H
#define	LCD_H

#define LCD_RS    PORTBbits.RB15
#define LCD_RW    PORTCbits.RC4
#define LCD_CLK   PORTCbits.RC3
#define LCD_DATA  PORTE

void startLCD();
void sendLCDcmd(unsigned char cmd);
void clearLCD();

#endif	/* LCD_H */

