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
    {  .name="Delta",          .neighbors={ T_ALPHA, T_GAMMA}, .cardtype=INFANTRY,  .owner=-1,  .troops=0 },
};
Continent continents[NUM_CONTINENTS] = {};
int territoryScrollOrder[NUM_TERRITORIES] =
{
    T_ALPHA,
    T_BETA,
    T_GAMMA,
    T_DELTA,
};
#else
Territory territories[NUM_TERRITORIES] =
{
    [T_ALASKA] =                {  .name="Alaska",            .neighbors={ T_NORTHWEST, T_ALBERTA, T_KAMCHATKA, -1, -1, -1},                                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_NORTHWEST] =             {  .name="NW Territory",      .neighbors={ T_ALASKA, T_ALBERTA, T_ONTARIO, T_GREENLAND, -1, -1},                                         .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_GREENLAND] =             {  .name="Greenland",         .neighbors={ T_NORTHWEST, T_ONTARIO, T_QUEBEC, T_ICELAND, -1, -1},                                         .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_ALBERTA] =               {  .name="Alberta",           .neighbors={ T_ALASKA, T_NORTHWEST, T_ONTARIO, T_WESTERN_UNITED_STATES, -1, -1},                           .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_ONTARIO] =               {  .name="Ontario",           .neighbors={ T_NORTHWEST, T_GREENLAND, T_QUEBEC, T_WESTERN_UNITED_STATES, T_EASTERN_UNITED_STATES, T_ALBERTA},    .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_QUEBEC] =                {  .name="Quebec",            .neighbors={ T_ONTARIO, T_GREENLAND, T_EASTERN_UNITED_STATES, -1, -1, -1},                                 .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_WESTERN_UNITED_STATES] = {  .name="Western US",        .neighbors={ T_ALBERTA, T_ONTARIO, T_EASTERN_UNITED_STATES, T_MEXICO, -1, -1},                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_EASTERN_UNITED_STATES] = {  .name="Eastern US",        .neighbors={ T_WESTERN_UNITED_STATES, T_ONTARIO, T_QUEBEC, T_MEXICO, -1, -1},                              .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_MEXICO] =                {  .name="Mexico",            .neighbors={ T_WESTERN_UNITED_STATES, T_EASTERN_UNITED_STATES, T_VENEZUELA, -1, -1, -1},                   .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_VENEZUELA] =             {  .name="Venezuela",         .neighbors={ T_MEXICO, T_PERU, T_BRAZIL, -1, -1, -1},                                                      .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_PERU] =                  {  .name="Peru",              .neighbors={ T_VENEZUELA, T_BRAZIL, T_ARGENTINA, -1, -1, -1},                                              .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_ARGENTINA] =             {  .name="Argentina",         .neighbors={ T_PERU, T_BRAZIL, -1, -1, -1, -1},                                                            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_BRAZIL] =                {  .name="Brazil",            .neighbors={ T_VENEZUELA, T_PERU, T_ARGENTINA, T_NORTHERN_AFRICA, -1, -1},                                 .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_ICELAND] =               {  .name="Iceland",           .neighbors={ T_GREENLAND, T_SCANDINAVIA, T_GREAT_BRITAIN, -1, -1, -1},                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_SCANDINAVIA] =           {  .name="Scandinavia",       .neighbors={ T_ICELAND, T_UKRAINE, T_NORTHERN_EUROPE, T_GREAT_BRITAIN, -1, -1},                            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_GREAT_BRITAIN] =         {  .name="Great Britain",     .neighbors={ T_ICELAND, T_SCANDINAVIA, T_NORTHERN_EUROPE, T_WESTERN_EUROPE, -1, -1},                       .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_NORTHERN_EUROPE] =       {  .name="Northern Europe",   .neighbors={ T_SCANDINAVIA, T_UKRAINE, T_SOUTHERN_EUROPE, T_WESTERN_EUROPE, -1, -1},                       .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_WESTERN_EUROPE] =        {  .name="Western Europe",    .neighbors={ T_GREAT_BRITAIN, T_NORTHERN_EUROPE, T_SOUTHERN_EUROPE, T_NORTHERN_AFRICA, -1, -1},            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_SOUTHERN_EUROPE] =       {  .name="Southern Europe",   .neighbors={ T_WESTERN_EUROPE, T_NORTHERN_EUROPE, T_UKRAINE, T_MIDDLE_EAST, T_EGYPT, T_NORTHERN_AFRICA},   .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_UKRAINE] =               {  .name="Ukraine",           .neighbors={ T_SCANDINAVIA, T_URALS, T_AFGHANISTAN, T_MIDDLE_EAST, T_SOUTHERN_EUROPE, T_NORTHERN_EUROPE},  .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_NORTHERN_AFRICA] =       {  .name="Northern Africa",   .neighbors={ T_WESTERN_EUROPE, T_SOUTHERN_EUROPE, T_EGYPT, T_EASTERN_AFRICA, T_CONGO, T_BRAZIL},           .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_EGYPT] =                 {  .name="Egypt",             .neighbors={ T_SOUTHERN_EUROPE, T_MIDDLE_EAST, T_EASTERN_AFRICA, T_NORTHERN_AFRICA, -1, -1},               .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_CONGO] =                 {  .name="Congo",             .neighbors={ T_NORTHERN_AFRICA, T_EASTERN_AFRICA, T_SOUTH_AFRICA, -1, -1, -1},                             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_EASTERN_AFRICA] =        {  .name="Eastern Africa",    .neighbors={ T_EGYPT, T_MIDDLE_EAST, T_MADAGASCAR, T_SOUTH_AFRICA, T_CONGO, T_NORTHERN_AFRICA},            .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_SOUTH_AFRICA] =          {  .name="South Africa",      .neighbors={ T_CONGO, T_EASTERN_AFRICA, T_MADAGASCAR, -1, -1, -1},                                         .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_MADAGASCAR] =            {  .name="Madagascar",        .neighbors={ T_SOUTH_AFRICA, T_EASTERN_AFRICA, -1, -1, -1, -1},                                            .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_URALS] =                 {  .name="Ural",              .neighbors={ T_UKRAINE, T_SIBERIA, T_CHINA, T_AFGHANISTAN, -1, -1},                                        .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_SIBERIA] =               {  .name="Siberia",           .neighbors={ T_YAKUTSK, T_IRKUTSK, T_MONGOLIA, T_CHINA, T_URALS, -1},                                      .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_YAKUTSK] =               {  .name="Yakutsk",           .neighbors={ T_KAMCHATKA, T_IRKUTSK, T_SIBERIA, -1, -1, -1},                                               .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_KAMCHATKA] =             {  .name="Kamchatka",         .neighbors={ T_ALASKA, T_IRKUTSK, T_YAKUTSK, T_MONGOLIA, T_JAPAN, -1},                                     .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_IRKUTSK] =               {  .name="Irkutsk",           .neighbors={ T_SIBERIA, T_YAKUTSK, T_KAMCHATKA, T_MONGOLIA, T_CHINA, -1},                                  .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_AFGHANISTAN] =           {  .name="Afghanistan",       .neighbors={ T_UKRAINE, T_URALS, T_CHINA, T_INDIA, T_MIDDLE_EAST, -1},                                     .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_JAPAN] =                 {  .name="Japan",             .neighbors={ T_KAMCHATKA, T_MONGOLIA, -1, -1, -1, -1},                                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_MONGOLIA] =              {  .name="Mongolia",          .neighbors={ T_IRKUTSK, T_KAMCHATKA, T_JAPAN, T_CHINA, T_SIBERIA, -1},                                     .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_MIDDLE_EAST] =           {  .name="Middle East",       .neighbors={ T_SOUTHERN_EUROPE, T_UKRAINE, T_AFGHANISTAN, T_INDIA, T_EASTERN_AFRICA, T_EGYPT},             .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_CHINA] =                 {  .name="China",             .neighbors={ T_AFGHANISTAN, T_SIBERIA, T_URALS, T_MONGOLIA, T_SIAM, T_INDIA},                              .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_INDIA] =                 {  .name="India",             .neighbors={ T_MIDDLE_EAST, T_AFGHANISTAN, T_CHINA, T_SIAM, -1, -1},                                       .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_SIAM] =                  {  .name="Siam",              .neighbors={ T_INDIA, T_CHINA, T_INDONESIA, -1, -1, -1},                                                   .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_INDONESIA] =             {  .name="Indonesia",         .neighbors={ T_SIAM, T_NEW_GUINEA, T_WESTERN_AUSTRALIA, -1, -1, -1},                                       .cardtype=INFANTRY,   .owner=-1,  .troops=0 },
    [T_NEW_GUINEA] =            {  .name="New Guinea",        .neighbors={ T_INDONESIA, T_WESTERN_AUSTRALIA, T_EASTERN_AUSTRALIA, -1, -1, -1},                           .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
    [T_WESTERN_AUSTRALIA] =     {  .name="W Australia",       .neighbors={ T_INDONESIA, T_NEW_GUINEA, T_EASTERN_AUSTRALIA, -1, -1, -1},                                  .cardtype=ARTILLERY,  .owner=-1,  .troops=0 },
    [T_EASTERN_AUSTRALIA] =     {  .name="E Australia",       .neighbors={ T_WESTERN_AUSTRALIA, T_NEW_GUINEA, -1, -1, -1, -1},                                           .cardtype=CAVALRY,    .owner=-1,  .troops=0 },
};
Continent continents[NUM_CONTINENTS] = 
{
    { .name="North America", .memberCount=9,  .members={T_ALASKA, T_NORTHWEST, T_GREENLAND, T_ALBERTA, T_ONTARIO, T_QUEBEC, T_WESTERN_UNITED_STATES, T_EASTERN_UNITED_STATES, T_MEXICO, -1, -1, -1}, .value=5 },
    { .name="South America", .memberCount=4,  .members={T_VENEZUELA, T_BRAZIL, T_PERU, T_ARGENTINA, -1, -1, -1, -1, -1, -1, -1, -1}, .value=2 },
    { .name="Europe",        .memberCount=7,  .members={T_ICELAND, T_SCANDINAVIA, T_UKRAINE, T_GREAT_BRITAIN, T_NORTHERN_EUROPE, T_WESTERN_EUROPE, T_SOUTHERN_EUROPE, -1, -1, -1, -1, -1}, .value=5 },
    { .name="Africa",        .memberCount=6,  .members={T_NORTHERN_AFRICA, T_EGYPT, T_EASTERN_AFRICA, T_CONGO, T_SOUTH_AFRICA, T_MADAGASCAR, -1, -1, -1, -1, -1, -1}, .value=3 },
    { .name="Asia",          .memberCount=12, .members={T_URALS, T_SIBERIA, T_IRKUTSK, T_KAMCHATKA, T_YAKUTSK, T_MONGOLIA, T_AFGHANISTAN, T_MIDDLE_EAST, T_INDIA, T_CHINA, T_SIAM, T_JAPAN}, .value=7 },
    { .name="Australia",     .memberCount=4,  .members={T_INDONESIA, T_NEW_GUINEA, T_WESTERN_AUSTRALIA, T_EASTERN_AUSTRALIA, -1, -1, -1, -1, -1, -1, -1, -1}, .value=2 },
};
int territoryScrollOrder[NUM_TERRITORIES] =
{
    T_ALASKA,
    T_NORTHWEST,
    T_GREENLAND,
    T_ALBERTA,
    T_ONTARIO,
    T_QUEBEC,
    T_WESTERN_UNITED_STATES,
    T_EASTERN_UNITED_STATES,
    T_MEXICO,
    T_VENEZUELA,
    T_PERU,
    T_BRAZIL,
    T_ARGENTINA,
    T_ICELAND,
    T_SCANDINAVIA,
    T_GREAT_BRITAIN,
    T_NORTHERN_EUROPE,
    T_UKRAINE,
    T_WESTERN_EUROPE,
    T_SOUTHERN_EUROPE,
    T_NORTHERN_AFRICA,
    T_EGYPT,
    T_CONGO,
    T_EASTERN_AFRICA,
    T_SOUTH_AFRICA,
    T_MADAGASCAR,
    T_URALS,
    T_SIBERIA,
    T_YAKUTSK,
    T_KAMCHATKA,
    T_IRKUTSK,
    T_AFGHANISTAN,
    T_MONGOLIA,
    T_JAPAN,
    T_MIDDLE_EAST,
    T_CHINA,
    T_INDIA,
    T_SIAM,
    T_INDONESIA,
    T_NEW_GUINEA,
    T_WESTERN_AUSTRALIA,
    T_EASTERN_AUSTRALIA,
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
