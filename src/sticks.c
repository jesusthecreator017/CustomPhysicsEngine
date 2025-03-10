#include "sticks.h"

// Draws and renders the stick, including the starting position and color
void RenderStick(Stick* s){
    DrawLineEx(s->particlePositions[0]->position, s->particlePositions[1]->position, 2.0f, BLACK);
}

// Creates a stick within a given length and where it starts/end
void InitStick(Stick* s, Particle* p1, Particle* p2, float maxLen, float minLen){
    // Attributes
    s->particlePositions[0] = p1;
    s->particlePositions[1] = p2;
    s->length = maxLen;
    s->minLength = minLen;

    s->Update = UpdateStick;
    s->Render = RenderStick;
}

// And update the UpdateStick function
void UpdateStick(Stick* s){
    // Ignore Momentarily
    Vector2 axis = Vector2Subtract(s->particlePositions[1]->position, s->particlePositions[0]->position);
    float distance = Vector2Length(axis);
    Vector2 distanceVector = {distance, distance};
    Vector2 n = Vector2Divide(axis, distanceVector);
    float delta = s->length - distance;

    if (!s->particlePositions[0]->isPinned){
        s->particlePositions[0]->position.x -= 0.5f * n.x * delta;
        s->particlePositions[0]->position.y -= 0.5f * n.y * delta;
    }
    
    if(!s->particlePositions[1]->isPinned){
        s->particlePositions[1]->position.x += 0.5f * n.x * delta;
        s->particlePositions[1]->position.y += 0.5f * n.y * delta;
    }
}