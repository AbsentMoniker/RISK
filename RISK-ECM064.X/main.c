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
#include "flash.h"

// implement stubs for required game logic in io.h

void panic(int line, const char * file, const char * fun, const char * text)
{
    static int panicking = 0;
    if(panicking)
        abort(); // recursion is bad!
    panicking = 1;

    __builtin_disable_interrupts();
    setTextDisplay(0, "Error in %s()", fun);
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

    extern unsigned char * piDataPtr ;
//    extern unsigned char * piStatsPtr = NULL;

int main(void)
{
    initClocks();
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();
    initRNG();
    
    startLCD();


    // Start a new game
    changeState(INIT);
    updateText();
   
    // Clear input flags so that the initial positions of the buttons and
    // especially the encoder don't get read as inputs.
    IFS0bits.T2IF = 1;
    usleep(1); // make sure the interrupt has time to fire
    clearFlagAdvance();
    clearFlagCancel();
    clearFlagNext();
    clearFlagPrevious();
    clearFlagPowerOn();
    clearFlagPowerOff();

    while(1)
    {
        startNewFrame();
        updateDisplayData();
        updatePiData();
        processStats();
        updatePiStats();
    
        if(flagSetAdvance())
        {
            static int RNGseeded = 0;
            if(!RNGseeded)
            {
                seedRNG();
                shuffleDeck(); // the deck was generated before the RNG got seeded, so do it again.
                RNGseeded = 1;
            }
            gameInput(ADVANCE);;
            clearFlagAdvance();
        }

        if(flagSetCancel())
        {
            gameInput(CANCEL);
            //setTextDisplay(1, "port f3 is %d", PORTFbits.RF3);
            //setTextDisplay(0, "%p", piDataPtr);
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

        if(flagSetPiCommand())
        {
            
            if(piCommand[0] >= 6 && piCommand[0] < 12)
                drawCard(piCommand[0] - 6);
            else if(piCommand[0] < 6 && piCommand[0] == currentPlayer)
            {
                if(cardInput(piCommand[1], piCommand[2], piCommand[3]) > 0)
                    startCardsColor(currentPlayer);
            }
            //setTextDisplay(3, "Pi: %d %d %d %d", piCommand[0], piCommand[1], piCommand[2], piCommand[3]);
            clearFlagPiCommand();
        }

        // The power switch doesn't actually work :(
        if(flagSetPowerOn())
        {
            //setTextDisplay(0, "power switch on");
            //enableDisplays();
            clearFlagPowerOn();
        }
        if(flagSetPowerOff())
        {
            //setTextDisplay(0, "power switch off");
            // wait until displays are off, then save game
             //disableDisplays();
            // msleep(10);
            // saveGame();
            // if(!flashIserased())
            //      eraseFlash();
            // writeSaveToFlash();
            clearFlagPowerOff();
        }
        
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

// SPIs one byte out to the LEDs / 7-segs
void SPIbyte(unsigned char byte)
{
    while(SPI4STATbits.SPITBE != 1) {}
    SPI4BUF = byte;
    while(SPI4STATbits.SPITBE != 1) {}

}
