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

    // Timer 4/5 interrupts
    //IFS0bits.T5IF = 0;
    //IEC0bits.T5IE = 1;
    //IPC6bits.T5IP = 7;
    //IPC6bits.T5IS = 0;

    // Port A, D, F change interrupts enabled with priority 7.0
    IFS3bits.CNAIF = 0;
    IEC3bits.CNAIE = 1;
    IPC29bits.CNAIP = 7;
    IPC29bits.CNAIS = 0;

    IFS3bits.CNDIF = 0;
    IEC3bits.CNDIE = 1;
    IPC30bits.CNDIP = 7;
    IPC30bits.CNDIS = 0;

    IFS3bits.CNFIF = 0;
    IEC3bits.CNFIE = 1;
    IPC30bits.CNFIP = 7;
    IPC30bits.CNFIS = 0;


    // enable change interrupt on A7, D13, F8
    CNCONAbits.ON = 1;
    CNENAbits.CNIEA7 = 1;
    CNPUAbits.CNPUA7 = 1;
    CNCONDbits.ON = 1;
    CNENDbits.CNIED13 = 1;
    CNPUDbits.CNPUD13 = 1;
    CNCONFbits.ON = 1;
    CNENFbits.CNIEF8 = 1;
    CNPUFbits.CNPUF8 = 1;

    INTCONbits.MVEC = 1;

    __builtin_enable_interrupts();
}

void initPorts()
{
    // On the Explorer 16 board, the LEDs are (from left to right):
    // A7  A6  F12 F13 A3  A2  A1  A0
    // Buttons are:
    // N/C D13 A7  F8                   (yes A7 again)
    // LCD uses:
    // B15 C3  C4  E0..E7
    //
    // F1 and D1 are used for SPI1
    // G is used for ??? but it got set that way in harmony
    //
    //        111111
    //        5432109876543210
    TRISA = 0b1111111111111111;
    TRISB = 0b0111111111111111;
    TRISC = 0b1111111111100111;
    TRISD = 0b1111111111111101;
    TRISE = 0b1111111100000000;
    TRISF = 0b1111111111111001;
    TRISG = 0b1111001111000011;

    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;

    // Clear LEDs at start of program
    //PORTACLR = 0b11001111;
    //PORTFCLR = 0b0011000000000000;

}

void initTimers()
{
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

    // timer 2 will be temporarily hijacked to seed the RNG
    // until the first button press
    //T2CONbits.TCKPS = 0b111;
    //PR2 = 390625; 
    //T2CONbits.ON = 1;

}

void initSPI()
{
    // Let's configure an SPI!
    SPI1CON = 0;           // SPI off
    SPI1BRG = 0x1;         // baud rate
    SPI1STATCLR = 0x40;    // clear status
    RPF1R = 0b0101;        // SPI data out on F1
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.ON = 1;    // SPI on

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