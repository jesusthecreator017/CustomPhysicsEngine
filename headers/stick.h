#ifndef STICK_H
#define STICK_H

#include "main.h"

#define PAIR 2

typedef struct stick{
    // Attributes
    Vector2 particlePositions[PAIR];
    float length;

    // Methods
    void (*Update)(struct stick* s, float dt);
    void (*Render)(struct stick* s);
}Stick;

#endif