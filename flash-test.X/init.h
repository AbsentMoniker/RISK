/* 
 * File:   init.h
 * Author: jacob
 *
 * Created on February 19, 2015, 6:30 PM
 */

#ifndef INIT_H
#define	INIT_H

void initInterrupts();
void initClocks();
void initPorts();
void initTimers();
void initSPI();
void initRNG();
void seedRNG();

#endif	/* INIT_H */
