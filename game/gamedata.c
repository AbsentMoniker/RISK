// gamedata.c:
// Game constants and territory graph layout.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015
//
// a webpage covering various rules clarifications and variants:
// http://www.kent.ac.uk/smsas/personal/odl/riskfaq.htm

#include "gamedata.h"

#ifdef USE_SIMPLE_MAP
Territory territories[NUM_TERRITORIES] =
{
    {  .name="Alpha",          .neighbors={ T_BETA, T_DELTA},  .cardtype=INFANTRY,  .owner=-1,  .troops=0 },
    {  .name="Beta",           .neighbors={ T_GAMMA, T_ALPHA}, .cardtype=INFANTRY,  .owner=-1,  .troops=0 },
    {  .name="Gamma",          .neighbors={ T_DELTA, T_BETA},  .cardtype=INFANTRY,  .owner=-1,  .troops=0 },
    {  .name="Delta",          .neighbors={ T_ALPHA, T_DELTA}, .cardtype=INFANTRY,  .owner=-1,  .troops=0 },
};
Continent continents[NUM_CONTINENTS] = {};
#else
Territory territories[NUM_TERRITORIES] =
{
    {  .name="Alaska",                 .neighbors={ T_NORTHWEST, T_ALBERTA, T_KAMCHATKA, -1, -1, -1},                                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="NW Territory",           .neighbors={ T_ALASKA, T_ALBERTA, T_ONTARIO, T_GREENLAND, -1, -1},                                         .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Greenland",              .neighbors={ T_NORTHWEST, T_ONTARIO, T_QUEBEC, T_ICELAND, -1, -1},                                         .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Alberta",                .neighbors={ T_ALASKA, T_NORTHWEST, T_ONTARIO, T_WESTERN_UNITED_STATES, -1, -1},                           .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Ontario",                .neighbors={ T_NORTHWEST, T_GREENLAND, T_QUEBEC, T_WESTERN_UNITED_STATES, T_EASTERN_UNITED_STATES, -1},    .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Quebec",                 .neighbors={ T_ONTARIO, T_GREENLAND, T_EASTERN_UNITED_STATES, -1, -1, -1},                                 .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Western US",             .neighbors={ T_ALBERTA, T_ONTARIO, T_EASTERN_UNITED_STATES, T_MEXICO, -1, -1},                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Eastern US",             .neighbors={ T_WESTERN_UNITED_STATES, T_ONTARIO, T_QUEBEC, T_MEXICO, -1, -1},                              .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Mexico",                 .neighbors={ T_WESTERN_UNITED_STATES, T_EASTERN_UNITED_STATES, T_VENEZUELA, -1, -1, -1},                   .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Venezeula",              .neighbors={ T_MEXICO, T_PERU, T_BRAZIL, -1, -1, -1},                                                      .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Peru",                   .neighbors={ T_VENEZUELA, T_BRAZIL, T_ARGENTINA, -1, -1, -1},                                              .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Argentina",              .neighbors={ T_PERU, T_BRAZIL, -1, -1, -1, -1},                                                            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Brazil",                 .neighbors={ T_VENEZUELA, T_PERU, T_ARGENTINA, T_NORTHERN_AFRICA, -1, -1},                                 .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Iceland",                .neighbors={ T_GREENLAND, T_SCANDINAVIA, T_GREAT_BRITAIN, -1, -1, -1},                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Scandinavia",            .neighbors={ T_ICELAND, T_UKRAINE, T_NORTHERN_EUROPE, T_GREAT_BRITAIN, -1, -1},                            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Great Britain",          .neighbors={ T_ICELAND, T_SCANDINAVIA, T_NORTHERN_EUROPE, T_WESTERN_EUROPE, -1, -1},                       .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Northern Europe",        .neighbors={ T_SCANDINAVIA, T_UKRAINE, T_SOUTHERN_EUROPE, T_WESTERN_EUROPE, -1, -1},                       .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Western Europe",         .neighbors={ T_GREAT_BRITAIN, T_NORTHERN_EUROPE, T_SOUTHERN_EUROPE, T_NORTHERN_AFRICA, -1, -1},            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Southern Europe",        .neighbors={ T_WESTERN_EUROPE, T_NORTHERN_EUROPE, T_UKRAINE, T_MIDDLE_EAST, T_EGYPT, T_NORTHERN_AFRICA},   .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Ukraine",                .neighbors={ T_SCANDINAVIA, T_URALS, T_AFGHANISTAN, T_MIDDLE_EAST, T_SOUTHERN_EUROPE, T_NORTHERN_EUROPE},  .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Northern Africa",        .neighbors={ T_WESTERN_EUROPE, T_SOUTHERN_EUROPE, T_EGYPT, T_EASTERN_AFRICA, T_CONGO, T_BRAZIL},           .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Egypt",                  .neighbors={ T_SOUTHERN_EUROPE, T_MIDDLE_EAST, T_EASTERN_AFRICA, T_NORTHERN_AFRICA, -1, -1},               .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Congo",                  .neighbors={ T_NORTHERN_AFRICA, T_EASTERN_AFRICA, T_SOUTH_AFRICA, -1, -1, -1},                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Eastern Africa",         .neighbors={ T_EGYPT, T_MIDDLE_EAST, T_MADAGASCAR, T_SOUTH_AFRICA, T_CONGO, T_NORTHERN_AFRICA},            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="South Africa",           .neighbors={ T_CONGO, T_EASTERN_AFRICA, T_MADAGASCAR, -1, -1, -1},                                         .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Madagascar",             .neighbors={ T_SOUTH_AFRICA, T_EASTERN_AFRICA, -1, -1, -1, -1},                                            .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Urals",                  .neighbors={ T_UKRAINE, T_SIBERIA, T_CHINA, T_AFGHANISTAN, -1, -1},                                        .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Siberia",                .neighbors={ T_YAKUTSK, T_IRKUTSK, T_MONGOLIA, T_CHINA, T_URALS, -1},                                      .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Yakutsk",                .neighbors={ T_KAMCHATKA, T_IRKUTSK, T_SIBERIA, -1, -1, -1},                                               .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Kamchatka",              .neighbors={ T_ALASKA, T_IRKUTSK, T_YAKUTSK, T_MONGOLIA, T_JAPAN, -1},                                     .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Irkutsk",                .neighbors={ T_SIBERIA, T_YAKUTSK, T_KAMCHATKA, T_MONGOLIA, T_CHINA, -1},                                  .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Afghanistan",            .neighbors={ T_UKRAINE, T_URALS, T_CHINA, T_INDIA, T_MIDDLE_EAST, -1},                                     .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="Japan",                  .neighbors={ T_KAMCHATKA, T_MONGOLIA, -1, -1, -1, -1},                                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Mongolia",               .neighbors={ T_IRKUTSK, T_KAMCHATKA, T_JAPAN, T_CHINA, T_SIBERIA, -1},                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Middle East",            .neighbors={ T_SOUTHERN_EUROPE, T_UKRAINE, T_AFGHANISTAN, T_INDIA, T_EASTERN_AFRICA, T_EGYPT},             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="China",                  .neighbors={ T_AFGHANISTAN, T_SIBERIA, T_URALS, T_MONGOLIA, T_SIAM, T_INDIA},                              .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="India",                  .neighbors={ T_MIDDLE_EAST, T_AFGHANISTAN, T_CHINA, T_SIAM, -1, -1},                                       .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Siam",                   .neighbors={ T_INDIA, T_CHINA, T_INDONESIA, -1, -1, -1},                                                   .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Indonesia",              .neighbors={ T_SIAM, T_NEW_GUINEA, T_WESTERN_AUSTRAILIA, -1, -1, -1},                                      .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    {  .name="New Guinea",             .neighbors={ T_INDONESIA, T_WESTERN_AUSTRAILIA, T_EASTERN_AUSTRAILIA, -1, -1, -1},                         .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    {  .name="Western Aus.",           .neighbors={ T_INDONESIA, T_NEW_GUINEA, T_EASTERN_AUSTRAILIA, -1, -1, -1},                                 .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    {  .name="Eastern Aus.",           .neighbors={ T_WESTERN_AUSTRAILIA, T_NEW_GUINEA, -1, -1, -1, -1},                                          .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
};
Continent continents[NUM_CONTINENTS] = 
{
    { .name="North America", .firstmember=T_ALASKA, .members=9, .value=5 },
    { .name="South America", .firstmember=T_VENEZUELA, .members=4, .value=2 },
    { .name="Europe", .firstmember=T_ICELAND, .members=7, .value=5 },
    { .name="Africa", .firstmember=T_NORTHERN_AFRICA, .members=6, .value=3 },
    { .name="Asia", .firstmember=T_URALS, .members=12, .value=7 },
    { .name="Austrailia", .firstmember=T_INDONESIA, .members=4, .value=2 },
};
#endif

const int cardExchangeValues[NUM_EXCHANGE_VALUES] = 
{ 4, 6, 8, 10, 12, 15};

#ifdef USE_SIMPLE_MAP
const int initialTroops[MAX_PLAYERS + 1] =
{ 0, 0, 7};
#else
const int initialTroops[MAX_PLAYERS + 1] = 
{ 0, 0, 40, 35, 30, 25, 20};
#endif
