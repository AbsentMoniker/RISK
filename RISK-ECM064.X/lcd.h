/* 
 * File:   lcd.h
 * Author: jacob
 *
 * Created on February 24, 2015, 1:00 PM
 */

#include <p32xxxx.h>

#ifndef LCD_H
#define	LCD_H

#define LCD_RS    PORTDbits.RD4
#define LCD_RW    PORTDbits.RD3
#define LCD_CLK   PORTDbits.RD5
#define LCD_RCLK  PORTCbits.RC14
#define LCD_DATA  SPI1BUF
#define LCD_VO    PORTCbits.RC13
//#define LCD_DATA  PORTE // this is not so simple, it needs to go over SPI

void startLCD();
void sendLCDcmd(unsigned char cmd);
void clearLCD();

#endif	/* LCD_H */

