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

int flagSetAdvance();
int flagSetCancel();
int flagSetNext();
int flagSetPrevious();
void clearFlagAdvance();
void clearFlagCancel();
void clearFlagNext();
void clearFlagPrevious();

void updateEncoderFlagA(unsigned newstate);
void updateEncoderFlagB(unsigned newstate);

#endif	/* BUTTONS_H */
