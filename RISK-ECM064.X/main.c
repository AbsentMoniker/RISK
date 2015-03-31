/*
 * File:   main.c
 * Author: jacob
 *
 * Created on February 19, 2015, 6:12 PM
 */

#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h> // __ISR macro here

#include "device_config.h"
#include "init.h"
#include "lcd.h"
#include "gamelogic.h"

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

int main(void)
{
    initClocks();
#if 0
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();
    initRNG();

    startLCD();

    changeState(INIT);
    updateText();
#endif

    TRISE = 0;
    ANSELE = 0;

    unsigned a = 0;

    while(1)
    {
        PORTE = a;
        a += 1;
        for(int i = 0; i < 10*1000*1000; i++)
        {}
    }

    return EXIT_SUCCESS;
}

// TODO: switch these to the correct timer and factor
void msleep(int msecs)
{
    TMR4 = 0;
    T4CONbits.ON = 1;
    while(TMR4 < msecs * 782) // timer counts in increments of 1.28 us
    {}
    T4CONbits.ON = 0;

}
void usleep(int usecs)
{
    TMR4 = 0;
    T4CONbits.ON = 1;
    while(TMR4 < usecs * 782 / 1000) // timer counts in increments of 1.28 us
    {}
    T4CONbits.ON = 0;

}