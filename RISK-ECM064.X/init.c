#include <p32xxxx.h>
#include "init.h"

// ***********************************
// ******                       ******
// ******        ECM-064        ******
// ******                       ******
// ***********************************


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
    PB3DIV = 0x8001;
    PB4DIV = 0x8001;
    PB5DIV = 0x8001;
    PB7DIV = 0x8001;
    PB8DIV = 0x8001;

    REFO1CONCLR = 0;
    REFO2CONCLR = 0;
    REFO3CONCLR = 0;
    REFO4CONCLR = 0;

    systemLock();
}

void initInterrupts()
{
    __builtin_disable_interrupts();

    // Shadow register sets for each interrupt level
    // why this is configurable is beyond me
    PRISS = 0x76543210;

    // Timer 2 interrupt: priority 2.0
    // ISR in buttons.c
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 2;
    IPC2bits.T2IS = 0;

    // Timer 3 interrupt: priority 3.0
    // ISR in displays.c
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;

    // SPI4 transmit buffer interrupt: priority 3.1
    // This interrupt will only be enabled when there is more data ready to
    // be stored in the SPI's transmit buffer; after each time the data is
    // finished shifting, the buffer will sit empty so the interrupt should be
    // disabled.
    // ISR in displays.c
    IFS5bits.SPI4TXIF = 0;
    IEC5bits.SPI4TXIE = 0;
    IPC41bits.SPI4TXIP = 3;
    IPC41bits.SPI4TXIS = 1;

    // SPI 2 rx interrupt
    // priority 4.0
    // ISR in pi.c
    IFS4bits.SPI2RXIF = 0;
    IEC4bits.SPI2RXIE = 1;
    IPC35bits.SPI2RXIP = 4;
    IPC35bits.SPI2RXIS = 0;


    INTCONbits.MVEC = 1;

    __builtin_enable_interrupts();
}

void initPorts()
{
    // 1 = input, 0 = output
    //
    //        111111
    //        5432109876543210     pins available
    TRISB = 0b0000000011111111; // 0 - 15
    TRISC = 0b1011111111111111; // 12 - 15
    TRISD = 0b1111000111000001; // 0 - 5, 9 - 11
    TRISE = 0b1111111111111111; // 0 - 7
    TRISF = 0b1111111111111111; // 0 - 1, 3 - 5
    TRISG = 0b1111111011111111; // 6 - 9

    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;


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
    // Used to time length of color display on cards
    T4CON = 0;
    T4CONbits.TCKPS = 0b111; // 1:256 prescale = 2.56us
    PR4 = 0xFFFF;
    TMR4 = 0;
    T4CONbits.ON = 1;

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
    PR8 = 781250;            // 0.64us * 781250 = 500 ms
    TMR8 = 0;
    T8CONbits.ON = 1;

}

void initSPI()
{

#if 1
    // ----- SPI 1 -----
    // Used for shifting data into LCD.
    SPI1CON = 0;
    SPI1CON2 = 0;
    SPI1STATbits.SPIROV = 0;    // clear status
    
    SPI1BRG = 3;                // baud rate = 12.5 MHz
    RPD2R = 0b0101;             // SPI data out on D2
    SPI1CONbits.DISSDI = 1;     // disable input
    SPI1CONbits.MODE32 = 0;     // 8-bit mode
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.MSTEN = 1;      // master mode
    SPI1CONbits.CKE = 1;        // data changes on neg edge
    SPI1CONbits.ON = 1;         // SPI on

#endif
    
    // ----- SPI 2 -----
    // Used for communicating with Pi.
    SPI2CON = 0;
    SPI2CON2 = 0;
    SPI2STATbits.SPIROV = 0;    // clear status
    (void) SPI2BUF;             // clear receive buffer
    
    // SDO2 output pin? G8
    RPG8R = 0b0110;
    // SDI2 input pin? G7
    SDI2R = 0b0001;
    SS2R = 0b0001;
    SPI2CONbits.MODE32 = 0;     // 8-bit mode
    SPI2CONbits.MODE16 = 0;
    SPI2CONbits.MSTEN = 0;      // slave mode
    SPI2CONbits.SRXISEL = 0b01;
    SPI2CONbits.CKE = 1;
    SPI2CONbits.ENHBUF = 1;
    SPI2CONbits.ON = 1;         // SPI on

    SPI2BUF = 0xFF;

    // ----- SPI 3 ----
    // Not used
    SPI3CON = 0;
    
    // ----- SPI 4 -----
    // Used for outputting data to the 43 territory boards, and the
    // dice displays and continent displays on the main board.
    SPI4CON = 0;
    SPI4CON2 = 0;
    SPI4STATbits.SPIROV = 0;    // clear status

    SPI4BRG = 14;                // baud rate 3.33 MHz
    RPD11R = 0b1000;            // SDO4 out on pin D11
    SPI4CONbits.DISSDI = 1;     // disable input
    SPI4CONbits.ENHBUF = 1;     // full 128-bit buffer
    SPI4CONbits.MODE32 = 0;     // 8-bit mode
    SPI4CONbits.MODE16 = 0;
    SPI4CONbits.MSTEN = 1;      // master mode
    SPI4CONbits.STXISEL = 0b10; // interrupt when buffer half-empty
    SPI4CONbits.ON = 1;         // SPI on

}

// Public domain code from
// http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
//
// The built-in hardware PRNG is used to provide the seed
// (ie the 16 initial 32-bit value of state), then calls to randint
// will advance this RNG.

/* initialize state to random bits */
static unsigned long rngstate[16];
/* init should also reset this to 0 */
static unsigned int rngindex = 0;
/* return 32 bit random number */
unsigned long nextrandom(void)
{
    unsigned long a, b, c, d;
    a  = rngstate[rngindex];
    c  = rngstate[(rngindex+13)&15];
    b  = a^c^(a<<16)^(c<<15);
    c  = rngstate[(rngindex+9)&15];
    c ^= (c>>11);
    a  = rngstate[rngindex] = b^c;
    d  = a^((a<<5)&0xDA442D24UL);
    rngindex = (rngindex + 15)&15;
    a  = rngstate[rngindex];
    rngstate[rngindex] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return rngstate[rngindex];
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
    for(int i = 0; i < 16; i++)
    {
        rngstate[i] = RNGNUMGEN1;
        // Wait 42 cycles for the next number to be available
        asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n"
                     "nop\n nop\n nop\n nop\n nop\n nop\n");
    }
    rngindex = 0;
    RNGCONbits.PRNGEN = 0;
}

#ifdef USE_RANDOM
int randint(int min, int max)
{
    return min + (nextrandom() % (max - min + 1));
}
#else
int randint(int min, int max)
{
    return min;
}
#endif






