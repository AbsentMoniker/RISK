#ifndef PI_H
#define	PI_H

#include "gamelogic.h"

#define PI_DATA_LENGTH (NUM_TERRITORIES + NUM_CARDS)

extern unsigned char piData[PI_DATA_LENGTH];

void updatePiData();

#endif	/* PI_H */

