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
    if(RCONbits.POR)
    {
        msleep(1000);
        RCONbits.POR = 0;
    }
    else
        msleep(2);

    LCD_RW = 0;
    SHORTWAIT();
    LCD_CLK = 1;
    SHORTWAIT();
    LCD_VO = 0;
    msleep(1);
    sendLCDcmd(LCDCMD_ON);
    sendLCDcmd(LCDCMD_TWOLINE);
    sendLCDcmd(LCDCMD_CLR);
    msleep(2);

}

void clearLCD()
{
    sendLCDcmd(LCDCMD_CLR);
    usleep(40);
}
void sendLCDcmd(unsigned char cmd)
{
    SHORTWAIT();
    LCD_RS = 0;
    usleep(40);
    LCD_DATA = cmd;
    while(SPI1STATbits.SPITBE == 0)
    {}
    LCD_RCLK = 1;
    SHORTWAIT();
    LCD_RCLK = 0;
    SHORTWAIT();
    LCD_CLK = 0;
    usleep(40);
    LCD_RS = 1;
    SHORTWAIT();
    LCD_CLK = 1;
    usleep(40);
}

void setTextDisplay(int line, const char * format, ...)
{

    if(line != 0 && line != 1)
        return;
    char text[21] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(text, 21, format, args);
    va_end(args);

    usleep(40);
    if(line == 0)
        sendLCDcmd(LCDCMD_LINE1);
    if(line == 1)
        sendLCDcmd(LCDCMD_LINE2);
    for(int i = 0; i < 20; i++)
    {
        LCD_DATA = text[i]? text[i] : ' ';
        while(SPI1STATbits.SPITBE == 0)
        {}
        LCD_RCLK = 1;
        SHORTWAIT();
        LCD_RCLK = 0;
        SHORTWAIT();
        LCD_CLK = 0;
        usleep(40);
        LCD_CLK = 1;
        usleep(40);
        
    }
}