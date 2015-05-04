// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#ifndef PI_H
#define	PI_H

#include "gamelogic.h"

#define PI_DATA_LENGTH (NUM_TERRITORIES + NUM_CARDS)
#define PI_COMMAND_LENGTH 4

extern unsigned char piData[PI_DATA_LENGTH];
extern unsigned char piCommand[PI_COMMAND_LENGTH];

void updatePiData();

int flagSetPiCommand();
void clearFlagPiCommand();

#endif	/* PI_H */

