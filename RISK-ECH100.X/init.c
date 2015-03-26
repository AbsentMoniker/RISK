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

// PB clocks are maximum 100 MHz, except PB7 at 200 MHz
void initClocks()
{
    systemUnlock();

    OSCCONbits.FRCDIV = 0;
    PB1DIV = 0x8001; // system usage
    PB2DIV = 0x8001; // SPI
    PB3DIV = 0x8001; // timers
    PB4DIV = 0x8001; // I/O port access
    PB5DIV = 0x8001; // RNG
    PB7DIV = 0x8000; // CPU clock
    PB8DIV = 0x8001; // not used in this project

    REFO1CON = 0;
    REFO2CON = 0;
    REFO3CON = 0;
    REFO4CON = 0;

    systemLock();
}

void initInterrupts()
{
    __builtin_disable_interrupts();

    // Shadow register sets for each interrupt level
    // why this is configurable is beyond me
    PRISS = 0x76543210;

    // Timer 2 interrupts
    // Interrupt every 2 ms, priority 2.0
    // ISR in buttons.c
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 2;
    IPC2bits.T2IS = 0;

    // Timer 3 interrupts
    // Interrupt every 1 ms, priority 3.0
    // ISR in main.c
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;

    // SPI 3 rx interrupt
    // priority 4.0
    // ISR in pi.c
    IFS4bits.SPI3RXIF = 0;
    IEC4bits.SPI3RXIE = 1;
    IPC38bits.SPI3RXIP = 4;
    IPC38bits.SPI3RXIS = 0;

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
    // ----- TIMER 1 -----
    // Used to provide short, precise waits for the usleep and msleep functions
    // which are used mostly in the LCD driver.
    // Will be started when a wait is called for.
    T1CON = 0;
    T1CONbits.TCKPS = 0b10; // 1:64 prescale = 0.64us increments
                            // 0.64us * 0xFFFF = 41.9ms maximum
    PR1 = 0xFFFF; // maximum period
    TMR1 = 0;

    // ----- TIMER 2 -----
    // Generates interrupts to poll button state.
    T2CON = 0;
    T2CONbits.TCKPS = 0b010; // 1:4 prescale = 0.04us increments
    PR2 = 50000; // 0.04us * 50 000 = 2 ms
    TMR2 = 0;
    T2CONbits.ON = 1;

    // ----- TIMER 3 -----
    // Generates interrupts to start LED SPI shifting.
    T3CON = 0;
    T3CONbits.TCKPS = 0b010; // 1:4 prescale = 0.04us increments
    PR3 = 25000; // 0.04us * 25 000 = 1 ms
    TMR3 = 0;
    T3CONbits.ON = 1;

    // ----- TIMER 4 -----
    // Not used
    T4CON = 0;

    // ----- TIMER 5 -----
    // Not used
    T5CON = 0;

    // ----- TIMER 6/7 -----
    // Used to provide a random value to seed the RNG.
    T6CON = 0;
    T7CON = 0;
    T6CONbits.T32 = 1;       // 32-bit timer
    T6CONbits.TCKPS = 0b000; // no prescale
    PR6 = 0xFFFFFFFF;        // maximum period
    TMR6 = 0;
    T6CONbits.ON = 1;

    // ----- TIMER 8/9 -----
    // Used to flash LEDs of selected territories.
    T8CON = 0;
    T9CON = 0;
    T8CONbits.T32 = 1;       // 32-bit timer
    T8CONbits.TCKPS = 0b110; // 1:64 prescale
    PR8 = 390625;            // 0.64us * 390625 = 250 ms
    TMR8 = 0;
    T8CONbits.ON = 1;
}

void initSPI()
{
    // Let's configure an SPI!
    SPI1CON = 0;
    SPI1CON2 = 0;
    SPI1BRG = 3;                // baud rate = 12.5 MHz
    SPI1STATbits.SPIROV = 0;    // clear status
    RPF1R = 0b0101;             // SPI data out on F1
    SPI1CONbits.MODE32 = 0;     // 8-bit mode
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.MSTEN = 1;      // master mode
    SPI1CONbits.ON = 1;         // SPI on

    // Let's configure another SPI!
    SPI3CON = 0;
    SPI3CON2 = 0;
    (void) SPI3BUF;             // clear receive buffer
    SPI3STATbits.SPIROV = 0;    // clear status

    SDI3R = 0b0110;             // SDI on pin B10
    RPB5R = 0b0111;             // SDO on pin B5
    SPI3CONbits.MODE32 = 0;     // 8-bit mode
    SPI3CONbits.MODE16 = 0;
    SPI3CONbits.MSTEN = 0;      // slave mode
    SPI3CONbits.CKE = 1;        // output transition on falling clock edge
    SPI3CONbits.ENHBUF = 1;     // use 128-bit buffer
    SPI3CONbits.SRXISEL = 0b01; // interrupt when rbuf not empty
    SPI3CONbits.ON = 1;         // SPI on

    SPI3BUF = 0xFF;             // Load buffer with non-command for pi

}

void initRNG()
{
    RNGPOLY1 = 0x00C00003;
    RNGPOLY2 = 0x00000000;
    RNGCONbits.PLEN = 42;

    // RNG cannot be used until seedRNG() is called to start it running.
}

void seedRNG()
{
    RNGNUMGEN1 = RNGNUMGEN2 = TMR6;
    RNGCONbits.PRNGEN = 1;

    // Turn off the timer, we don't need it anymore
    T6CONbits.ON = 0;
}