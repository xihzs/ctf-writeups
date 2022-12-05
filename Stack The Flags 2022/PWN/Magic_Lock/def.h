#pragma once


// Spells format
typedef struct {
    unsigned short size;
    short dmg;
    char name[12];
} Fireball, *pFireball;


typedef struct {
    short dmg;
    short distance;
    char name[12];
} Telekenisis, *pTelekenisis;

