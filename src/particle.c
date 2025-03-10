#include "particle.h"

// "Class" Function Definition
void Update(Particle* p, float dt){
    if(!p->isPinned){
        // Store current position before updating it
        Vector2 temp = p->position;

        // Calculate the acceleration
        Vector2 acceleration = {0, GRAVITY};

        // Verlet Integration
        p->position = Vector2Add(Vector2Subtract(Vector2Scale(p->position, 2), p->oldPosition), Vector2Scale(acceleration, dt*dt));

        // Update the old position
        p->oldPosition = temp;
        
        // Clamp velocity by limiting the distance between current and old position
        Vector2 newVelocity = Vector2Subtract(p->position, p->oldPosition);
        float currentSpeed = Vector2Length(newVelocity);
        
        // If speed exceeds maximum, scale it down
        if(currentSpeed > MAX_VELOCITY) {
            // Normalize and scale to max velocity
            newVelocity = Vector2Scale(Vector2Normalize(newVelocity), MAX_VELOCITY);
            // Recalculate position based on clamped velocity
            p->position = Vector2Add(p->oldPosition, newVelocity);
        }

        // Updating debug data
        p->info.position = p->position;
        p->info.velocity = newVelocity; // Updated velocity after clamping
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
void ResolveCollision(Particle* p1, Particle* p2) {
    // Calculate distance vector
    float dx = p2->position.x - p1->position.x;
    float dy = p2->position.y - p1->position.y;
    float dist = sqrt(dx*dx + dy*dy);
    
    // Avoid division by zero
    if (dist < 0.0001f) {
        // Particles are at same position, push slightly apart
        p2->position.x += 0.1f;
        p2->position.y += 0.1f;
        return;
    }
    
    // Calculate collision normal
    float nx = dx / dist;
    float ny = dy / dist;
    
    // Calculate penetration depth
    float penetration = (p1->radius + p2->radius) - dist;
    
    // If either particle is static, the other takes full responsibility for moving
    float p1MoveRatio = 0.5f;
    float p2MoveRatio = 0.5f;
    
    if (p1->isPinned) {
        p1MoveRatio = 0.0f;
        p2MoveRatio = 1.0f;
    } else if (p2->isPinned) {
        p1MoveRatio = 1.0f;
        p2MoveRatio = 0.0f;
    } else {
        // Dynamic vs dynamic: use mass ratio for physically correct response
        float totalMass = p1->mass + p2->mass;
        p1MoveRatio = p2->mass / totalMass;
        p2MoveRatio = p1->mass / totalMass;
    }
    
    // Separate the particles based on penetration and mass ratio
    p1->position.x -= nx * penetration * p1MoveRatio;
    p1->position.y -= ny * penetration * p1MoveRatio;
    
    p2->position.x += nx * penetration * p2MoveRatio;
    p2->position.y += ny * penetration * p2MoveRatio;
    
    // Calculate collision impulse for bounce
    float combinedRestitution = (p1->restitution + p2->restitution) * 0.5f;
    
    // Calculate velocities (for Verlet, velocity = position - lastPosition)
    Vector2 v1 = {p1->position.x - p1->oldPosition.x, p1->position.y - p1->oldPosition.y};
    Vector2 v2 = {p2->position.x - p2->oldPosition.x, p2->position.y - p2->oldPosition.y};
    
    // Calculate relative velocity along normal
    float vRelX = v2.x - v1.x;
    float vRelY = v2.y - v1.y;
    float vDot = vRelX * nx + vRelY * ny;
    
    // Only apply bounce if objects are moving toward each other
    if (vDot < 0) {
        float impulse = vDot * combinedRestitution;
        
        // Apply impulse based on mass ratio
        if (!p1->isPinned) {
            p1->oldPosition.x += nx * impulse * p1MoveRatio;
            p1->oldPosition.y += ny * impulse * p1MoveRatio;
        }
        
        if (!p2->isPinned) {
            p2->oldPosition.x -= nx * impulse * p2MoveRatio;
            p2->oldPosition.y -= ny * impulse * p2MoveRatio;
        }
    }
}

// Checks for when a particle is within another particle 
bool ParticleVsParticle(Particle* a, Particle* b){
    float r = a->radius + b->radius;
    r *= r; // Squaring the sum of radii

    // Corrected distance squared formula
    float distance_squared = powf(b->position.x - a->position.x, 2) + powf(b->position.y - a->position.y, 2);

    return distance_squared < r; // Check if objects overlap
}