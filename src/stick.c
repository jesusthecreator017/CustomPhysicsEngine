#include "stick.h"


// Struct Methods
void UpdateStick(Stick* s, float dt);

void RenderStick(Stick* s){
    DrawLineV(s->particlePositions[0], s->particlePositions[1], BLACK);
}

// Constructor
void InitStick(Stick* s, Vector2 pos1, Vector2 pos2, float len){
    s->particlePositions[0] = pos1;
    s->particlePositions[1] = pos2;
    s->length = len;
}