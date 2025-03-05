#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    // Apply gravity
    Vector2 acceleration = {0, GRAVITY};

    // Integrate half of the acceleration 
    p->velocity = Vector2Add(p->velocity, Vector2Scale(acceleration, dt * 0.5f));

    // Apply damping (air resistance or friction)
    p->velocity.x *= DAMPING_FACTOR;
    p->velocity.y *= DAMPING_FACTOR;

    // Integrate velocity into position (Semi-Implicit Euler Integration)
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;

    // Integrate the other half of velocity here
    p->velocity = Vector2Add(p->velocity, Vector2Scale(acceleration, dt * 0.5f));

    // Update all debug information
    p->info.position = p->position;
    p->info.velocity = p->velocity;
}

void Render(Particle* p){
    DrawCircleV(p->position, p->radius, p->color);
}

// Constructor Functions
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r){
    // Attributes
    p->position = pos;
    p->velocity = (Vector2){0, 0};
    p->color = c;
    p->mass = m;
    p->restitution = rest;
    p->radius = r;
    p->type = TYPE_PARTICLE;
    p->info = (DebugData){pos, p->velocity, c, r};

    // Functions
    p->Update = Update;
    p->Render = Render;
}

// General Function Definitions
void ConstrainParticle(Particle* p){
    if((p->position.x - p->radius) <= 0){
        p->position.x = 0 + p->radius;   // Update the X position
        p->velocity.x *= -1; // Reverse the velocity to make the ball "Bounce off of the wall"
    } else if ((p->position.x + p->radius) >= GetScreenWidth()){
        p->position.x = GetScreenWidth() - p->radius;   // Update the X position
        p->velocity.x *= -1; // Reverse the velocity to make the ball "Bounce off of the wall"
    }

    if((p->position.y - p->radius) <= 0){
        p->position.y = 0 + p->radius;   // Update the Y Position
        p->velocity.y *= -1; // Reverse the velocity to make the ball "Bounce off of the wall"
    } else if ((p->position.y + p->radius) >= GetScreenHeight()){
        p->position.y = GetScreenHeight() - p->radius;
        p->velocity.y *= -1;
    } else if((p->position.y + p->radius) >= GetScreenHeight()){
        p->velocity.x = 0;
        p->velocity.y = 0;
    }
}

void ResolveCollision(Particle* a, Particle* b){
    Vector2 normal = {b->position.x - a->position.x, b->position.y - a->position.y};
    float distance = sqrtf(normal.x * normal.x + normal.y * normal.y);

    if(distance == 0) return;
    normal.x /= distance;
    normal.y /= distance;

    Vector2 relativeVelocity = {b->velocity.x - a->velocity.x, b->velocity.y - a->velocity.y};;
    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

    if(velocityAlongNormal > 0) return;

    float e = fmin(a->restitution, b->restitution);
    float j = -(1 + e) * velocityAlongNormal / (1/a->mass + 1/b->mass);

    Vector2 impulse = {j * normal.x, j * normal.y};
    a->velocity.x -= impulse.x / a->mass;
    a->velocity.y -= impulse.y / a->mass;
    b->velocity.x += impulse.x / b->mass;
    b->velocity.y += impulse.y / b->mass;
}

bool ParticleVsParticle(Particle* a, Particle* b){
    float r = a->radius + b->radius;
    r *= r; // Squaring the sum of radii

    // Corrected distance squared formula
    float distance_squared = powf(b->position.x - a->position.x, 2) + powf(b->position.y - a->position.y, 2);

    return distance_squared < r; // Check if objects overlap
}