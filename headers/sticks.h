#ifndef STICKS_H
#define STICKS_H

#include "particle.h"

#define PAIR 2

typedef struct stick{
    // Attributes
    Particle* particlePositions[PAIR];
    float length;
    float minLength;

    // Methods
    void (*Update)(struct stick* s);
    void (*Render)(struct stick* s);
}Stick;

void UpdateStick(Stick* s);
void RenderStick(Stick* s);

void InitStick(Stick* s, Particle* p1, Particle* p2, float len, float minLen);

#endif