#include "engine.h"
#include <stdlib.h>
#include "MouseOperations.h"

// Update InitEngine to include scene management
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
    
    // Set up scene management function pointers
    engine->InitScene = InitScene;
    engine->UpdateScene = UpdateScene;
    engine->RenderScene = RenderScene;
    
    // Default to menu scene
    engine->currentScene = SCENE_MENU;
    
    // Initialize the default scene
    engine->InitScene(engine, SCENE_MENU);
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
    // Scene switching controls
    if (IsKeyPressed(KEY_A)) {
        SwitchScene(engine, SCENE_PENDULUM);
    }
    else if (IsKeyPressed(KEY_S)) {
        SwitchScene(engine, SCENE_CLOTH);
    }
    else if (IsKeyPressed(KEY_D)) {
        SwitchScene(engine, SCENE_PARTICLES);
    }
    else if (IsKeyPressed(KEY_Z)) {
        SwitchScene(engine, SCENE_MENU);
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

void SwitchScene(Engine* engine, SceneType newScene){
    // Initialize the new Scene 
    engine->currentScene = newScene;
    engine->isActive = (newScene != SCENE_MENU);
    engine->InitScene(engine, newScene);
}

// Function to initialize a specific scene
void InitScene(Engine* engine, SceneType scene) {
    // Set particle and stick count based on scene before freeing memory
    switch(scene) {
        case SCENE_MENU:
            engine->particleCount = 0;
            engine->stickCount = 0;
            break;
            
        case SCENE_PENDULUM:
            engine->particleCount = 6; // 1 anchor + 5 segments
            engine->stickCount = 5; // 5 sticks connecting each segment
            break;
            
        case SCENE_CLOTH:
            {
                int rows = 5, cols = 5;
                engine->particleCount = rows * cols;
                engine->stickCount = (rows * (cols - 1)) + (cols * (rows - 1)); // Horizontal + Vertical connections
            }
            break;
            
        case SCENE_PARTICLES:
            engine->particleCount = 50; // Example: 50 random particles
            engine->stickCount = 0; // No sticks needed
            break;
            
        default:
            engine->particleCount = 5;  // Default to network
            engine->stickCount = 6;
            break;
    }
    
    // Free old memory **only after setting the new scene's requirements**
    if (engine->particles) free(engine->particles);
    if (engine->sticks) free(engine->sticks);

    // Allocate new memory based on updated counts
    if (engine->particleCount > 0) {
        engine->particles = (Particle*)malloc(sizeof(Particle) * engine->particleCount);
    } else {
        engine->particles = NULL;
    }

    if (engine->stickCount > 0) {
        engine->sticks = (Stick*)malloc(sizeof(Stick) * engine->stickCount);
    } else {
        engine->sticks = NULL;
    }
    
    // Ensure active state is correctly set
    engine->isActive = (scene != SCENE_MENU);

    // Initialize scene-specific particles/sticks
    switch(scene) {
        case SCENE_MENU:
            // Nothing to initialize
            break;
        case SCENE_PENDULUM:
            // Initialize pendulum setup
            InitParticle(&engine->particles[0], (Vector2){GetScreenWidth()/2, 100}, BLUE, 50, 0.5f, 10.0f, true);
            for (int i = 1; i <= 5; i++) {
                InitParticle(&engine->particles[i], (Vector2){GetScreenWidth()/2, 100 + i * 60}, RED, 30, 0.5f, 10.0f, false);
                InitStick(&engine->sticks[i-1], &engine->particles[i-1], &engine->particles[i], 60.0f, 60.0f);
            }
            break;
        case SCENE_CLOTH:
            {
                int rows = 5, cols = 5;
                float spacing = 40.0f;
                int stickIndex = 0;

                for (int y = 0; y < rows; y++) {
                    for (int x = 0; x < cols; x++) {
                        int index = y * cols + x;
                        InitParticle(&engine->particles[index], 
                                     (Vector2){GetScreenWidth()/2 - (cols * spacing)/2 + x * spacing, 200 + y * spacing}, 
                                     BLUE, 10, 0.5f, 5.0f, y == 0);
                    }
                }
                
                for (int y = 0; y < rows; y++) {
                    for (int x = 0; x < cols - 1; x++) {
                        int p1 = y * cols + x;
                        int p2 = y * cols + x + 1;
                        InitStick(&engine->sticks[stickIndex++], &engine->particles[p1], &engine->particles[p2], spacing, spacing);
                    }
                }

                for (int x = 0; x < cols; x++) {
                    for (int y = 0; y < rows - 1; y++) {
                        int p1 = y * cols + x;
                        int p2 = (y + 1) * cols + x;
                        InitStick(&engine->sticks[stickIndex++], &engine->particles[p1], &engine->particles[p2], spacing, spacing);
                    }
                }
            }
            break;
        case SCENE_PARTICLES:
            for (int i = 0; i < engine->particleCount; i++) {
                InitParticle(&engine->particles[i], 
                             (Vector2){GetRandomValue(100, GetScreenWidth()-100), GetRandomValue(100, GetScreenHeight()-100)}, 
                             (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255}, 
                             GetRandomValue(10, 30), 0.5f, 10.0f, false);
            }
            break;
        default:
            CreateParticleNetwork(engine);
            break;
    }
}


// Function to update a specific scene
void UpdateScene(Engine* engine, float dt) {
    // Scene-specific updates could be added here
    switch(engine->currentScene) {
        case SCENE_MENU:
            // Menu doesn't need physics updates
            break;
        case SCENE_PENDULUM:
            // Use the mendulum Physics
            if (engine->isActive) {
                UpdatePhysics(engine, dt);
            }
        default:
            break;
    }
}

// Function to render a specific scene
void RenderScene(Engine* engine) {
    // Render all physics objects first
    EngineRender(engine);
    
    // Add scene-specific UI elements
    switch(engine->currentScene) {
        case SCENE_MENU:
            DrawText("Welcome to our Custom Physics Engine!", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 100, 30, BLUE);
            DrawText("Created by Jesus, Chase, Alvaro.", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 50, 25, BLUE);
            DrawText("Version 0.1 - 03/09", GetScreenWidth()/2 - 300, GetScreenHeight()/2, 25, BLUE);
            
            DrawText("[a] Pendulum Simulation", GetScreenWidth()/2 - 200, GetScreenHeight()/2 + 100, 25, BLUE);
            DrawText("[s] Cloth Simulation", GetScreenWidth()/2 - 200, GetScreenHeight()/2 + 150, 25, BLUE);
            DrawText("[d] Particle System", GetScreenWidth()/2 - 200, GetScreenHeight()/2 + 200, 25, BLUE);
            break;
            
        case SCENE_PENDULUM:
            DrawText("Pendulum Simulation", 20, 20, 20, DARKBLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
            
        case SCENE_CLOTH:
            DrawText("Cloth Simulation", 20, 20, 20, DARKBLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
            
        case SCENE_PARTICLES:
            DrawText("Particle System", 20, 20, 20, DARKBLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
            
        default:
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
    }
}