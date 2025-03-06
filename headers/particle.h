#ifndef PARTICLE_H
#define PARTICLE_H

// Includes
#include "debug.h"

// Particle "Class"
typedef struct particle{
    // Attributes
    Color color;
    ObjectType type;
    DebugData info;
    Vector2 position;
    Vector2 oldPosition;
    float mass;
    float restitution;
    float radius;
    int isGrabbed;
    bool isPinned;

    // Functions (Simulating Classes)
    void (*Update)(struct particle* p, float dt);
    void (*Render)(struct particle* p);
} Particle;

// "Class" Function Prototypes
void Update(Particle* p, float dt);
void Render(Particle* p);

// Initial Constructor
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r, bool isPinned);

// General Function Prototypes
void ConstrainParticle(Particle* p);
void ResolveCollision(Particle* a, Particle* b);
bool ParticleVsParticle(Particle* a, Particle* b);


#endif