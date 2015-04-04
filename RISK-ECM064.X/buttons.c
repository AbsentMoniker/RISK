#include "buttons.h"
#include <p32xxxx.h>
#include <sys/attribs.h> // __ISR macro here
#include <stdint.h>

#include "gamelogic.h"

#define READY 0
#define SET 1
#define CLEARED -1

static int advanceFlag = READY;
static int cancelFlag = READY;
static int nextFlag = READY;
static int previousFlag = READY;

static unsigned buttonAdvanceState = 0;
static unsigned buttonCancelState = 0;
static unsigned encoderAState = 0;
static unsigned encoderBState = 0;

static unsigned previousEncoderA = 0;
static unsigned previousEncoderB = 0;

// Timer 2 interrupt: poll the buttons
void __ISR(_TIMER_2_VECTOR, IPL2SRS) pollButtons()
{
    buttonAdvanceState = (buttonAdvanceState << 1) | BUTTON_ADVANCE_PIN;
    buttonCancelState = (buttonCancelState << 1) | BUTTON_CANCEL_PIN;
    encoderAState = (encoderAState << 1) | ENCODER_PIN_A;
    encoderBState = (encoderBState << 1) | ENCODER_PIN_B;

    continentOwners[3] = previousEncoderA - 1;
    continentOwners[4] = previousEncoderB - 1;

    // Button has been pressed if the last six reads were true
    // Button has been released if the last six reads were false
    if(advanceFlag == READY && (buttonAdvanceState & 0x3F) == 0)
        advanceFlag = SET;
    else if((buttonAdvanceState & 0x3F) == 0x3F)
        advanceFlag = READY;

    if(cancelFlag == READY && (buttonCancelState & 0x3F) == 0)
        cancelFlag = SET;
    else if((buttonCancelState & 0x3F) == 0x3F)
        cancelFlag = READY;

    if((encoderAState & 0x3F) == 0)
        updateEncoderFlagA(0);
    else if((encoderAState & 0x3F) == 0x3F)
        updateEncoderFlagA(1);

    if((encoderBState & 0x3F) == 0)
        updateEncoderFlagB(0);
    else if((encoderBState & 0x3F) == 0x3F)
        updateEncoderFlagB(1);

    IFS0bits.T2IF = 0; // clear interrupt flag
}

int flagSetAdvance() { return advanceFlag == SET; }
int flagSetCancel() { return cancelFlag == SET; }
int flagSetNext() { return nextFlag == SET; }
int flagSetPrevious() { return previousFlag == SET; }

void clearFlagAdvance() { advanceFlag = CLEARED; }
void clearFlagCancel() { cancelFlag = CLEARED; }
void clearFlagNext() { nextFlag = CLEARED; }
void clearFlagPrevious() { previousFlag = CLEARED; }

#define ENCODER_NONE 0
#define ENCODER_CCW 1
#define ENCODER_CW 2
#define ENCODER_ERROR 3

// lookup with {oldA, oldB, newA, newB}
static const int encoderDecodings[16] =
{
    // NEW AB:  00             01             10             11
    /* OLD */
    /* A B */
    /* 0 0 */   ENCODER_NONE,  ENCODER_CCW,   ENCODER_CW,    ENCODER_ERROR,
    /* 0 1 */   ENCODER_CW,    ENCODER_NONE,  ENCODER_ERROR, ENCODER_CCW,
    /* 1 0 */   ENCODER_CCW,   ENCODER_ERROR, ENCODER_NONE,  ENCODER_CW,
    /* 1 1 */   ENCODER_ERROR, ENCODER_CW,    ENCODER_CCW,   ENCODER_NONE,
};

void updateEncoderFlagA(unsigned newA)
{
    unsigned lookup = (previousEncoderA << 3) | (previousEncoderB << 2) | (newA << 1) | previousEncoderB;
    if(encoderDecodings[lookup] == ENCODER_CW)
        nextFlag = SET;
    else if(encoderDecodings[lookup] == ENCODER_CCW)
        previousFlag = SET;
    previousEncoderA = newA;
}

void updateEncoderFlagB(unsigned newB)
{
    unsigned lookup = (previousEncoderA << 3) | (previousEncoderB << 2) | (previousEncoderA << 1) | newB;
    if(encoderDecodings[lookup] == ENCODER_CW)
        nextFlag = SET;
    else if(encoderDecodings[lookup] == ENCODER_CCW)
        previousFlag = SET;
    previousEncoderB = newB;
}