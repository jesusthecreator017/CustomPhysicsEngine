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

typedef struct {
    Particle* particles[MAX_PARTICLES_PER_CELL];
    int count;
} GridCell;

typedef struct {
    GridCell cells[GRID_WIDTH][GRID_HEIGHT];
} SpatialGrid;

// "Class" Function Prototypes
void Update(Particle* p, float dt);
void Render(Particle* p);

// Initial Constructor
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r, bool isPinned);

// General Function Prototypes
void ConstrainParticle(Particle* p);
void ResolveCollision(Particle* a, Particle* b);
bool ParticleVsParticle(Particle* a, Particle* b);
void DrawVelocityArrow(Particle* p);

// Grid Function Prototypes
void InitGrid(SpatialGrid* grid);
void AddParticleToGrid(SpatialGrid* grid, Particle* particle);
void UpdateGrid(SpatialGrid* grid, Particle* particles, int particleCount);
void CheckGridCollisions(SpatialGrid* grid);

#endif