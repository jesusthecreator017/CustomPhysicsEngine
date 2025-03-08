#include "sticks.h"


void RenderStick(Stick* s){
    DrawLineEx(s->particlePositions[0]->position, s->particlePositions[1]->position, 2.0f, BLACK);
}

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
    float dx = s->particlePositions[1]->position.x - s->particlePositions[0]->position.x;
    float dy = s->particlePositions[1]->position.y - s->particlePositions[0]->position.y;
    float distance = sqrtf(dx*dx + dy*dy);
    
    if (distance == 0) return; // Prevent division by zero
    
    float diff;
    float percent;
    
    // Apply the appropriate constraint
    if (distance > s->length) {
        // Too far - pull particles closer
        diff = s->length - distance;
        percent = (diff / distance) / 2;
    }
    else if (distance < s->minLength) {
        // Too close - push particles apart
        diff = s->minLength - distance;
        percent = (diff / distance) / 2;
    }
    else {
        // Within allowed range - do nothing
        return;
    }
    
    // Add damping factor
    float dampingFactor = 0.95;
    percent *= dampingFactor;
    
    Vector2 offset = {dx * percent, dy * percent};

    if (!s->particlePositions[0]->isPinned){
        s->particlePositions[0]->position.x -= offset.x;
        s->particlePositions[0]->position.y -= offset.y;
    }
    
    if(!s->particlePositions[1]->isPinned){
        s->particlePositions[1]->position.x += offset.x;
        s->particlePositions[1]->position.y += offset.y;
    }
}