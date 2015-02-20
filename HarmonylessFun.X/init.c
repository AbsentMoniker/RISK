#include <p32xxxx.h>
#include "init.h"

inline void systemUnlock()
{
    // disable interrupts
    asm volatile("di":::"memory");

    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

inline void systemLock()
{
    SYSKEY = 0x0;
    // enable interrupts
    asm volatile("ei":::"memory");
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
    TRISF = 0b11111111111111111100111111111101;
    TRISD = 0b11111111111111111111111111111101;
    TRISG = 0b11111111111111111111001111000011;

    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;

    PORTACLR = 0b11001111;
    PORTFCLR = 0b0011000000000000;

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
