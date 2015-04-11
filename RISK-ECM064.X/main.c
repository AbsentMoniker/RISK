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
    
    startLCD();

#if 1
    changeState(INIT);
    updateText();
#endif
   
    // Clear input flags so that the initial positions of the buttons
    // don't get read as inputs.
    IFS0bits.T2IF = 1;
    clearFlagAdvance();
    clearFlagCancel();
    clearFlagNext();
    clearFlagPrevious();

#if 0
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
#endif

    //int a = 0;
    while(1)
    {
#if 1
        updateDisplayData();
        
        if(flagSetAdvance())
        {
            static int RNGseeded = 0;
            if(!RNGseeded)
            {
                seedRNG();
                RNGseeded = 1;
            }
            gameInput(ADVANCE);
            clearFlagAdvance();
        }

        if(flagSetCancel())
        {
            gameInput(CANCEL);
            clearFlagCancel();
        }

        if(flagSetNext())
        {
            gameInput(NEXT);
            clearFlagNext();
        }

        if(flagSetPrevious())
        {
            gameInput(PREVIOUS);
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