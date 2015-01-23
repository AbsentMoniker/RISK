#ifndef TERRITORY_H
#define TERRITORY_H
// territory.h:
// Territory structure and individual territory functions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

typedef enum
{
    INFANTRY, CAVALRY, ARTILLERY, WILD,
} CardType;

#define MAX_NEIGHBORS 6
typedef struct
{
    const char * name;
    const int neighbors[MAX_NEIGHBORS];
    const CardType cardtype;

    int owner;
    int troops;
} Territory;

#endif
