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

    // Draw the velocity arrow
    DrawVelocityArrow(p);
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
    Vector2 delta = Vector2Subtract(p2->position, p1->position);
    float dist = Vector2Length(delta);
    
    // Early exit for identical positions
    if (dist < 0.0001f) {
        // Slight random jitter to separate overlapping particles
        p2->position.x += 0.01f * (float)GetRandomValue(-10, 10);
        p2->position.y += 0.01f * (float)GetRandomValue(-10, 10);
        return;
    }
    
    // Precompute collision normal
    Vector2 normal = Vector2Scale(delta, 1.0f/dist);
    
    // Calculate penetration and mass ratios only once
    float penetration = (p1->radius + p2->radius) - dist;
    float p1MoveRatio, p2MoveRatio;
    
    if (p1->isPinned) {
        p1MoveRatio = 0.0f;
        p2MoveRatio = 1.0f;
    } else if (p2->isPinned) {
        p1MoveRatio = 1.0f;
        p2MoveRatio = 0.0f;
    } else {
        float totalMass = p1->mass + p2->mass;
        p1MoveRatio = p2->mass / totalMass;
        p2MoveRatio = p1->mass / totalMass;
    }
    
    // Apply position correction
    if (!p1->isPinned) {
        p1->position = Vector2Subtract(p1->position, 
                         Vector2Scale(normal, penetration * p1MoveRatio));
    }
    
    if (!p2->isPinned) {
        p2->position = Vector2Add(p2->position, 
                       Vector2Scale(normal, penetration * p2MoveRatio));
    }
    
    // Calculate velocities once
    Vector2 v1 = Vector2Subtract(p1->position, p1->oldPosition);
    Vector2 v2 = Vector2Subtract(p2->position, p2->oldPosition);
    
    // Calculate relative velocity
    float relVelDot = Vector2DotProduct(Vector2Subtract(v2, v1), normal);
    
    // Only apply impulse if objects are moving toward each other
    if (relVelDot < 0) {
        float combinedRestitution = (p1->restitution + p2->restitution) * 0.5f;
        float impulse = relVelDot * combinedRestitution;
        
        // Apply impulse
        if (!p1->isPinned) {
            p1->oldPosition = Vector2Add(p1->oldPosition, 
                              Vector2Scale(normal, impulse * p1MoveRatio));
        }
        
        if (!p2->isPinned) {
            p2->oldPosition = Vector2Subtract(p2->oldPosition, 
                              Vector2Scale(normal, impulse * p2MoveRatio));
        }
    }
}

// Checks for when a particle is within another particle 
bool ParticleVsParticle(Particle* a, Particle* b) {
    // Avoid square root calculation by using squared distances
    float radiusSum = a->radius + b->radius;
    float radiusSumSquared = radiusSum * radiusSum;
    
    // Direct calculation without pow() function
    float dx = b->position.x - a->position.x;
    float dy = b->position.y - a->position.y;
    float distanceSquared = dx*dx + dy*dy;
    
    return distanceSquared < radiusSumSquared;
}

// Function to draw velocity arrow
void DrawVelocityArrow(Particle* p) {
    // Calculate velocity vector
    Vector2 velocity = Vector2Subtract(p->position, p->oldPosition);
    
    // Only draw arrows if the particle is actually moving
    float speed = Vector2Length(velocity);
    if (speed < 0.1f) return; // Don't draw tiny arrows
    
    // Scale arrow length based on speed - adjust scaling factor as needed
    float arrowScale = 50.0f; // You can adjust this to make arrows longer/shorter
    float arrowLength = speed * arrowScale;
    
    // Cap maximum arrow length
    float maxLength = 200.0f;
    if (arrowLength > maxLength) arrowLength = maxLength;
    
    // Calculate endpoint of the arrow
    Vector2 normalized = Vector2Normalize(velocity);
    Vector2 endpoint = Vector2Add(p->position, Vector2Scale(normalized, arrowLength));
    
    // Draw the main line of the arrow
    Color arrowColor = RED; // You can choose a different color
    DrawLineEx(p->position, endpoint, 2.0f, arrowColor);
    
    // Draw the arrowhead
    float headLength = arrowLength * 0.3f; // Arrowhead size proportional to arrow length
    if (headLength > 10.0f) headLength = 10.0f; // Cap max arrowhead size
    
    // Calculate arrowhead vectors (perpendicular to velocity)
    Vector2 perpendicular = {-normalized.y, normalized.x};
    Vector2 head1 = Vector2Subtract(endpoint, Vector2Scale(normalized, headLength));
    head1 = Vector2Add(head1, Vector2Scale(perpendicular, headLength * 1.0f));
    
    Vector2 head2 = Vector2Subtract(endpoint, Vector2Scale(normalized, headLength));
    head2 = Vector2Subtract(head2, Vector2Scale(perpendicular, headLength * 1.0f));
    
    // Draw arrowhead
    DrawLineEx(endpoint, head1, 2.0f, arrowColor);
    DrawLineEx(endpoint, head2, 2.0f, arrowColor);
}

// Grid Function Prototypes

// Initialize the grid
void InitGrid(SpatialGrid* grid) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid->cells[x][y].count = 0;
        }
    }
}

// Add particle to appropriate grid cell
void AddParticleToGrid(SpatialGrid* grid, Particle* particle) {
    int cellX = (int)(particle->position.x / GRID_CELL_SIZE);
    int cellY = (int)(particle->position.y / GRID_CELL_SIZE);
    
    // Ensure within bounds
    if (cellX < 0) cellX = 0;
    if (cellY < 0) cellY = 0;
    if (cellX >= GRID_WIDTH) cellX = GRID_WIDTH - 1;
    if (cellY >= GRID_HEIGHT) cellY = GRID_HEIGHT - 1;
    
    // Add to cell if there's room
    int idx = grid->cells[cellX][cellY].count;
    if (idx < MAX_PARTICLES_PER_CELL) {
        grid->cells[cellX][cellY].particles[idx] = particle;
        grid->cells[cellX][cellY].count++;
    }
}

void UpdateGrid(SpatialGrid* grid, Particle* particles, int particleCount) {
    InitGrid(grid);
    
    for (int i = 0; i < particleCount; i++) {
        AddParticleToGrid(grid, &particles[i]);
    }
}

// Check collisions using the grid
void CheckGridCollisions(SpatialGrid* grid) {
    // For each cell in the grid
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            GridCell* cell = &grid->cells[x][y];
            
            // Check collisions within this cell
            for (int i = 0; i < cell->count; i++) {
                for (int j = i + 1; j < cell->count; j++) {
                    if (ParticleVsParticle(cell->particles[i], cell->particles[j])) {
                        ResolveCollision(cell->particles[i], cell->particles[j]);
                    }
                }
            }
            
            // Check collisions with neighboring cells (to avoid checking twice)
            // Only need to check 3 neighbors due to how we iterate through the grid
            int neighbors[3][2] = {{1,0}, {0,1}, {1,1}};
            
            for (int n = 0; n < 3; n++) {
                int nx = x + neighbors[n][0];
                int ny = y + neighbors[n][1];
                
                if (nx < GRID_WIDTH && ny < GRID_HEIGHT) {
                    GridCell* neighborCell = &grid->cells[nx][ny];
                    
                    // Check collisions between cells
                    for (int i = 0; i < cell->count; i++) {
                        for (int j = 0; j < neighborCell->count; j++) {
                            if (ParticleVsParticle(cell->particles[i], neighborCell->particles[j])) {
                                ResolveCollision(cell->particles[i], neighborCell->particles[j]);
                            }
                        }
                    }
                }
            }
        }
    }
}