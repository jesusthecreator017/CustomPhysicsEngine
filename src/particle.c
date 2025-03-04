#include "particle.h"

void initializeParticles(Particle* particles, int numParticles){
    for(int i = 0; i < numParticles; i++){
        // Vectors 
        particles[i].position = (Vector2){GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight())};
        particles[i].velocity = (Vector2){0, 0};
        particles[i].acceleration = (Vector2){0, 0};

        // Color
        particles[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};

        // Floats
        particles[i].radius = GetRandomValue(1, 20);
        particles[i].mass = (0.75 * particles[i].radius);
    }
}

void updateParticles(Particle* particles, int numParticles, float dt){
    for(int i = 0; i < numParticles; i++){
        // Calculate Acceleration
        Particle* particle = &particles[i];
        Vector2 force = computeForce(particle);
        particle->acceleration = (Vector2){force.x / particle->mass, force.y / particle->mass};
        
        // Apply Physics
        particle->position.x += particle->velocity.x * dt;
        particle->position.y += particle->velocity.y * dt;  

        particle->velocity.x += particle->acceleration.x * dt;
        particle->velocity.y += particle->acceleration.y * dt;
    }
}
void renderParticles(Particle* particles, int numParticles){
    for(int i = 0; i < numParticles; i++){
        Particle* particle = &particles[i];
       DrawCircle(particle->position.x, particle->position.y, particle->radius, particle->color); 
    }
    
}

void constrainParticles(Particle* particles, int numParticles){
    for(int i = 0; i < numParticles; i++){
        Particle* particle = & particles[i];
        if((particle->position.x - (particle->radius / 2)) < 0 || (particle->position.x + (particle->radius / 2)) > GetScreenWidth()){
            particle->velocity.x *= -1;
        }
        if((particle->position.y - (particle->radius / 2)) < 0 || (particle->position.y + (particle->radius / 2)) > GetScreenHeight()){
            particle->velocity.y *= -1;
        }
    }
}

// Forces
Vector2 computeForce(Particle* p1){
    return (Vector2){0, p1->mass * 9.81f};
}