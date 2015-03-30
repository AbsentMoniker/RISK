#include "pi.h"
#include <p32xxxx.h>
#include <sys/attribs.h>
#include "gamelogic.h"

unsigned char piData[PI_DATA_LENGTH];
static unsigned char * terrPtr = piData;
static unsigned char * cardsPtr = piData + NUM_TERRITORIES;

unsigned char piCommand[PI_COMMAND_LENGTH];
static unsigned char * cmdPtr = piCommand;

void updatePiData()
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        terrPtr[i] = territories[i].owner;
    }

    for(int i = 0; i < NUM_CARDS; i++)
    {
        cardsPtr[i] = 0xFF;
    }
    for(int i = 0; i < numPlayers; i++)
    {
        for(int j = 0; j < hands[i].cards; j++)
        {
            cardsPtr[hands[i].hand[j].index] = i;
        }
    }
}

void __ISR(_SPI3_RX_VECTOR, IPL4SRS) SPI3RXisr()
{
    static unsigned char * piDataPtr = NULL;

    unsigned char rx = SPI3BUF;

    if(rx == 0xFF)
    {
        if(piDataPtr == NULL)
        {
            // Pi is starting a data request
            piDataPtr = piData;
        }

        if(piDataPtr - piData == PI_DATA_LENGTH)
        {
            // End of data
            piDataPtr = NULL;
            SPI3BUF = 0x01;
        }
        else
        {
            // Prepare next byte
            //SPI3BUF = *piDataPtr++;
            SPI3BUF = 0x01;
        }
    }
    else
    {
        if(cmdPtr - piCommand == 4)
        {
            cmdPtr = piCommand;
        }
        *cmdPtr++ = rx;
        SPI3BUF = 0xFF;
    }

    IFS4bits.SPI3RXIF = 0; // clear interrupt flag
}
