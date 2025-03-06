#ifndef PARTICLE_H
#define PARTICLE_H

// Includes
#include "debug.h"

// Particle "Class"
typedef struct particle{
    // Attributes
    Vector2 position;
    Vector2 oldPosition;
    Color color;
    float mass;
    float restitution;
    float radius;
    int isGrabbed;
    ObjectType type;
    DebugData info;

    // Functions (Simulating Classes)
    void (*Update)(struct particle* p, float dt);
    void (*Render)(struct particle* p);
} Particle;

// "Class" Function Prototypes
void Update(Particle* p, float dt);
void Render(Particle* p);

// Initial Constructor
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r);

// General Function Prototypes
void ConstrainParticle(Particle* p);
void ResolveCollision(Particle* a, Particle* b);
bool ParticleVsParticle(Particle* a, Particle* b);


#endif