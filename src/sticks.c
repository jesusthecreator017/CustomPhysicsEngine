#include "sticks.h"

void UpdateStick(Stick* s){
    float dx = s->particlePositions[1]->x - s->particlePositions[0]->x;
    float dy = s->particlePositions[1]->y - s->particlePositions[0]->y;
    float distance = Vector2Distance(*s->particlePositions[0], *s->particlePositions[1]);
    float diff = s->length - distance;
    float percent = (diff / distance) / 2;

    Vector2 offset = Vector2Scale((Vector2){dx, dy}, percent);

    s->particlePositions[0]->x -= offset.x; // Now modifies the actual particle positions
    s->particlePositions[0]->y -= offset.y;

    s->particlePositions[1]->x += offset.x;
    s->particlePositions[1]->y += offset.y;
}


void RenderStick(Stick* s){
    DrawLineEx(*s->particlePositions[0], *s->particlePositions[1], 2.0f, BLACK);
}

void InitStick(Stick* s, Vector2* pos1, Vector2* pos2, float len){
    // Attributes
    s->particlePositions[0] = pos1;
    s->particlePositions[1] = pos2;
    s->length = len;

    s->Update = UpdateStick;
    s->Render = RenderStick;
}