#include <p32xxxx.h>
#include "lcd.h"

#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")
void msleep(int msecs);
void usleep(int msecs);

#include <stdio.h>
#include <stdarg.h>

// LCD needs 37us between commands, according to its datasheet.


#define LCDCMD_ON      0x0C
#define LCDCMD_TWOLINE 0x38
#define LCDCMD_CLR     0x01
#define LCDCMD_LINE1   0x80
#define LCDCMD_LINE2   0xC0

#define LCD_POWER_UP_WAIT_MSEC 500
#define LCD_LONG_WAIT_MSEC 20
#define LCD_SHORT_WAIT_USEC 40


void startLCD()
{
    msleep(LCD_POWER_UP_WAIT_MSEC); // make sure LCD has a chance to power up

    LCD_RW = 0;  // writing
    SHORTWAIT();
    LCD_CLK = 1; // hold clock high
    
    msleep(LCD_LONG_WAIT_MSEC);
    sendLCDcmd(LCDCMD_ON);
    sendLCDcmd(LCDCMD_TWOLINE);
    sendLCDcmd(LCDCMD_CLR);
    usleep(LCD_SHORT_WAIT_USEC);
}

void clearLCD()
{
    sendLCDcmd(0x01);
}
void sendLCDcmd(unsigned char cmd)
{
    LCD_RS = 0;
    usleep(LCD_SHORT_WAIT_USEC);
    LCD_DATA = cmd;
    LCD_CLK = 0;
    usleep(LCD_SHORT_WAIT_USEC);
    LCD_RS = 1;
    LCD_CLK = 1;
    usleep(LCD_SHORT_WAIT_USEC);
}

void setTextDisplay(int line, const char * format, ...)
{
    if(line != 0 && line != 1)
        return;
    char text[17] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(text, 17, format, args);
    va_end(args);

    usleep(LCD_SHORT_WAIT_USEC);
    if(line == 0)
        sendLCDcmd(LCDCMD_LINE1);
    if(line == 1)
        sendLCDcmd(LCDCMD_LINE2);
    usleep(LCD_SHORT_WAIT_USEC);
    for(int i = 0; i < 16; i++)
    {
        LCD_DATA = text[i]? text[i] : ' ';
        LCD_CLK = 0;
        usleep(LCD_SHORT_WAIT_USEC);
        LCD_CLK = 1;
        usleep(LCD_SHORT_WAIT_USEC);
    }
}
