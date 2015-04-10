#include <p32xxxx.h>
#include "lcd.h"


void msleep(int msecs);
void usleep(int msecs);

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


// The on command is 0b00001DCB, with
// D=1 (display on), C=0 (cursor off), B=0 (blinking off)
#define LCDCMD_ON      0b00001100
// Mode is 0b0001DNFxx, with
// D=1 (8-bit mode), N=1 (2 lines), F=0 (default font)
#define LCDCMD_TWOLINE 0b00011100
#define LCDCMD_CLR     0b00000001
#define LCDCMD_LINE1   0x80 // Set cursor to position 0
#define LCDCMD_LINE2   0xC0 // Set cursor to position 0x40
#define LCDCMD_LINE3   0x94 // Set cursor to position 20
#define LCDCMD_LINE4   0xD8 // Set cursor to position 0x40 + 20

// LCD needs at least 37us between most commands, according to its datasheet.
#define LCD_SHORT_WAIT() usleep(40) // to be safe
// If the LCD was just powered on (meaning the micro was just powered on
// as well), give it a full second to power up before sending commands.
// Otherwise it needs 2 ms from the ON command to normal operation.
#define LCD_LONG_WAIT()  msleep(2)
#define LCD_POWER_ON_WAIT() msleep(1000)
// Wait a few cycles between toggling the same micro port multiple times.
#define PORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void startLCD()
{
    if(RCONbits.POR)
    {
        LCD_POWER_ON_WAIT();
        RCONbits.POR = 0;
    }
    else
        LCD_SHORT_WAIT();

    PORTWAIT();
    LCD_RW = 0;  // Write to LCD
    PORTWAIT();
    LCD_CLK = 1; // Idle LCD clock high
    PORTWAIT();
    LCD_VO = 0;  // LCD contrast to maximum
    PORTWAIT();
    LCD_RS = 0;  // Writing data
    PORTWAIT();
    LCD_RCLK = 1;  // Writing data

    LCD_LONG_WAIT();
    sendLCDcmd(LCDCMD_TWOLINE);
    sendLCDcmd(LCDCMD_ON);
    sendLCDcmd(LCDCMD_CLR);
    LCD_LONG_WAIT();

}

void clearLCD()
{
    sendLCDcmd(LCDCMD_CLR);
    LCD_SHORT_WAIT();
}
void sendLCDcmd(unsigned char cmd)
{
    PORTWAIT();
    LCD_RS = 0;  // Writing commands
    LCD_SHORT_WAIT();

    for(int i = 0; i < 10; i++)
    {
    LCD_RCLK = 0;
    PORTWAIT();
    LCD_RCLK = 1;
    PORTWAIT();
    }

    
    // SPI out command
    LCD_DATA = cmd;
    while(SPI1STATbits.SPITBE == 0)
    {}
#if 0
    LCD_RCLK = 0;
    PORTWAIT();
    LCD_RCLK = 1;
    PORTWAIT();
#endif

for(int i = 0; i < 10; i++)
    {
    LCD_RCLK = 0;
    PORTWAIT();
    LCD_RCLK = 1;
    PORTWAIT();
    }


    // Pulse LCD clock
    LCD_CLK = 0;
    LCD_SHORT_WAIT();
    LCD_CLK = 1;
    LCD_SHORT_WAIT();
    
    PORTWAIT();
    LCD_RS = 1; // Back to data
    PORTWAIT();
    LCD_SHORT_WAIT();
}

void setTextDisplay(int line, const char * format, ...)
{
    // Convert any args that need formatting
    if(line != 0 && line != 1)
        return;
    char text[21] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(text, 21, format, args); // nprintf will put a '\0' in the 21st char
    va_end(args);

    if(line == 0)
        sendLCDcmd(LCDCMD_LINE1);
    if(line == 1)
        sendLCDcmd(LCDCMD_LINE2);
    for(int i = 0; i < 20; i++)
    {
        // SPI out character
        LCD_DATA = text[i]? text[i] : ' ';
        while(SPI1STATbits.SPITBE == 0)
        {}
        LCD_RCLK = 1;
        PORTWAIT();
        LCD_RCLK = 0;
        PORTWAIT();

        // Pulse LCD clock
        LCD_CLK = 0;
        LCD_SHORT_WAIT();
        LCD_CLK = 1;
        LCD_SHORT_WAIT();
        
    }
}