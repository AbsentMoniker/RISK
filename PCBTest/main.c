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

// Making two different writes to the same port in quick succession can cause
// problems, so call this macro to make sure both writes get through.
// eg: PORTAbits.RA0 = 1; SHORTWAIT(); PORTAbits.RA1 = 1;
#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop":::"memory")

void msleep(int msecs);
void usleep(int usecs);
void SPIByte(unsigned char byte);

int main(void)
{
    initClocks();
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();

    startLCD();

    setTextDisplay(0, "Hello World!");
    setTextDisplay(1, "Line 1");
    setTextDisplay(2, "Line 2");
    setTextDisplay(3, "Line 3");

    PORTFbits.RF2 = 0;

    while(1)
    {
        static const int digits[10] = {
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
        static int x = 0;

        int ones = digits[x % 10];
        int tens = digits[(x / 10) % 10];

        SPIByte(ones);
        SPIByte(tens);
        SPIByte((x % 8) << 2);
        usleep(1);
        PORTFbits.RF2 = 1;
        SHORTWAIT();
        PORTFbits.RF2 = 0;
        msleep(500);

        x += 1;

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

void SPIByte(unsigned char byte)
{
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = byte;
    while(SPI1STATbits.SPITBE != 1) {}


}