// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#include <p32xxxx.h>
#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void startLCD()
{
    if(RCONbits.POR)
    {
        LCD_POWER_ON_WAIT();
        //RCONbits.POR = 0;
    }
    else
        LCD_LONG_WAIT();

    LCD_RW = 0;  // Writing to LCD
    PORTWAIT();

    LCD_CLK = 1; // Idle LCD clock high
    PORTWAIT();
#if 0
    LCD_VO = 0;  // LCD contrast to maximum
    PORTWAIT();
#endif

    LCD_RS = 0;  // Writing data
    PORTWAIT();

    LCD_RCLK = 1;  // Idle Rclk high

    LCD_LONG_WAIT();
    sendLCDcmd(LCDCMD_TWOLINE);
    sendLCDcmd(LCDCMD_ON);
    sendLCDcmd(LCDCMD_CLR);
    LCD_LONG_WAIT();

}

void clearLCD()
{
    sendLCDcmd(LCDCMD_CLR);
}
void sendLCDcmd(unsigned char cmd)
{
    LCD_RS = 0;  // Writing commands
    LCD_SHORT_WAIT();
    
    // SPI out command
    LCD_DATA = cmd;
    while(SPI1STATbits.SPIBUSY)
    {}

    LCD_RCLK = 0;
    PORTWAIT();
    LCD_RCLK = 1;
    PORTWAIT();

    // Pulse LCD clock
    LCD_CLK = 0;
    LCD_SHORT_WAIT();
    LCD_CLK = 1;
    LCD_SHORT_WAIT();
    
    PORTWAIT();
    LCD_RS = 1; // Back to data
    LCD_SHORT_WAIT();
}

void sendLCDchar(unsigned char ch)
{

    // SPI out character
    LCD_DATA = ch;
    while(SPI1STATbits.SPIBUSY)
    {}

    LCD_RCLK = 0;
    PORTWAIT();
    LCD_RCLK = 1;
    PORTWAIT();

    // Pulse LCD clock
    LCD_CLK = 0;
    LCD_SHORT_WAIT();
    LCD_CLK = 1;
    LCD_SHORT_WAIT();
}


void setTextDisplay(int line, const char * format, ...)
{
    // Convert any args that need formatting
    if(line != 0 && line != 1 && line != 2 && line != 3)
        return;
    
    char text[21] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(text, 21, format, args); // snprintf will put a '\0' in the 21st char
    va_end(args);

    if(line == 0)
        sendLCDcmd(LCDCMD_LINE0);
    if(line == 1)
        sendLCDcmd(LCDCMD_LINE1);
    if(line == 2)
        sendLCDcmd(LCDCMD_LINE2);
    if(line == 3)
        sendLCDcmd(LCDCMD_LINE3);

    for(int i = 0; i < 20; i++)
    {
        // SPI out character
        LCD_DATA = text[i]? text[i] : ' ';
        while(SPI1STATbits.SPIBUSY)
        {}

        LCD_RCLK = 0;
        PORTWAIT();
        LCD_RCLK = 1;
        PORTWAIT();

        // Pulse LCD clock
        LCD_CLK = 0;
        LCD_SHORT_WAIT();
        LCD_CLK = 1;
        LCD_SHORT_WAIT();     
    }
}
