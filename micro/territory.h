#ifndef TERRITORY_H
#define TERRITORY_H
// territory.h:
// Territory structure and individual territory functions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#define MAX_NEIGHBORS 6
typedef struct
{
    const char * name;
    const int neighbors[MAX_NEIGHBORS];

    int owner;
    int troops;
} Territory;

#endif
