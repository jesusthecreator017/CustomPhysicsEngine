#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    if(!p->isPinned){
        // "Velocity of the particles"
        Vector2 velocity = Vector2Subtract(p->position, p->oldPosition);

        // Store current position before updating it
        Vector2 temp = p->position;

        // Calculate the acceleration
        Vector2 acceleration = {0, GRAVITY};

        // Verlet Integration
        p->position = Vector2Add(Vector2Subtract(Vector2Scale(p->position, 2), p->oldPosition), Vector2Scale(acceleration, dt*dt));

        // Update the old position
        p->oldPosition = temp;

        // Updating debug data
        p->info.position = p->position;
        p->info.velocity = velocity;
        p->info.color = p->color;
    }
}

// Creates the particle circle and rednders it appropirately
void Render(Particle* p){
    DrawCircleV(p->position, p->radius, p->color);
}

// Constructor Functions that creates particle
void InitParticle(Particle* p, Vector2 pos, Color c, float m, float rest, float r, bool isPinned){
    // Attributes
    p->position = pos;
    p->oldPosition = pos;
    p->color = c;
    p->mass = m;
    p->restitution = rest;
    p->radius = r;
    p->type = TYPE_PARTICLE;
    p->info = (DebugData){pos, p->oldPosition, c, r};
    p->isPinned = isPinned;

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

// Calculates the physics needed for the collison within the particles
void ResolveCollision(Particle* a, Particle* b) {
    // Compute collision normal
    Vector2 collisionNormal = Vector2Subtract(b->position, a->position);
    float distance = Vector2Length(collisionNormal);
    float radiusSum = a->radius + b->radius;

    // Ensure particles are actually colliding
    if (distance == 0 || distance >= radiusSum) return;

    // Normalize the collision normal
    collisionNormal = Vector2Scale(collisionNormal, 1.0f / distance);

    // Compute penetration depth and apply **fractional correction** (50% each)
    float penetrationDepth = (radiusSum - distance) * 0.5f;

    // Add a relaxation factor to prevent over-correction
    float relaxationFactor = 0.4f;
    penetrationDepth *= relaxationFactor;

    // Apply correction based on masses
    float totalMass = a->mass + b->mass;
    float aFactor = b->mass / totalMass;
    float bFactor = a->mass / totalMass;

    if (!a->isPinned)
        a->position = Vector2Subtract(a->position, Vector2Scale(collisionNormal, penetrationDepth * aFactor));
    if (!b->isPinned)
        b->position = Vector2Add(b->position, Vector2Scale(collisionNormal, penetrationDepth * bFactor));


    // Compute relative velocity using Verlet method
    Vector2 relativeVelocity = Vector2Subtract(
        Vector2Subtract(b->position, b->oldPosition),
        Vector2Subtract(a->position, a->oldPosition)
    );

    // Compute velocity along collision normal
    float velocityAlongNormal = Vector2DotProduct(relativeVelocity, collisionNormal);

    // If particles are separating, skip response
    if (velocityAlongNormal > 0) return;

    // Compute **clamped** restitution (must be between 0 and 1)
    float restitution = fminf(fmaxf(a->restitution, 0.0f), 1.0f);

    // Compute impulse magnitude
    float impulseMagnitude = -(1.0f + restitution) * velocityAlongNormal;
    impulseMagnitude /= (1.0f / a->mass) + (1.0f / b->mass);

    // Apply impulse conservatively (reduce factor to 50%)
    Vector2 impulse = Vector2Scale(collisionNormal, impulseMagnitude * 0.5f);

    a->oldPosition = Vector2Subtract(a->oldPosition, Vector2Scale(impulse, 1.0f / a->mass));
    b->oldPosition = Vector2Add(b->oldPosition, Vector2Scale(impulse, 1.0f / b->mass));
}

// Checks for when a particle is within another particle 
bool ParticleVsParticle(Particle* a, Particle* b){
    float r = a->radius + b->radius;
    r *= r; // Squaring the sum of radii

    // Corrected distance squared formula
    float distance_squared = powf(b->position.x - a->position.x, 2) + powf(b->position.y - a->position.y, 2);

    return distance_squared < r; // Check if objects overlap
}