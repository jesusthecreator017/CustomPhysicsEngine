#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    //Vector2 velocity = Vector2Subtract(p->position, p->oldPosition);

    // Store the current position before updating
    Vector2 tempPosition = p->position;

    // Apply acceleration (gravity)
    Vector2 acceleration = {0, GRAVITY};  // Gravity acts on the y-axis

    // Verlet Integration Update
    p->position = Vector2Add(Vector2Subtract(Vector2Scale(p->position, 2), p->oldPosition), Vector2Scale(acceleration, dt*dt));

    // Update old position
    p->oldPosition = tempPosition;

}

void Render(Particle* p){
    DrawCircleV(p->position, p->radius, p->color);
}

// Constructor Functions
void InitParticle(Particle* p, Vector2 pos, Vector2 oldPos, Color c, float m, float rest, float r){
    // Attributes
    p->position = pos;
    p->oldPosition = oldPos;
    p->color = c;
    p->mass = m;
    p->restitution = rest;
    p->radius = r;
    p->type = TYPE_PARTICLE;
    p->info = (DebugData){pos, p->oldPosition, c, r};

    // Functions
    p->Update = Update;
    p->Render = Render;
}

// General Function Definitions
void ConstrainParticle(Particle* p){
    Vector2 velocity = Vector2Subtract(p->position, p->oldPosition);

    // Constrain X-axis
    if (p->position.x < 0) {
        p->position.x = 0;
        p->oldPosition.x = p->position.x - velocity.x;  // Reverse velocity
    }

    if (p->position.x > GetScreenWidth()) {
        p->position.x = GetScreenWidth();
        p->oldPosition.x = p->position.x + velocity.x;  // Reverse velocity
    }

    // Constrain Y-axis
    if (p->position.y < 0) {
        p->position.y = 0;
        p->oldPosition.y = p->position.y - velocity.y;  // Reverse velocity
    }

    if (p->position.y > GetScreenHeight()) {
        p->position.y = GetScreenHeight();
        p->oldPosition.y = p->position.y + velocity.y;  // Reverse velocity
    }
}

bool ParticleVsParticle(Particle* a, Particle* b){
    float r = a->radius + b->radius;
    r *= r; // Squaring the sum of radii

    // Corrected distance squared formula
    float distance_squared = powf(b->position.x - a->position.x, 2) + powf(b->position.y - a->position.y, 2);

    return distance_squared < r; // Check if objects overlap
}