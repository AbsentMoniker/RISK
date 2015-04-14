#include "pi.h"
#include <p32xxxx.h>
#include <sys/attribs.h>
#include "gamelogic.h"
#include <string.h>


unsigned char piData[PI_DATA_LENGTH];
unsigned char piDataBuffer[PI_DATA_LENGTH];
static unsigned char * terrPtr = piDataBuffer;
static unsigned char * cardsPtr = piDataBuffer + NUM_TERRITORIES;

unsigned char piCommand[PI_COMMAND_LENGTH];
static unsigned char * cmdPtr = piCommand;
static int piCommandFlag = 0;

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
    memcpy(piData, piDataBuffer, PI_DATA_LENGTH);
}

void __ISR(_SPI2_RX_VECTOR, IPL4SRS) SPI2RXisr()
{
    static unsigned char * piDataPtr = NULL;

    unsigned char rx = SPI2BUF;

    if(rx == 0xFE)
    {
        // Pi is starting a data request
        piDataPtr = piData;
        SPI2BUF = *piDataPtr++;
    }
    else if(rx == 0xFF)
    {
        // Pi needs another byte

        if(piDataPtr == NULL)
        {
            SPI2BUF = 0x80;
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
        // Pi is starting a command
        cmdPtr = piCommand;
        // No meaningful response
        SPI2BUF = 0xFF;
    }
    else
    {
        // PI is continuing a commmand
        if(cmdPtr != NULL)
        {
            *cmdPtr++ = rx;
            if(cmdPtr - piCommand == 4)
            {
                // PI has finished a command
                cmdPtr = NULL;
                piCommandFlag = 1;
            }
            
        }
        // No meaningful response
        SPI2BUF = 0xFF;
    }

    IFS4bits.SPI2RXIF = 0; // clear interrupt flag
}

int flagSetPiCommand()
{
    return piCommandFlag == 1;
}
void clearFlagPiCommand()
{
    piCommandFlag = 0;
}
