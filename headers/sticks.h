#ifndef STICKS_H
#define STICKS_H

#include "main.h"

#define PAIR 2

typedef struct stick{
    // Attributes
    Vector2* particlePositions[PAIR];
    float length;

    // Methods
    void (*Update)(struct stick* s);
    void (*Render)(struct stick* s);
}Stick;

void UpdateStick(Stick* s);
void RenderStick(Stick* s);

void InitStick(Stick* s, Vector2* pos1, Vector2* pos2, float len);

#endif