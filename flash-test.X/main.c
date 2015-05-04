// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.

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


unsigned char dummy[40000];
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

#define MY_FLASH_ADDR ((void *) 0xBD1FC000)

void writeflash()
{

    // Set up Address and Data Registers
    NVMADDR  = (unsigned int)MY_FLASH_ADDR & 0x1FFFFFFF;     // physical address
    NVMDATA0 = 0x12345678;     // value
    // set the operation, assumes WREN = 0
    NVMCONbits.NVMOP = 0x1;   // NVMOP for Word programming
    // Enable Flash for write operation and set the NVMOP
    NVMCONbits.WREN = 1;

    // Start programming
    __builtin_disable_interrupts();
    NVMKEY = 0x0;
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;
    NVMCONSET = _NVMCON_WR_MASK;
    __builtin_enable_interrupts();

    // Wait for WR bit to clear
    while(NVMCON & _NVMCON_WR_MASK)
    {}
    // Disable future Flash Write/Erase operations
    NVMCONbits.WREN = 0;
}

int main(void)
{
    initClocks();
    initInterrupts();
    initPorts();
    initTimers();
    initSPI();
    initRNG();

    volatile unsigned char * mymemory = MY_FLASH_ADDR;
    
    startLCD();
    //setTextDisplay(0, "%p", MY_FLASH_ADDR);
    if(RCONbits.POR)
        setTextDisplay(1, "powered on");
    setTextDisplay(2, "%x", mymemory[0]);
    setTextDisplay(3, "%x", mymemory[0]);

    RCONbits.POR = 0;
    int a = 0;
    while(1)
    {
        if(a == 50)
        {
            setTextDisplay(1, "writing flash");
            writeflash();
        }
        msleep(1000);
        setTextDisplay(0, "%d", a);
        setTextDisplay(3, "%x", mymemory[a % 4]);
        a += 1;
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
