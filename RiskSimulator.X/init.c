#include <p32xxxx.h>
#include "init.h"
#include <xc.h>

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

    // Timer 4/5 interrupts
    //IFS0bits.T5IF = 0;
    //IEC0bits.T5IE = 1;
    //IPC6bits.T5IP = 7;
    //IPC6bits.T5IS = 0;

    // Port D change interrupt enabled with priority 1.0
    IFS3bits.CNDIF = 0;
    IEC3bits.CNDIE = 1;
    IPC30bits.CNDIP = 7;
    IPC30bits.CNDIS = 0;
    // enable change interrupt on bit 13
    CNCONDbits.ON = 1;
    CNENDbits.CNIED13 = 1;

    INTCONbits.MVEC = 1;

    __builtin_enable_interrupts();
}


void initPorts()
{
    // On the Explorer 16 board, the LEDs are (from left to right):
    // A7  A6  F12 F13 A3  A2  A1  A0
    // Buttons are:
    // N/C D13 A7  F8
    //
    // F1 and D1 are used for SPI1
    // G is used for ??? but it got set that way in harmony
    //
    //        3322222222221111111111
    //        10987654321098765432109876543210
    TRISA = 0b11111111111111111111111100110000;
    TRISF = 0b11111111111111111100111111111111;
    TRISD = 0b11111111111111111111111111111111;
    TRISG = 0b11111111111111111111001111000011;

    // Nothing is analog
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;

    // Make sure the LEDs are off when the program starts
    PORTACLR = 0b11001111;
    PORTFCLR = 0b0011000000000000;

}

void initTimers()
{
    // Let's configure a timer!
    T4CON = 0;               // timers off
    T5CON = 0;
    T4CONbits.TCS = 0;       // use internal oscillator
    T4CONbits.T32 = 1;       // 32-bit timer
    T4CONbits.TCKPS = 0b111; // 1:256 pre-scale
    //PR4 = 0xFFFFFFFF;        // maximum period
    PR4 = 0x00001000;

}
