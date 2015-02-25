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
int randint(int min, int max)
{
    unsigned random = RNGNUMGEN1;
    return min + (random % (max - min + 1)); 
}
//void setTextDisplay(int line, const char * format, ...) {}

// Making two different writes to the same port in quick succession can cause
// problems, so call this macro to make sure both writes get through.
// eg: PORTAbits.RA0 = 1; SHORTWAIT(); PORTAbits.RA1 = 1;
#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void msleep(int msecs);
void SPIRiskTerritory(int terr);
void buttontest();

int inputflag1, inputflag2, inputflag3;
int color = 0;

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

    while(1)
    {
        static int terr = 0;
        //SPIRiskTerritory(1);
        SPIRiskTerritory(terr);
        //terr = (terr + 1) % NUM_TERRITORIES;
        
        if(inputflag1 && PORTDbits.RD13 == 0)
        {
            //color = 1;
            //clearLCD();
            gameInput(CANCEL);
            inputflag1 = 0;
        }
        if(inputflag2 && PORTAbits.RA7 == 0)
        {
            //color = 2;
            //clearLCD();
            gameInput(NEXT);
            inputflag2 = 0;
        }
        if(inputflag3 && PORTFbits.RF8 == 0)
        {
            static int RNGseeded = 0;
            //color = 4;
            //clearLCD();
            if(!RNGseeded)
            {
                seedRNG();
                RNGseeded = 1;
            }
            gameInput(ADVANCE);
            inputflag3 = 0;
        }
        //printLCDline(1, "Hello World!");
        //printLCDline(2, "This is Risk");
        //msleep(1000);
        msleep(100);
    }

    return EXIT_SUCCESS;
}

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

void SPIRiskTerritory(int terr)
{
    static int digits[10] = {
        // gfedcba
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111100, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01100111, // 9
    };

    color = territories[terr].owner + 1;
    if(destination == terr && TMR2 > 390625/2)
        color = 7;
    int ones = territories[terr].troops % 10;
    int tens = (territories[terr].troops / 10) % 100;

    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = digits[ones];
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = (tens? digits[tens] : 0x00);
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = color;

    PORTFbits.RF2 = 1;
    //PORTFbits.RF2 = 1;
    //SHORTWAIT();
    //PORTFbits.RF2 = 0;

}

void buttontest()
{
    //unsigned s1 = PORTDbits.RD13;
    //unsigned s2 = PORTFbits.RF8;
    //PORTAbits.RA1 = s1;
    //SHORTWAIT();
    //PORTAbits.RA0 = s2;
}

// Middle button
void __ISR(_CHANGE_NOTICE_A_VECTOR, IPL7SRS) portAisr()
{
    if(PORTAbits.RA7 == 0)
        inputflag2 = 1;
    IFS3bits.CNAIF = 0;
}

// Left button (not including the stupid button, which is farthest left)
void __ISR(_CHANGE_NOTICE_D_VECTOR, IPL7SRS) portDisr()
{
    if(PORTDbits.RD13 == 0)
        inputflag1 = 1;
    IFS3bits.CNDIF = 0;
}

// Right button
void __ISR(_CHANGE_NOTICE_F_VECTOR, IPL7SRS) portFisr()
{
    if(PORTFbits.RF8 == 0)
        inputflag3 = 1;
    IFS3bits.CNFIF = 0;
}