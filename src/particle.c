#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    // Store current position before updating it
    Vector2 temp = p->position;

    // Calculate the acceleration
    Vector2 acceleration = {0, GRAVITY};

    // Verlet Integration
    p->position = Vector2Add(Vector2Subtract(Vector2Scale(p->position, 2), p->oldPosition), Vector2Scale(acceleration, dt*dt));

    // Update the old position
    p->oldPosition = temp;
}

void Render(Particle* p){
    DrawCircleV(p->position, p->radius, p->color);
}

// Constructor Functions
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r){
    // Attributes
    p->position = pos;
    p->oldPosition = pos;
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

    // X-Axis Constraints
    if(p->position.x - p->radius < 0){
        p->position.x = 0 + p->radius;
        p->oldPosition.x = p->position.x + velocity.x * p->restitution;
    } else if(p->position.x + p->radius > GetScreenWidth()){
        p->position.x = GetScreenWidth() - p->radius;
        p->oldPosition.x = p->position.x + velocity.x * p->restitution;
    }

    // Y-Axis Constraints
    if(p->position.y - p->radius < 0){
        p->position.y = 0 + p->radius;
        p->oldPosition.y = p->position.y + velocity.y * p->restitution;
    } else if(p->position.y + p->radius > GetScreenHeight()){
        p->position.y = GetScreenHeight() - p->radius;
        p->oldPosition.y = p->position.y + velocity.y * p->restitution;
    }
}

bool ParticleVsParticle(Particle* a, Particle* b){
    float r = a->radius + b->radius;
    r *= r; // Squaring the sum of radii

    // Corrected distance squared formula
    float distance_squared = powf(b->position.x - a->position.x, 2) + powf(b->position.y - a->position.y, 2);

    return distance_squared < r; // Check if objects overlap
}