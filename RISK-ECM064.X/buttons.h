/* 
 * File:   buttons.h
 * Author: jacob
 *
 * Created on March 23, 2015, 4:47 PM
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#include <p32xxxx.h>

#define BUTTON_ADVANCE_PIN  PORTFbits.RF4
#define BUTTON_CANCEL_PIN   PORTFbits.RF5
#define ENCODER_PIN_A       PORTFbits.RF0
#define ENCODER_PIN_B       PORTFbits.RF1
#define BUTTON_POWER_PIN    PORTFbits.RF3

#define BUTTON_ADVANCE_SHIFT 4
#define BUTTON_CANCEL_SHIFT 5
#define ENCODER_A_SHIFT 0
#define ENCODER_B_SHIFT 1
#define BUTTON_POWER_SHIFT 3

#define BUTTON_ADVANCE_MASK 0x00000010
#define BUTTON_CANCEL_MASK 0x00000020
#define ENCODER_A_MASK 0x00000001
#define ENCODER_B_MASK 0x00000002
#define BUTTON_POWER_MASK 0x0000008

#define BUTTON_ADVANCE_STATE_MASK 0x000003F0
#define BUTTON_CANCEL_STATE_MASK 0x000007E0
#define ENCODER_A_STATE_MASK 0x00000007
#define ENCODER_B_STATE_MASK 0x0000000E
#define BUTTON_POWER_STATE_MASK 0x000001F8

extern int advanceFlag;
extern int cancelFlag;
extern int nextFlag;
extern int previousFlag;
extern int powerOffFlag;
extern int powerOnFlag;
extern int holdTurnFlag;

static inline int flagSetAdvance() { return advanceFlag == 1; }
static inline int flagSetCancel() { return cancelFlag == 1; }
static inline int flagSetNext() { return nextFlag == 1; }
static inline int flagSetPrevious() { return previousFlag == 1; }
static inline int flagSetPowerOff() { return powerOffFlag == 1; }
static inline int flagSetPowerOn() { return powerOnFlag == 1; }
static inline int flagSetHoldTurn() { return holdTurnFlag == 1; }

static inline void clearFlagAdvance() { advanceFlag = -1; }
static inline void clearFlagCancel() { cancelFlag = -1; }
static inline void clearFlagNext() { nextFlag = -1; }
static inline void clearFlagPrevious() { previousFlag = -1; }
static inline void clearFlagPowerOff() { powerOffFlag = -1; }
static inline void clearFlagPowerOn() { powerOnFlag = -1; }
static inline void clearFlagHoldTurn() { holdTurnFlag = -1; }

int powerOn();

#endif	/* BUTTONS_H */
