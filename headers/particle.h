#ifndef PARTICLE_H
#define PARTICLE_H

#include "main.h"

typedef struct{
    // Vectors
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    // Color
    Color color;

    // Floats
    float radius;
    float mass;
}Particle;

void initializeParticles(Particle* particles, int numParticles);
void updateParticles(Particle* particles, int numParticles, float dt);
void renderParticles(Particle* particles, int numParticles);
void constrainParticles(Particle* particles, int numParticles);

// Force Computation
Vector2 computeForce(Particle* p1);

#endif