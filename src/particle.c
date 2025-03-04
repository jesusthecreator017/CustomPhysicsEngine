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