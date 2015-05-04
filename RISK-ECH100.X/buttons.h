// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.

#ifndef BUTTONS_H
#define	BUTTONS_H

#include <p32xxxx.h>

#define BUTTON_ADVANCE_PIN PORTFbits.RF8
#define BUTTON_NEXT_PIN PORTAbits.RA7
#define BUTTON_CANCEL_PIN PORTDbits.RD13

int flagSet_advance();
int flagSet_cancel();
int flagSet_next();
void clearFlag_advance();
void clearFlag_cancel();
void clearFlag_next();

#endif	/* BUTTONS_H */
