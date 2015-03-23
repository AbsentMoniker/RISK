#include "buttons.h"
#include <p32xxxx.h>
#include <sys/attribs.h> // __ISR macro here
#include <stdint.h>

#define READY 0
#define SET 1
#define CLEARED -1

static int buttonAdvanceFlag = READY;
static int buttonCancelFlag = READY;
static int buttonNextFlag = READY;

static unsigned buttonAdvanceState = 0;
static unsigned buttonCancelState = 0;
static unsigned buttonNextState = 0;

// Timer 2 interrupt: poll the buttons
void __ISR(_TIMER_2_VECTOR, IPL2SRS) pollButtons()
{
    buttonAdvanceState = (buttonAdvanceState << 1) | BUTTON_ADVANCE_PIN;
    buttonCancelState = (buttonCancelState << 1) | BUTTON_CANCEL_PIN;
    buttonNextState = (buttonNextState << 1) | BUTTON_NEXT_PIN;

    // Button has been pressed if the last six reads were false
    // Button has been released if the last six reads were true
    if(buttonAdvanceFlag == READY)
    {
        if((buttonAdvanceState & 0x3F) == 0)
            buttonAdvanceFlag = SET;
    }
    else
    {
        if((buttonAdvanceState & 0x3F) == 0x3F)
            buttonAdvanceFlag = READY;
    }

    if(buttonCancelFlag == READY)
    {
        if((buttonCancelState & 0x3F) == 0)
            buttonCancelFlag = SET;
    }
    else
    {
        if((buttonCancelState & 0x3F) == 0x3F)
            buttonCancelFlag = READY;
    }

    if(buttonNextFlag == READY)
    {
        if((buttonNextState & 0x3F) == 0)
            buttonNextFlag = SET;
    }
    else
    {
        if((buttonNextState & 0x3F) == 0x3F)
            buttonNextFlag = READY;
    }

    IFS0bits.T2IF = 0; // clear interrupt flag
}

int flagSet_advance() { return buttonAdvanceFlag == SET; }
int flagSet_cancel() { return buttonCancelFlag == SET; }
int flagSet_next() { return buttonNextFlag == SET; }

void clearFlag_advance() { buttonAdvanceFlag = CLEARED; }
void clearFlag_cancel() { buttonCancelFlag = CLEARED; }
void clearFlag_next() { buttonNextFlag = CLEARED; }
