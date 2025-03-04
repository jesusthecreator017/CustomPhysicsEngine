#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    // Apply the effects of gravity on the velocity of the ball
    p->velocity.y += GRAVITY * dt;

    // Apply the rest of the physics
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;
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

    // Functions
    p->Update = Update;
    p->Render = Render;
}

// General Function Definitions
void ConstrainParticle(Particle* p){
    if((p->position.x - p->radius / 2) < 0 || (p->position.x + p->radius / 2) > GetScreenWidth()){
        p->velocity.x *= -1; // Reverse the velocity to make the ball "Bounce off of the wall"
    }
    if((p->position.y - p->radius / 2) < 0 || (p->position.y + p->radius / 2) > GetScreenHeight()){
        p->velocity.y *= -1; // Reverse the velocity to make the ball "Bounce off of the wall"
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
    float deltaX = b->position.x - a->position.x;
    float deltaY = b->position.y - a->position.y;
    float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
    return distance < (a->radius + b->radius);
}