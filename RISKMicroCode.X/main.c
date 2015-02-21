/* 
 * File:   main.c
 * Author: jacob
 *
 * Created on February 19, 2015, 6:12 PM
 */

#include <p32xxxx.h>
#include "device_config.h"

#include <stdlib.h>

#include "init.h"


#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void msleep(int msecs);
void SPItest();
void buttontest();

int main(void)
{
    initClocks();
    initPorts();
    initTimers();
    initSPI();

    while(1)
    {
        SPItest();
        buttontest();
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

void SPItest()
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
    static int counter = 0;

    int ones = counter % 10;
    int tens = counter / 10;

    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = digits[ones];
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = (tens? digits[tens] : 0x00);
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = counter % 8;
    msleep(250);

    counter += 1;
    if(counter >= 100)
        counter = 0;

}

void buttontest()
{
    unsigned s1 = PORTDbits.RD13;
    unsigned s2 = PORTFbits.RF8;
    PORTAbits.RA1 = s1;
    SHORTWAIT();
    PORTAbits.RA0 = s2;

}