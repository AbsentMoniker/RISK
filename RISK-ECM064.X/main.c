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

//   startLCD();
#if 0
    changeState(INIT);
    updateText();
#endif

    cardExchangeValue = 51;

    attackerDice[0] = 0;
    attackerDice[1] = 0;
    attackerDice[2] = 0;
    defenderDice[0] = 0;
    defenderDice[1] = 0;
    

    continentOwners[0] = -1;
    continentOwners[1] = -1;
    continentOwners[2] = -1;
    continentOwners[3] = -1;
    continentOwners[4] = -1;
    continentOwners[5] = -1;

    updateDisplayData();

    IFS0bits.T2IF = 1;
    clearFlagAdvance();
    clearFlagCancel();
    clearFlagNext();
    clearFlagPrevious();

    while(1)
    {
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