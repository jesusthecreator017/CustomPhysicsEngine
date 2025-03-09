#include "engine.h"
#include <stdlib.h>
#include "MouseOperations.h"

// Initialize the engine
void InitEngine(Engine* engine, int particleCount, int stickCount) {
    // Allocate memory for particles and sticks
    engine->particles = (Particle*)malloc(sizeof(Particle) * particleCount);
    engine->sticks = (Stick*)malloc(sizeof(Stick) * stickCount);
    
    engine->particleCount = particleCount;
    engine->stickCount = stickCount;
    
    // Physics settings
    engine->timeStep = PHYSICS_TIME_STEP;
    engine->accumulator = 0.0f;
    engine->constraintIterations = CONSTRAINT_ITERATIONS;
    engine->globalDamping = 0.998f;
    
    // Input and UI state
    engine->mouseGrabCondition = 0;
    engine->isActive = false;
    
    // Set up function pointers
    engine->Initialize = InitEngine;
    engine->Update = EngineUpdate;
    engine->Render = EngineRender;
    engine->HandleInput = EngineHandleInput;
    engine->Shutdown = ShutdownEngine;
    
    // Create initial network of particles
    CreateParticleNetwork(engine);
}

// Clean up resources
void ShutdownEngine(Engine* engine) {
    free(engine->particles);
    free(engine->sticks);
    
    engine->particleCount = 0;
    engine->stickCount = 0;
}

// Update physics with fixed timestep
void EngineUpdate(Engine* engine, float deltaTime) {
    // Cap delta time to prevent spiral of death
    if (deltaTime > 0.25f) deltaTime = 0.25f;
    
    // Accumulate time
    engine->accumulator += deltaTime;
    
    // Run physics at fixed intervals
    int steps = 0;
    while (engine->accumulator >= engine->timeStep && steps < MAX_PHYSICS_STEPS) {
        UpdatePhysics(engine, engine->timeStep);
        engine->accumulator -= engine->timeStep;
        steps++;
    }
}

// Update physics at a fixed timestep
void UpdatePhysics(Engine* engine, float dt) {
    // Apply constraints first
    for (int i = 0; i < engine->particleCount; i++) {
        ConstrainParticle(&engine->particles[i]);
    }
    
    // Update all particles
    for (int i = 0; i < engine->particleCount; i++) {
        engine->particles[i].Update(&engine->particles[i], dt);
    }
    
    // Run constraint solving iterations
    for (int i = 0; i < engine->constraintIterations; i++) {
        // Update all sticks
        for (int j = 0; j < engine->stickCount; j++) {
            engine->sticks[j].Update(&engine->sticks[j]);
        }
        
        // Resolve collisions
        for (int j = 0; j < engine->particleCount; j++) {
            for (int k = j + 1; k < engine->particleCount; k++) {
                if (ParticleVsParticle(&engine->particles[j], &engine->particles[k])) {
                    ResolveCollision(&engine->particles[j], &engine->particles[k]);
                }
            }
        }
    }
}

// Render all objects
void EngineRender(Engine* engine) {
    if (!engine->isActive) return;
    
    // Render sticks first (behind particles)
    for (int i = 0; i < engine->stickCount; i++) {
        engine->sticks[i].Render(&engine->sticks[i]);
    }
    
    // Render particles
    for (int i = 0; i < engine->particleCount; i++) {
        engine->particles[i].Render(&engine->particles[i]);
    }
    
    // Debug rendering
    if (engine->mouseGrabCondition == 1) {
        for (int i = 0; i < engine->particleCount; i++) {
            if (engine->particles[i].isGrabbed) {
                ObjectInfo(engine->particles[i].info, engine->particles[i].type);
                break;
            }
        }
    }
}

// Handle user input
void EngineHandleInput(Engine* engine) {
    // Toggle active state
    if (IsKeyPressed(KEY_A)) {
        engine->isActive = true;
    }
    
    if (IsKeyPressed(KEY_Z)) {
        engine->isActive = false;
    }
    
    // Handle mouse interactions
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        PickUpParticle(engine->particles, &engine->mouseGrabCondition);
    } else {
        // Reset grab condition
        engine->mouseGrabCondition = 0;
        for (int i = 0; i < engine->particleCount; i++) {
            engine->particles[i].isGrabbed = 0;
        }
    }
}

// Create initial network of particles
void CreateParticleNetwork(Engine* engine) {
    // Initialize particles with your standard setup
    InitParticle(
        &engine->particles[0],
        (Vector2){500, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &engine->particles[1],
        (Vector2){600, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &engine->particles[2],
        (Vector2){600, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &engine->particles[3],
        (Vector2){500, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &engine->particles[4],
        (Vector2){500, 200},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        true
    );
    
    // Initialize sticks
    InitStick(&engine->sticks[0], &engine->particles[0], &engine->particles[1], Vector2Distance(engine->particles[0].position, engine->particles[1].position), Vector2Distance(engine->particles[0].position, engine->particles[1].position) - 10.0f);
    InitStick(&engine->sticks[1], &engine->particles[1], &engine->particles[2], Vector2Distance(engine->particles[1].position, engine->particles[2].position), Vector2Distance(engine->particles[1].position, engine->particles[2].position) - 10.0f);
    InitStick(&engine->sticks[2], &engine->particles[2], &engine->particles[3], Vector2Distance(engine->particles[2].position, engine->particles[3].position), Vector2Distance(engine->particles[2].position, engine->particles[3].position) - 10.0f);
    InitStick(&engine->sticks[5], &engine->particles[4], &engine->particles[2], Vector2Distance(engine->particles[4].position, engine->particles[2].position), Vector2Distance(engine->particles[4].position, engine->particles[2].position) - 10.0f);
    InitStick(&engine->sticks[3], &engine->particles[3], &engine->particles[0], Vector2Distance(engine->particles[3].position, engine->particles[0].position), Vector2Distance(engine->particles[3].position, engine->particles[0].position) - 10.0f);
    InitStick(&engine->sticks[4], &engine->particles[0], &engine->particles[2], Vector2Distance(engine->particles[0].position, engine->particles[2].position), Vector2Distance(engine->particles[0].position, engine->particles[2].position) / 2);
}