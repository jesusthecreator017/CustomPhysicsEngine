#ifndef DEBUG_H
#define DEBUG_H

#include "main.h"

typedef enum {
    TYPE_PARTICLE,
    TYPE_AABB
} ObjectType;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    Color color;
    float radius;
} DebugData;

void ObjectInfo(DebugData data, ObjectType type);

#endif
