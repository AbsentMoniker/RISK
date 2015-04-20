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
#define LCD_RCLK  PORTBbits.RB15
#define LCD_DATA  SPI1BUF
#define LCD_VO    PORTCbits.RC13

// Wait functions defined in main.c
void msleep(int msecs);
void usleep(int msecs);

// The on command is 0b00001DCB, with
// D=1 (display on), C=0 (cursor off), B=0 (blinking off)
#define LCDCMD_ON      0b00001100

// Mode is 0b0001DNFxx, with
// D=1 (8-bit mode), N=1 (2 lines), F=0 (default font)
#define LCDCMD_TWOLINE 0b00111000

// Clear also sends cursor to position 0
#define LCDCMD_CLR     0b00000001

#define LCDCMD_LINE0   0x80 // Set cursor to position 0
#define LCDCMD_LINE1   0xC0 // Set cursor to position 0x40
#define LCDCMD_LINE2   0x94 // Set cursor to position 20
#define LCDCMD_LINE3   0xD4 // Set cursor to position 0x40 + 20

// LCD needs at least 37us between most commands, according to its datasheet.
#define LCD_SHORT_WAIT() usleep(40) // wait 40us to be safe

// If the micro was just powered on (meaning the LCD was just powered on
// as well), give it a full second to power up before sending commands.
// Otherwise it needs 2 ms from the ON command to normal operation.
#define LCD_LONG_WAIT()  msleep(2)
#define LCD_POWER_ON_WAIT() msleep(1000)

// Wait a few cycles between toggling the same micro port multiple times.
// This is about 30ns, far shorter than the LCD needs.
#define PORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void startLCD();
void sendLCDcmd(unsigned char cmd);
void sendLCDchar(unsigned char ch);
void clearLCD();

void setTextDisplay(int line, const char * format, ...);

#endif	/* LCD_H */

