#include "sticks.h"

void UpdateStick(Stick* s){
    float dx = s->particlePositions[1]->position.x - s->particlePositions[0]->position.x;
    float dy = s->particlePositions[1]->position.y - s->particlePositions[0]->position.y;
    float distance = Vector2Distance(s->particlePositions[0]->position, s->particlePositions[1]->position);
    float diff = s->length - distance;
    float percent = (diff / distance) / 2;

    Vector2 offset = Vector2Scale((Vector2){dx, dy}, percent);

    if (!s->particlePositions[0]->isPinned){
        s->particlePositions[0]->position.x -= offset.x; // Now modifies the actual particle positions
        s->particlePositions[0]->position.y -= offset.y;
    }
    
    if(!s->particlePositions[1]->isPinned){
        s->particlePositions[1]->position.x += offset.x;
        s->particlePositions[1]->position.y += offset.y;
    }
}


void RenderStick(Stick* s){
    DrawLineEx(s->particlePositions[0]->position, s->particlePositions[1]->position, 2.0f, BLACK);
}

void InitStick(Stick* s, Particle* p1, Particle* p2, float len){
    // Attributes
    s->particlePositions[0] = p1;
    s->particlePositions[1] = p2;
    s->length = len;

    s->Update = UpdateStick;
    s->Render = RenderStick;
}