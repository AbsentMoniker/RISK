#include "displays.h"
#include <sys/attribs.h>
#include <p32xxxx.h>

#include "gamelogic.h"

unsigned char displayData[DISPLAY_DATA_LENGTH];
static unsigned char * displayDataTerritories = displayData;
static unsigned char * displayDataCards = displayData + (3*NUM_TERRITORIES);
static unsigned char * displayDataDice = displayData + 3 + (3*NUM_TERRITORIES);
static unsigned char * displayDataContinents = displayData + 5 + 3 + (3*NUM_TERRITORIES);

#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")
#define PULSE_RCLK() do{ PORTDbits.RD9 = 1; SHORTWAIT(); PORTDbits.RD9 = 0; }while(0)


void updateDisplayData()
{
    // Territory displays
    for(int i = 0, territory = NUM_TERRITORIES - 1; territory >= 0; i += 3, territory -= 1)
    {
        displayDataTerritories[i] = PLAYER_COLOR(territories[territory].owner) << TERRITORY_LED_SHIFT;
        displayDataTerritories[i+1] = (territories[territory].troops / 10) % 10 != 0? digits[(territories[territory].troops / 10) % 10] : 0;
        displayDataTerritories[i+2] = digits[territories[territory].troops % 10];
    }

    // Card displays
    displayDataCards[0] = digits[cardExchangeValue % 10];
    displayDataCards[1] = (cardExchangeValue / 10) % 10 != 0? digits[(cardExchangeValue / 10) % 10] : 0;
    displayDataCards[2] = COLOR_BLACK; // cards have no LEDs

    // Dice displays
    displayDataDice[0] = defenderDice[1] != 0? digits[defenderDice[1]] : 0;
    displayDataDice[1] = defenderDice[0] != 0? digits[defenderDice[0]] : 0;
    displayDataDice[2] = attackerDice[2] != 0? digits[attackerDice[2]] : 0;
    displayDataDice[3] = attackerDice[1] != 0? digits[attackerDice[1]] : 0;
    displayDataDice[4] = attackerDice[0] != 0? digits[attackerDice[0]] : 0;

    // Continent displays
    displayDataContinents[0] = (PLAYER_COLOR(continentOwners[0]) << CONTINENT_LED_UPPER_SHIFT) |
            (PLAYER_COLOR(continentOwners[1]) << CONTINENT_LED_LOWER_SHIFT);
    displayDataContinents[1] = (PLAYER_COLOR(continentOwners[2]) << CONTINENT_LED_UPPER_SHIFT) |
            (PLAYER_COLOR(continentOwners[3]) << CONTINENT_LED_LOWER_SHIFT);
    displayDataContinents[2] = (PLAYER_COLOR(continentOwners[4]) << CONTINENT_LED_UPPER_SHIFT) |
            (PLAYER_COLOR(continentOwners[5]) << CONTINENT_LED_LOWER_SHIFT);
}

static int blankDisplays = 0;
static unsigned char * displayDataPtr = NULL;

void __ISR(_TIMER_3_VECTOR, IPL3SRS) startDisplays()
{
    static int counter = 0;

    counter++;
    if(counter % 5 == 0)
    {
        // start shifting data
        blankDisplays = 0;
    }
    else if(counter % 5 == 1)
    {
        // display data
        PULSE_RCLK();
        // start shifting blanks
        //blankDisplays = 1;
    }
    else
    {
        if(counter % 5 == 2)
        {
            // display blanks
            PULSE_RCLK();
        }
        
        // just clear the flag, don't actually shift anything
        IFS0bits.T3IF = 0;
        return;
    }

    displayDataPtr = displayData;
    while(!SPI4STATbits.SPITBF)
    {
        SPI4BUF = blankDisplays? 0x00 : *displayDataPtr;
        displayDataPtr++;
    }

    IEC5bits.SPI4TXIE = 1;

    IFS0bits.T3IF = 0;
}

void __ISR(_SPI4_TX_VECTOR, IPL4SRS) refillDisplayBuffer()
{
    while(!SPI4STATbits.SPITBF)
    {
        SPI4BUF = blankDisplays? 0x00 : *displayDataPtr;
        displayDataPtr++;
        if(displayDataPtr - displayData == DISPLAY_DATA_LENGTH)
        {
            displayDataPtr = NULL;
            IEC5bits.SPI4TXIE = 0;
            break;
        }
    }
    IFS5bits.SPI4TXIF = 0;
}