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

unsigned char piStats[PI_STATS_LENGTH];
static unsigned char * statDicePtr = piStats;
static unsigned char * statTroopsPtr = piStats + (14*MAX_PLAYERS);
static unsigned char * statHoldsPtr = piStats + (14*MAX_PLAYERS) + (6*MAX_PLAYERS);
static unsigned char * statTakenPtr = piStats + (14*MAX_PLAYERS) + (6*MAX_PLAYERS) + (2*MAX_PLAYERS);

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

void updatePiStats()
{
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        statDicePtr[14*i] = totalDiceRolls[i] >> 8;
        statDicePtr[14*i + 1] = totalDiceRolls[i] & 0xFF;
        for(int d = 0; d < 6; d++)
        {
            statDicePtr[14*i + 2 + d*2] = diceRolls[i][d] >> 8;
            statDicePtr[14*i + 2 + d*2] = diceRolls[i][d] & 0xFF;
        }
    }
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        statTroopsPtr[6*i] = reinforcementCount[i] >> 8;
        statTroopsPtr[6*i + 1] = reinforcementCount[i] & 0xFF;
        statTroopsPtr[6*i + 2] = killCount[i] >> 8;
        statTroopsPtr[6*i + 3] = killCount[i] & 0xFF;
        statTroopsPtr[6*i + 4] = deathCount[i] >> 8;
        statTroopsPtr[6*i + 5] = deathCount[i] & 0xFF;
    }
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        statHoldsPtr[2*i] = longestHeldTerritory[i].territory;
        statHoldsPtr[2*i + 1] = longestHeldTerritory[i].count;
    }

    statTakenPtr[0] = mostTakenTerritory.territory;
    statTakenPtr[1] = mostTakenTerritory.count;
}

void __ISR(_SPI2_RX_VECTOR, IPL4SRS) SPI2RXisr()
{
    static unsigned char * piDataPtr = NULL;
    static unsigned char * piStatsPtr = NULL;
    static int sendingStats = 0;

    unsigned char rx = SPI2BUF;

    if(rx == 0xFE)
    {
        // Pi is starting a data request
        piDataPtr = piData;
        SPI2BUF = *piDataPtr++;
        sendingStats = 0;
    }
    else if(rx == 0xFC)
    {
        // Pi is starting a stats request
        piStatsPtr = piStats;
        SPI2BUF = *piStatsPtr++;
        sendingStats = 1;
    }
    else if(rx == 0xFF)
    {
        // Pi needs another byte
        if(sendingStats)
        {
            if(piStatsPtr == NULL)
            {
                SPI2BUF = 0x80;
            }
            else if(piStatsPtr - piStats == PI_STATS_LENGTH)
            {
                // End of data
                piStatsPtr = NULL;
                SPI2BUF = 0xFF;
            }
            else
            {
                // Prepare next byte
                SPI2BUF = *piStatsPtr++;
            }
        }
        else
        {
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
