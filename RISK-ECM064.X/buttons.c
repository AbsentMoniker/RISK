#include "buttons.h"
#include <p32xxxx.h>
#include <sys/attribs.h> // __ISR macro here
#include <stdint.h>

#include "gamelogic.h"

#define READY 0
#define SET 1
#define CLEARED -1

int advanceFlag = READY;
int cancelFlag = READY;
int nextFlag = READY;
int previousFlag = READY;

static unsigned buttonAdvanceState = 0;
static unsigned buttonCancelState = 0;
static unsigned encoderAState = 0;
static unsigned encoderBState = 0;

static unsigned previousEncoderA = 0;
static unsigned previousEncoderB = 0;

// Timer 2 interrupt: poll the buttons
void __ISR(_TIMER_2_VECTOR, IPL2SRS) pollButtons()
{
    register unsigned buttonStates = PORTF;
    buttonAdvanceState = (buttonAdvanceState << 1) | (buttonStates & BUTTON_ADVANCE_MASK);
    buttonCancelState = (buttonCancelState << 1) | (buttonStates & BUTTON_CANCEL_MASK);
    encoderAState = (encoderAState << 1) | (buttonStates & ENCODER_A_MASK);
    encoderBState = (encoderBState << 1) | (buttonStates & ENCODER_B_MASK);

    continentOwners[3] = previousEncoderA - 1;
    continentOwners[4] = previousEncoderB - 1;

    // Button has been pressed if the last six reads were true
    // Button has been released if the last six reads were false
    if(advanceFlag == READY && (buttonAdvanceState & BUTTON_ADVANCE_MASK) == 0)
        advanceFlag = SET;
    else if((buttonAdvanceState & BUTTON_ADVANCE_MASK) == BUTTON_ADVANCE_MASK)
        advanceFlag = READY;

    if(cancelFlag == READY && (buttonCancelState & BUTTON_CANCEL_MASK) == 0)
        cancelFlag = SET;
    else if((buttonCancelState & BUTTON_CANCEL_MASK) == BUTTON_CANCEL_MASK)
        cancelFlag = READY;

    if((encoderAState & ENCODER_A_MASK) == 0)
        previousEncoderA = 0;
    else if((encoderAState & ENCODER_A_MASK) == ENCODER_A_MASK)
    {
        if(previousEncoderA == 0)
        {
            if(previousEncoderB == 0)
                previousFlag = SET;
            else
                nextFlag = SET;
        }
        previousEncoderA = 1;
    }

    if((encoderBState & ENCODER_B_MASK) == 0)
        previousEncoderB = 0;
    else if((encoderBState & ENCODER_B_MASK) == ENCODER_B_MASK)
        previousEncoderB = 1;

    IFS0bits.T2IF = 0; // clear interrupt flag
}