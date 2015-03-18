#include <p32xxxx.h>
#include "init.h"

static inline void systemUnlock()
{
    __builtin_disable_interrupts();
    // The datasheet emphasizes that these writes MUST be consecutive
    // instructions, which is why we disable interrupts
    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

static inline void systemLock()
{
    SYSKEY = 0x0;
   __builtin_enable_interrupts();
}

void initClocks()
{
    systemUnlock();

    OSCCONbits.FRCDIV = 0;
    PB1DIV = 0x8001;
    PB2DIV = 0x8001;
    PB3DIV = 0x8000;
    PB4DIV = 0x8001;
    PB5DIV = 0x8001;
    PB7DIV = 0x8000;
    PB8DIV = 0x8001;

    REFO1CONCLR = 0b1001000000000000;
    REFO2CONCLR = 0b1001000000000000;
    REFO3CONCLR = 0b1001000000000000;
    REFO4CONCLR = 0b1001000000000000;

    systemLock();
}

void initInterrupts()
{
    __builtin_disable_interrupts();

    // Shadow register sets for each interrupt level
    // why this is configurable is beyond me
    PRISS = 0x76543210;

    // put some interrupts here

    INTCONbits.MVEC = 1;

    __builtin_enable_interrupts();
}

void initPorts()
{
    // 1 = input, 0 = output
    //
    //        111111
    //        5432109876543210     pins available
    TRISB = 0b1111111111111111; // 0 - 15
    TRISC = 0b1111111111111111; // 12 - 15
    TRISD = 0b1111111111111111; // 0 - 5, 9 - 11
    TRISE = 0b1111111111111111; // 0 - 7
    TRISF = 0b1111111111111111; // 0 - 1, 3 - 5
    TRISG = 0b1111111111111111; // 6 - 9

    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;


}

void initTimers()
{
    // TODO: rest of the timers

    // Let's configure a timer!
    T4CON = 0;
    T5CON = 0;
    T4CONbits.TCS = 0;       // internal oscillator
    T4CONbits.T32 = 1;       // 32-bit timer
    T4CONbits.TCKPS = 0b111; // 1:256 pre-scale
    PR4 = 0xFFFFFFFF;        // maximum period

    T2CON = 0;
    T3CON = 0;
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 1;

    // TODO: use timer 1 for the RNG, its not doing anything useful

    // timer 2 will be temporarily hijacked to seed the RNG
    // until the first button press
    //T2CONbits.TCKPS = 0b111;
    //PR2 = 390625; 
    //T2CONbits.ON = 1;

}

void initSPI()
{
    // TODO: 3 spis
#if 0
    // Let's configure an SPI!
    SPI1CON = 0;           // SPI off
    SPI1BRG = 3;         // baud rate
    SPI1STATCLR = 0x40;    // clear status
    RPF1R = 0b0101;        // SPI data out on F1
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.ON = 1;    // SPI on
#endif

}

void initRNG()
{
    RNGPOLY1 = 0x00C00003;
    RNGPOLY2 = 0x00000000;
    RNGCONbits.PLEN = 42;

    // Set up timer to count really fast so we can use the value as a seed
    // for the RNG
    T2CONbits.TCKPS = 0b000;
    PR2 = 0xFFFFFFFF;
    T2CONbits.ON = 1;
}

void seedRNG()
{
    RNGNUMGEN1 = RNGNUMGEN2 = TMR2;
    RNGCONbits.PRNGEN = 1;

    // Put the timer back to the settings it needs to make LEDs blinky
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 0b111;
    TMR2 = 0;
    PR2 = 390625;
    T2CONbits.ON = 1;
    
}