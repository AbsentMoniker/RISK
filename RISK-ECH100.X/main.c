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
#include "buttons.h"

// implement function required for game logic in io.h
#ifdef NO_RANDOM
int randint(int min, int max)
{
    return min;
}
#else
int randint(int min, int max)
{
    unsigned random = RNGNUMGEN1;
    return min + (random % (max - min + 1));
}
#endif
void panic(int line, const char * file, const char * fun, const char * text)
{
    abort();
}

// Making two different writes to the same port in quick succession can cause
// problems, so call this macro to make sure both writes get through.
// eg: PORTAbits.RA0 = 1; SHORTWAIT(); PORTAbits.RA1 = 1;
#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void msleep(int msecs);
void usleep(int usecs);
void SPIRiskTerritory(int terr);
void SPIblank();

int inputflag1, inputflag2, inputflag3;

int main(void)
{
    initClocks();
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();
    initRNG();

    startLCD();


    changeState(INIT);
    updateText();

    PORTFbits.RF2 = 0;

    while(1)
    {
        SPIRiskTerritory(0);
        SPIRiskTerritory(1);
        usleep(1000);
        SPIblank();
        SPIblank();

        if(flagSet_advance())
        {
            static int RNGseeded = 0;

            if(!RNGseeded)
            {
                seedRNG();
                RNGseeded = 1;
            }
            gameInput(ADVANCE);

            clearFlag_advance();
        }
        if(flagSet_cancel())
        {
            gameInput(CANCEL);
            clearFlag_cancel();
        }
        if(flagSet_next())
        {
            gameInput(NEXT);
            clearFlag_next();
        }
        msleep(5);

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

void SPIRiskTerritory(int terr)
{
    static int digits[10] = {
        //gfedcba
        0b01111110, // 0
        0b00001100, // 1
        0b10110110, // 2
        0b10011110, // 3
        0b11001100, // 4
        0b11011010, // 5
        0b11111000, // 6
        0b00001110, // 7
        0b11111110, // 8
        0b11001110, // 9
    };

    int color = territories[terr].owner + 1;
    if((source == terr && TMR2 < T8_MSECS(125)) ||
            (destination == terr && TMR2 > T8_MSECS(125)))
    {
        color = 7;
    }
    
    int ones = territories[terr].troops % 10;
    int tens = (territories[terr].troops % 100) / 10;

 
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = digits[ones];
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = (tens? digits[tens] : 0x00);
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = color << 2;

    while(SPI1STATbits.SPITBE != 1) {}



    usleep(1);

    PORTFbits.RF2 = 0;

    usleep(1);

    PORTFbits.RF2 = 1;

}

void SPIblank()
{
    SPI1BUF = 0;
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = 0;
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = 0;
    while(SPI1STATbits.SPITBE != 1) {}

    usleep(1);
    PORTFbits.RF2 = 0;
    usleep(1);
    PORTFbits.RF2 = 1;
}

void __ISR(_TIMER_3_VECTOR, IPL3SRS) timer3isr()
{
    IFS0bits.T3IF = 0; // clear interrupt flag
}

void __ISR(_SPI2_RX_VECTOR, IPL4SRS) SPI2RXisr()
{
    (void)SPI2BUF;

    IFS4bits.SPI2RXIF = 0; // clear interrupt flag
}
