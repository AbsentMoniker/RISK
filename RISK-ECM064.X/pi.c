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
        terrPtr[i] = territories[i].owner;
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

void __ISR(_SPI2_RX_VECTOR, IPL4SRS) SPI2RXisr()
{
    static unsigned char * piDataPtr = NULL;

    unsigned char rx = SPI2BUF;
    continentOwners[0] = 0;

    if(rx == 0xFE)
    {
        // Pi is starting a data request
        piDataPtr = piData;
        continentOwners[1] = 2;

        SPI2BUF = *piDataPtr++;
        
    }
    else if(rx == 0xFF)
    {
        continentOwners[2] = 2;
        // Pi needs another byte

        if(piDataPtr == NULL)
        {
            SPI2BUF = 0x80;

            IFS4bits.SPI2RXIF = 0; // clear interrupt flag
            return;
        }
        else if(piDataPtr - piData == PI_DATA_LENGTH)
        {
            // End of data
            piDataPtr = NULL;
            SPI2BUF = 0xFF;
        }
        else
        {
            // Prepare next byte
            SPI2BUF = *piDataPtr++;
        }
    }
    else if(rx == 0xFD)
    {
        continentOwners[3] = 2;
        // Pi is starting a command
        cmdPtr = piCommand;
        SPI2BUF = 0xFF;
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
        SPI2BUF = 0xFF;
    }

    IFS4bits.SPI2RXIF = 0; // clear interrupt flag
}

int flagSetPiCommand()
{

}
void clearFlagPiCommand()
{

}
