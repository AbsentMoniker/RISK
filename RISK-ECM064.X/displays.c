#include "displays.h"
#include <sys/attribs.h>
#include <p32xxxx.h>

#include "gamelogic.h"
#include "buttons.h"


unsigned char displayData[DISPLAY_DATA_LENGTH];
static unsigned char * displayDataTerritories = displayData;
static unsigned char * displayDataCards = displayData + (3*NUM_TERRITORIES);
static unsigned char * displayDataDice = displayData + 3 + (3*NUM_TERRITORIES);
static unsigned char * displayDataContinents = displayData + 5 + 3 + (3*NUM_TERRITORIES);

#define SHORTWAIT() asm volatile ("nop\n nop\n nop\n nop\n nop\n nop")
#define PULSE_RCLK() do{ PORTDbits.RD9 = 1; SHORTWAIT(); PORTDbits.RD9 = 0; }while(0)

const int playerColors[MAX_PLAYERS] =
{
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_MAGENTA,
    COLOR_CYAN,
};

static int displaysEnabled = 1;

int cardsPlayer = -1;
int cardsTimeMillis = -1;


void startNewFrame()
{
    int time = TMR4 / 390;
    if(time < 10)
        return;
    
    T4CONbits.ON = 0;
    TMR4 = 0;
    T4CONbits.ON = 1;

    if(cardsTimeMillis >= 0)
    {
        cardsTimeMillis -= time;
        if(cardsTimeMillis < 0)
            cardsPlayer = -1;
    }
}

void startCardsColor(int player)
{
    cardsPlayer = player;
    cardsTimeMillis = 2000;
}

void updateDisplayData()
{
    // Territory displays
    for(int i = 0, territory = NUM_TERRITORIES - 1; territory >= 0; i += 3, territory -= 1)
    {
        displayDataTerritories[i] = digits[territories[territory].troops % 10];
        displayDataTerritories[i+1] = (territories[territory].troops / 10) % 10 != 0? digits[(territories[territory].troops / 10) % 10] : 0;
        if((source == territory && TMR8 < T8_MSECS(250)) ||
            (destination == territory && TMR8 > T8_MSECS(250)))
        {
            displayDataTerritories[i+2] = COLOR_WHITE << TERRITORY_LED_SHIFT;
        }
        else
        {
            displayDataTerritories[i+2] = playerColors[territories[territory].owner] << TERRITORY_LED_SHIFT;
        }
    }

    // Card displays
    displayDataCards[0] = digits[cardExchangeValue % 10];
    displayDataCards[1] = (cardExchangeValue / 10) % 10 != 0? digits[(cardExchangeValue / 10) % 10] : 0;
    displayDataCards[2] = playerColors[cardsPlayer]; // cards have no LEDs

    // Dice displays
    displayDataDice[0] = defenderDice[1] != 0? digits[defenderDice[1]] : 0;
    displayDataDice[1] = defenderDice[0] != 0? digits[defenderDice[0]] : 0;
    displayDataDice[2] = attackerDice[2] != 0? digits[attackerDice[2]] : 0;
    displayDataDice[3] = attackerDice[1] != 0? digits[attackerDice[1]] : 0;
    displayDataDice[4] = attackerDice[0] != 0? digits[attackerDice[0]] : 0;

    // Continent displays
#ifdef USE_SIMPLE_MAP
    displayDataContinents[0] = 0x00;
    displayDataContinents[1] = 0x00;
    displayDataContinents[2] = 0x00;
    displayDataContinents[3] = 0x00;
    displayDataContinents[4] = 0x00;
    displayDataContinents[5] = 0x00;
#else
    updateContinents();
    
    displayDataContinents[0] = (playerColors[continentOwners[1]] << CONTINENT_LED_UPPER_SHIFT) |
            (playerColors[continentOwners[0]] << CONTINENT_LED_LOWER_SHIFT);
    displayDataContinents[1] = (playerColors[continentOwners[3]] << CONTINENT_LED_UPPER_SHIFT) |
            (playerColors[continentOwners[2]] << CONTINENT_LED_LOWER_SHIFT);
    displayDataContinents[2] = (playerColors[continentOwners[5]] << CONTINENT_LED_UPPER_SHIFT) |
            (playerColors[continentOwners[4]] << CONTINENT_LED_LOWER_SHIFT);
#endif
}

static int blankDisplays = 0;
static unsigned char * displayDataPtr = NULL;

void __ISR(_TIMER_3_VECTOR, IPL3SRS) startDisplays()
{
    static int counter = 0;

    counter++;
    if(counter % 5 == 0)
    {
        // start shifting data if the power switch allows it
        if(powerOn() && displaysEnabled)
            blankDisplays = 0;
        else
            blankDisplays = 1;
    }
    else if(counter % 5 == 1)
    {
        // display data
        PULSE_RCLK();
        // start shifting blanks
        blankDisplays = 1;
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
    
    IEC5bits.SPI4TXIE = 1;

    while(!SPI4STATbits.SPITBF)
    {
        // We want the dice to be displayed at full brightness, everything else at reduced brightness.
        if(blankDisplays && !(displayDataPtr >= displayDataDice && displayDataPtr < displayDataDice + 5))
            SPI4BUF = 0x00;
        else
            SPI4BUF = *displayDataPtr;
        displayDataPtr++;
        
        if(displayDataPtr - displayData == DISPLAY_DATA_LENGTH)
        {
            displayDataPtr = NULL;
            IEC5bits.SPI4TXIE = 0;
            break;
        }
    }

    IFS0bits.T3IF = 0;
}

void disableDisplays() { displaysEnabled = 0; }
void enableDisplays() { displaysEnabled = 1; }


void __ISR(_SPI4_TX_VECTOR, IPL3SRS) refillDisplayBuffer()
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
