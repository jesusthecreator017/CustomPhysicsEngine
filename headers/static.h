#ifndef STATIC_H
#define STATIC_H

#include "particle.h"

typedef struct staticobject{
    // Attributes
    Vector2 position;
    ObjectType type;
    union {
        struct { Vector2 size; } Rect; // Width, Height
        struct { float radius; } Circle;
    }Shape;
    DebugData info;
    Color color;

    // Methods
    void (*Render)(struct staticobject* s);
}StaticObject;

// "Object" Functions
void RenderStaticObject(StaticObject* s);

// Initilization Function
// Size and R are optional depending on what shape you want for the static object
void InitStaticObject(StaticObject* s, Vector2 pos, ObjectType type, Color c, Vector2 size, float r);

// Collision detection
bool StaticObjectvsParticle(StaticObject* s, Particle* p);

// Collision Resolution


#endif