#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include "particle.h"
#include "sticks.h"

// Configuration constants
#define PHYSICS_TIME_STEP 1.0f/60.0f
#define MAX_PHYSICS_STEPS 5

typedef struct Engine{
    // Resources
    Particle* particles;
    Stick* sticks;
    int particleCount;
    int stickCount;
    
    // Physics settings
    float timeStep;
    float accumulator;
    int constraintIterations;
    float globalDamping;
    
    // Input handling
    int mouseGrabCondition;
    
    // UI state
    bool isActive;
    
    // Methods
    void (*Initialize)(struct Engine* engine, int particleCount, int stickCount);
    void (*Update)(struct Engine* engine, float deltaTime);
    void (*Render)(struct Engine* engine);
    void (*HandleInput)(struct Engine* engine);
    void (*Shutdown)(struct Engine* engine);
} Engine;

// Constructor/destructor functions
void InitEngine(Engine* engine, int particleCount, int stickCount);
void ShutdownEngine(Engine* engine);

// Core engine functions
void EngineUpdate(Engine* engine, float deltaTime);
void EngineRender(Engine* engine);
void EngineHandleInput(Engine* engine);
void UpdatePhysics(Engine* engine, float dt);

// Helper functions
void CreateParticleNetwork(Engine* engine);

#endif