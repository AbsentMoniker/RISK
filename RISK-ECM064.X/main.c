/*
 * File:   main.c
 * Author: jacob
 *
 * Created on February 19, 2015, 6:12 PM
 */

// ***********************************
// ******                       ******
// ******        ECM-064        ******
// ******                       ******
// ***********************************


#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h> // __ISR macro here

#include "device_config.h"
#include "init.h"
#include "lcd.h"
#include "gamelogic.h"
#include "displays.h"
#include "buttons.h"
#include "pi.h"

// implement stubs for required game logic in io.h
#ifdef USE_RANDOM
int randint(int min, int max)
{
    unsigned random = RNGNUMGEN1;
    return min + (random % (max - min + 1));
}
#else
int randint(int min, int max)
{
    return min;
}
#endif
void panic(int line, const char * file, const char * fun, const char * text)
{
    static int panicking = 0;
    if(panicking)
        abort(); // recursion is bad!
    panicking = 1;

    __builtin_disable_interrupts();
    setTextDisplay(0, "Error %s", fun);
    setTextDisplay(1, "at %s:%d", file, line);
    setTextDisplay(2, "%s", text);
    if(strlen(text) > 16)
        setTextDisplay(3, "%s", text + 16);
    else
        setTextDisplay(3, "");
    while(1)
    {
        // Go into an infinte loop until the micro is reset.
    }
    abort();
}

// Making two different writes to the same port in quick succession can cause
// problems, so call this macro to make sure both writes get through.
// eg: PORTAbits.RA0 = 1; SHORTWAIT(); PORTAbits.RA1 = 1;
#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void msleep(int msecs);
void usleep(int usecs);
void SPIbyte(unsigned char byte);

int main(void)
{
    initClocks();
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();
    initRNG();

#if 0
#define LCD_RS    PORTDbits.RD4
#define LCD_RW    PORTDbits.RD3
#define LCD_CLK   PORTDbits.RD5
#define LCD_RCLK  PORTBbits.RB15
#define LCD_DATA  SPI1BUF
#define LCD_VO    PORTCbits.RC13
#endif

   //startLCD();
#if 0
    changeState(INIT);
    updateText();
#endif



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

#if 1
    PORTWAIT();
    LCD_RW = 0;  // Write to LCD
    PORTWAIT();
    LCD_CLK = 1; // Idle LCD clock high
    PORTWAIT();
    LCD_VO = 0;  // LCD contrast to maximum
    PORTWAIT();
    LCD_RS = 1;  // Writing data
    continentOwners[5] = 4;
    updateDisplayData();
    while (0)
    {
        LCD_RCLK = 1;
        LCD_SHORT_WAIT();
        LCD_RCLK = 0;
        LCD_SHORT_WAIT();
    }
    continentOwners[0] = 1;
    updateDisplayData();
    msleep(1000);
    //LCD_RCLK = 1;
    sendLCDcmd(LCDCMD_TWOLINE);
    LCD_SHORT_WAIT();
    sendLCDcmd(LCDCMD_ON);
    LCD_SHORT_WAIT();
    sendLCDcmd(LCDCMD_CLR);

    //startLCD();
    //setTextDisplay(0, "hello world");
#endif

    updateDisplayData();

    IFS0bits.T2IF = 1;
    clearFlagAdvance();
    clearFlagCancel();
    clearFlagNext();
    clearFlagPrevious();

    updatePiData();
    for(int i = 0; i < 42; i++)
    {
        piData[i] = i % 6;
    }
    for(int i = 0; i < 44; i++)
    {
        if(i < 12)
            piData[i + 42] = i % 6;
        else
            piData[i + 42] = 0xFF;
    }

    continentOwners[0] = -1;
        continentOwners[1] = -1;
        continentOwners[2] = -1;
        continentOwners[3] = -1;
        continentOwners[4] = -1;
        continentOwners[5] = -1;

    int a = 0;
    while(1)
    {
        
        updateDisplayData();

#if 0
        PORTDbits.RD1 = a;
        PORTC = 0xFFFF;
        PORTB = 0xFFFF;
        if(a == 0)
            a = 1;
        else
            a = 0;
        msleep(1000);
#endif
#if 0
        updateDisplayData();
        if(flagSetAdvance())
        {
            continentOwners[0] = (continentOwners[0] == 0? -1 : 0);
            clearFlagAdvance();
        }

        if(flagSetCancel())
        {
            continentOwners[1] = (continentOwners[1] == 0? -1 : 0);
            clearFlagCancel();
        }

        if(flagSetNext())
        {
            attackerDice[0] += 1;
            if(attackerDice[0] >= 10)
                attackerDice[0] = 1;
            clearFlagNext();
        }

        if(flagSetPrevious())
        {
            attackerDice[0] -= 1;
            if(attackerDice[0] <= 0)
                attackerDice[0] = 9;
            clearFlagPrevious();
        }
#endif

    }

    return EXIT_SUCCESS;
}


void msleep(int msecs)
{
    // The timer has a maximum value of 0xFFFF * 0.64us ~= 41 ms
    while(msecs > 40)
    {
        usleep(40000);
        msecs -= 40;
    }

    usleep(msecs * 1000);
}
void usleep(int usecs)
{
    TMR1 = 0;
    T1CONbits.ON = 1;
    while(TMR1 < usecs * 100 / 64) // timer counts in increments of 0.64 us
    {}
    T1CONbits.ON = 0;
}

void SPIbyte(unsigned char byte)
{
    while(SPI4STATbits.SPITBE != 1) {}
    SPI4BUF = byte;
    while(SPI4STATbits.SPITBE != 1) {}

}