// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.

#ifndef FLASH_H
#define	FLASH_H

void readSaveFromFlash();
void writeSaveToFlash();
void eraseFlash();
void initiateFlashOp();
int flashIsErased();

#endif	/* FLASH_H */

