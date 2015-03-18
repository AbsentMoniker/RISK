/* 
 * File:   main.c
 * Author: jacob
 *
 * Created on February 19, 2015, 6:12 PM
 */

#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>
#include "device_config.h" // contains firmware configuration

#include <stdlib.h>

#include "init.h"
#include "/home/jacob/Documents/477/RISK/micro/game.h"
#include "/home/jacob/Documents/477/RISK/micro/io.h"
int randint(int min, int max) { return min; }
void setTextDisplay(int line, const char * format, ...) {}

// Making two different writes to the same port in quick succession can cause
// problems, so call this macro to make sure both writes get through.
// eg: PORTAbits.RA0 = 1; SHORTWAIT(); PORTAbits.RA1 = 1;
#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")

void msleep(int msecs);

int iflag = 0;

int main(void)
{
    int temp;
    initClocks();
    initPorts();
    initInterrupts();
    initTimers();

    
    TMR4 = 0;
    T4CONbits.ON = 1;

    while(1)
    {
        if(iflag)
        {
            gameInput(ADVANCE);
            iflag = 0;
        }

    }

    return EXIT_SUCCESS;
}

void __ISR(_CHANGE_NOTICE_D_VECTOR, IPL7SOFT) portDisr()
{
    if(PORTDbits.RD13)  // If bit changed to 1
        iflag = 1;      // set flag for main function to see.
    IFS3bits.CNDIF = 0; // Clear interrupt flag.
}