#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include "particle.h"
#include "sticks.h"

// Configuration constants
#define PHYSICS_TIME_STEP 1.0f/60.0f
#define MAX_PHYSICS_STEPS 5

// Scene Enum
typedef enum {
    SCENE_MENU,
    SCENE_PENDULUM,
    SCENE_CLOTH,
    SCENE_PARTICLES,
    SCENE_SOCCER
} SceneType;


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

    // Scene Type
    SceneType currentScene;
    
    // Input handling
    int mouseGrabCondition;
    
    // UI state
    bool isActive;
    
    // Main Methods
    void (*Initialize)(struct Engine* engine, int particleCount, int stickCount);
    void (*Update)(struct Engine* engine, float deltaTime);
    void (*Render)(struct Engine* engine);
    void (*HandleInput)(struct Engine* engine);
    void (*Shutdown)(struct Engine* engine);

    // Scene Methods (For scene specific operations)
    void (*InitScene)(struct Engine* engine, SceneType scene);
    void (*UpdateScene)(struct Engine* engine, float dt);
    void (*RenderScene)(struct Engine* engine);
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

void SwitchScene(Engine* engine, SceneType newScene);

// Scene Function Definitions
void InitScene(Engine* engine, SceneType scene);
void UpdateScene(Engine* engine, float dt);
void RenderScene(Engine* engine);

#endif