// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#include "pi.h"
#include <p32xxxx.h>
#include <sys/attribs.h>
#include "gamelogic.h"

unsigned char piData[PI_DATA_LENGTH];
static unsigned char * terrPtr = piData;
static unsigned char * cardsPtr = piData + NUM_TERRITORIES;

unsigned char piCommand[PI_COMMAND_LENGTH];
static unsigned char * cmdPtr = piCommand;
static int piCommandFlag;

void updatePiData()
{
    for(int i = 0; i < NUM_TERRITORIES; i++)
    {
        //terrPtr[i] = territories[i].owner;
        terrPtr[i] = i % 3;
    }

    for(int i = 0; i < NUM_CARDS; i++)
    {
        cardsPtr[i] = 0xFF;
    }
#if 0
    for(int i = 0; i < numPlayers; i++)
    {
        for(int j = 0; j < hands[i].cards; j++)
        {
            cardsPtr[hands[i].hand[j].index] = i;
        }
    }
#endif
}

void __ISR(_SPI3_RX_VECTOR, IPL4SRS) SPI3RXisr()
{
    static unsigned char * piDataPtr = NULL;

    unsigned char rx = SPI3BUF;

    if(rx == 0xFE)
    {
        // Pi is starting a data request
        piDataPtr = piData;

        SPI3BUF = *piDataPtr++;
    }
    else if(rx == 0xFF)
    {
        // Pi needs another byte

        if(piDataPtr == NULL)
        {
            SPI3BUF = 0x80;
            IFS4bits.SPI3RXIF = 0; // clear interrupt flag
            return;
        }
        else if(piDataPtr - piData == PI_DATA_LENGTH)
        {
            // End of data
            piDataPtr = NULL;
            SPI3BUF = 0xFF;
        }
        else
        {
            // Prepare next byte
            SPI3BUF = *piDataPtr++;
        }
    }
    else if(rx == 0xFD)
    {
        // Pi is starting a command
        cmdPtr = piCommand;
        SPI3BUF = 0xFF;
    }
    else
    {
        // PI is continuing a commmand
        if(cmdPtr != NULL)
        {
            *cmdPtr++ = rx;
            if(cmdPtr - piCommand == 4)
                cmdPtr = NULL;

        }
        SPI3BUF = 0xFF;
    }

    IFS4bits.SPI3RXIF = 0; // clear interrupt flag
}

int flagSetPiCommand()
{

}
void clearFlagPiCommand()
{
    
}
