#include <p32xxxx.h>
#include "lcd.h"

#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")
void msleep(int msecs);
void usleep(int msecs);

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// LCD needs 37us between commands, according to its datasheet.


#define LCDCMD_ON      0x0C
#define LCDCMD_TWOLINE 0x38
#define LCDCMD_CLR     0x01
#define LCDCMD_LINE1   0x80
#define LCDCMD_LINE2   0xC0


void startLCD()
{
    msleep(2); // make sure LCD has a chance to power up

    LCD_RW = 0;  // writing
    SHORTWAIT();
    LCD_CLK = 1; // hold clock high
    
    msleep(1);
    sendLCDcmd(LCDCMD_ON);
    sendLCDcmd(LCDCMD_TWOLINE);
    sendLCDcmd(LCDCMD_CLR);
    msleep(2);
}

void clearLCD()
{
    sendLCDcmd(0x01);
}
void sendLCDcmd(unsigned char cmd)
{
    // TODO: make this work using SPI
    printf("LCD not implemented =(\n");
    exit(1);

#if 0
    LCD_RS = 0;
    usleep(40);
    LCD_DATA = cmd;
    LCD_CLK = 0;
    usleep(40);
    LCD_RS = 1;
    LCD_CLK = 1;
    usleep(40);
#endif
}

void setTextDisplay(int line, const char * format, ...)
{
    // TODO: make this work using SPI
    printf("LCD not implemented =(\n");
    exit(1);

#if 0
    if(line != 0 && line != 1)
        return;
    char text[17] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(text, 17, format, args);
    va_end(args);

    usleep(40);
    if(line == 0)
        sendLCDcmd(LCDCMD_LINE1);
    if(line == 1)
        sendLCDcmd(LCDCMD_LINE2);
    for(int i = 0; i < 16; i++)
    {
        LCD_DATA = text[i]? text[i] : ' ';
        LCD_CLK = 0;
        usleep(40);
        LCD_CLK = 1;
        usleep(40);
    }
#endif
}