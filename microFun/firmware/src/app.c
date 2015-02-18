
#include "app.h"

int i;
void APP_Initialize ( void )
{

    //        10987654321098765432109876543210
    TRISA = 0b11111111111111111111111111110000;
    TRISF = 0b11111111111111111111111111111101;
    TRISD = 0b11111111111111111111111111111101;

    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;
    ANSELE = 0x0000;
    ANSELF = 0x0000;
    ANSELG = 0x0000;

    PORTA = 0;

    // Let's configure an SPI!
    SPI1CON = 0;        // SPI off
    SPI1BRG = 0x1;      // baud rate
    SPI1STATCLR = 0x40; // clear status
    SPI1CON = 0x8020;   // SPI on, master mode
    RPF1R = 0b0101;     // SPI data out on F1


    // Let's configure a timer!
    T4CON = 0;
    T5CON = 0;
    T4CONbits.TCS = 0;       // internal oscillator
    T4CONbits.T32 = 1;       // 32-bit timer
    T4CONbits.TCKPS = 0b111; // 1:256 pre-scale
    PR4 = 0xFFFFFFFF;        // maximum period


}

void msleep(int msecs)
{
    TMR4 = 0;
    T4CONbits.ON = 1;
    while(TMR4 < msecs * 782) // timer counts in increments of 1.28 us
    {}
    T4CONbits.ON = 0;

}

int digits[10] = {
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

int counter = 0;
#define SHORTWAIT() asm("nop\n nop\n nop\n nop\n nop\n nop")
void APP_Tasks ( void )
{
    int ones = counter % 10;
    int tens = counter / 10;

    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = digits[ones];
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = digits[tens];
    while(SPI1STATbits.SPITBE != 1) {}
    SPI1BUF = counter % 8;
    msleep(100);

    counter += 1;
    if(counter >= 100)
        counter = 0;

}
