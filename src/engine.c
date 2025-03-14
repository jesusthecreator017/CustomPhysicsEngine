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
    // Cap delta time to prevent spiral of death if frame rate drops too low
    if (deltaTime > 0.25f) deltaTime = 0.25f;
    
    // Accumulate time
    engine->accumulator += deltaTime;
    
    // Run fixed physics steps based on accumulated time
    int steps = 0;
    while (engine->accumulator >= engine->timeStep && steps < MAX_PHYSICS_STEPS) {
        // Process physics substeps for more accuracy
        const int substeps = 2;  // Number of substeps
        float subDt = engine->timeStep / substeps;
        
        for (int i = 0; i < substeps; i++) {
            UpdatePhysics(engine, subDt);
        }
        
        engine->accumulator -= engine->timeStep;
        steps++;
    }
}

// Update physics at a fixed timestep
void UpdatePhysics(Engine* engine, float dt) {
    // First, initialize a spatial grid
    SpatialGrid grid;
    InitGrid(&grid);
    
    // Apply forces and update all particles
    for (int i = 0; i < engine->particleCount; i++) {
        engine->particles[i].Update(&engine->particles[i], dt);
    }
    
    // Apply constraints (boundaries)
    for (int i = 0; i < engine->particleCount; i++) {
        ConstrainParticle(&engine->particles[i]);
    }
    
    // Run constraint solving iterations
    for (int iter = 0; iter < engine->constraintIterations; iter++) {
        // Update all sticks (distance constraints)
        for (int j = 0; j < engine->stickCount; j++) {
            engine->sticks[j].Update(&engine->sticks[j]);
        }
        
        // Update the grid with current particle positions
        UpdateGrid(&grid, engine->particles, engine->particleCount);
        
        // Use the grid for collision detection and resolution
        CheckGridCollisions(&grid);
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
    else if (IsKeyPressed(KEY_F)) {
        SwitchScene(engine, SCENE_SOCCER);
    }
    else if (IsKeyPressed(KEY_Q)) {
        SwitchScene(engine, SCENE_PATCHNOTES);
    }
    else if (IsKeyPressed(KEY_G)) {
        SwitchScene(engine, SCENE_CRADLE);
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
    InitStick(&engine->sticks[0], &engine->particles[0], &engine->particles[1], 
              Vector2Distance(engine->particles[0].position, engine->particles[1].position), 
              Vector2Distance(engine->particles[0].position, engine->particles[1].position) - 10.0f);

    InitStick(&engine->sticks[1], &engine->particles[1], &engine->particles[2], 
              Vector2Distance(engine->particles[1].position, engine->particles[2].position), 
              Vector2Distance(engine->particles[1].position, engine->particles[2].position) - 10.0f);

    InitStick(&engine->sticks[2], &engine->particles[2], &engine->particles[3], 
              Vector2Distance(engine->particles[2].position, engine->particles[3].position), 
              Vector2Distance(engine->particles[2].position, engine->particles[3].position) - 10.0f);

    InitStick(&engine->sticks[3], &engine->particles[3], &engine->particles[4], 
              Vector2Distance(engine->particles[3].position, engine->particles[4].position), 
              Vector2Distance(engine->particles[3].position, engine->particles[4].position) - 10.0f);

    InitStick(&engine->sticks[4], &engine->particles[4], &engine->particles[5], 
              Vector2Distance(engine->particles[4].position, engine->particles[5].position), 
              Vector2Distance(engine->particles[4].position, engine->particles[5].position) / 2);

    InitStick(&engine->sticks[5], &engine->particles[5], &engine->particles[5], 
              Vector2Distance(engine->particles[5].position, engine->particles[5].position), 
              Vector2Distance(engine->particles[5].position, engine->particles[5].position) - 10.0f);
}

void SwitchScene(Engine* engine, SceneType newScene){
    // Initialize the new Scene 
    engine->currentScene = newScene;
    engine->isActive = (newScene != SCENE_MENU);
    engine->InitScene(engine, newScene);
}

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
            engine->particleCount = 30;
            engine->stickCount = 0; // No sticks needed
            break;

        case SCENE_SOCCER:
            engine->particleCount = 2; 
            engine->stickCount = 0; 
            break;

        case SCENE_PATCHNOTES:
            engine->particleCount = 0; 
            engine->stickCount = 0; 
            break;

        case SCENE_CRADLE:
            engine->particleCount = 6; 
            engine->stickCount = 5; 
            break;
            
        default:
            engine->particleCount = 3;  // Default to network
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
            InitParticle(&engine->particles[0], 
            (Vector2){GetScreenWidth()/2, 100}, 
            BLUE, 
            50, 
            0.5f, 
            15.0f, 
            true);
            
            for (int i = 1; i <= 5; i++) {
                InitParticle(&engine->particles[i], 
                (Vector2){GetScreenWidth()/2, 100 + i * 60},
                (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255},
                1,
                0.5f, 
                0.000001f, 
                false);

                InitStick(&engine->sticks[i-1], &engine->particles[i-1], &engine->particles[i], 60.0f, 60.0f);

                if (i == 5){
                    InitParticle(&engine->particles[i], 
                    (Vector2){GetScreenWidth()/2, 100 + i * 60},
                    (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255},
                    50,
                    0.5f, 
                    25.0f, 
                    false);

                InitStick(&engine->sticks[i-1], &engine->particles[i-1], &engine->particles[i], 60.0f, 60.0f);
                    
                }
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
                                     (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255}, 
                                     10, 
                                     0.5f, 
                                     5.0f, 
                                     (y == 0 && x == 0) || (y == 0 && x == cols - 1));
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
                             GetRandomValue(10, 30), 
                             0.5f, 
                             10.0f, 
                             false);
            }
            break;
        
        case SCENE_SOCCER:
            InitParticle(&engine->particles[0], 
                                (Vector2){GetRandomValue(100, GetScreenWidth()-100), GetRandomValue(100, GetScreenHeight()-100)}, 
                                (Color){128, 0, 0, 255}, 
                                50, 
                                0.5f, 
                                35.0f, 
                                false);

            InitParticle(&engine->particles[1], 
                                (Vector2){GetRandomValue(100, GetScreenWidth()-100), GetRandomValue(100, GetScreenHeight()-100)}, 
                                (Color){64, 0, 64, 255}, 
                                50, 
                                0.5f, 
                                35.0f, 
                                false);
            break;

        case SCENE_CRADLE:
            InitParticle(&engine->particles[0], 
            (Vector2){GetScreenWidth()/2, 100}, 
            BLUE, 
            50, 
            0.5f, 
            15.0f, 
            true);
            
            for (int i = 1; i <= 5; i++) {
                InitParticle(&engine->particles[i], 
                (Vector2){GetScreenWidth()/2 + i * 60, 100},
                (Color){GetRandomValue(50, 255), 
                GetRandomValue(50, 255), 
                GetRandomValue(50, 255), 255},
                1,
                0.5f, 
                0.000001f, 
                false);

                InitStick(&engine->sticks[i-1], 
                          &engine->particles[i-1], 
                          &engine->particles[i], 
                          60.0f, 
                          60.0f);

                if (i == 5){
                    InitParticle(&engine->particles[i], 
                    (Vector2){GetScreenWidth()/2 + 400, 100},
                    (Color){GetRandomValue(50, 255), 
                    GetRandomValue(50, 255), 
                    GetRandomValue(50, 255), 255},
                    50,
                    0.5f, 
                    25.0f, 
                    false);

                InitStick(&engine->sticks[i-1], &engine->particles[i-1], &engine->particles[i], 60.0f, 60.0f);
                    
                }
            }

            break;

        case SCENE_PATCHNOTES:
            break;

        default:
            CreateParticleNetwork(engine);
            break;
    }
}


// Function to update a specific scene
void UpdateScene(Engine* engine, float dt) {
    //Most scenes should use the general physics
    if (engine->isActive) {
        UpdatePhysics(engine, dt);
    }

    // Scene-specific updates could be added here
    switch(engine->currentScene) {
        case SCENE_MENU:
            // Menu doesn't need physics updates
            break;
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
            DrawText("Welcome to our Custom Physics Engine!", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 300, 30, BLUE);
            DrawText("Created by Jesus, Chase, Alvaro.", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 250, 25, BLUE);
            DrawText("Version 0.13B - 03/13", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 200, 25, BLUE);
            
            DrawText("List of Current Modes: ", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 100, 25, BLUE);
            DrawText("[a] - Pendulum Simulation", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 50, 25, BLUE);
            DrawText("[s] - Cloth Simulation", GetScreenWidth()/2 - 300, GetScreenHeight()/2, 25, BLUE);
            DrawText("[d] - Particle System", GetScreenWidth()/2 - 300, GetScreenHeight()/2 + 50, 25, BLUE);
            DrawText("[f] - Soccer Game", GetScreenWidth()/2 - 300, GetScreenHeight()/2 + 100, 25, BLUE);
            DrawText("[g] - Newton Cradle Simulation", GetScreenWidth()/2 - 300, GetScreenHeight()/2 + 150, 25, BLUE);
            DrawText("Press [q] for current patch notes", GetScreenWidth()/2 - 300, GetScreenHeight()/2 + 300, 25, BLUE);
            break;
        
        case SCENE_PENDULUM:
            DrawText("Pendulum Simulation", 10, 110, 20, DARKBLUE);
            DrawText("- Changed balls to 0.0001f size", 10, 135, 20, DARKBLUE);
            DrawText("- Allows for movement to be seen", 10, 160, 20, DARKBLUE);

            DrawText("[a] Restart scene", 20, GetScreenHeight() - 70, 20, BLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
            
        case SCENE_CLOTH:
            DrawText("Cloth Simulation", 10, 110, 20, DARKBLUE);
            DrawText("- Creating a mutable shape", 10, 135, 20, DARKBLUE);

            DrawText("[s] Restart scene", 20, GetScreenHeight() - 70, 20, BLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
            
        case SCENE_PARTICLES:
            DrawText("Particle System", 10, 110, 20, DARKBLUE);
            DrawText("# of Particles: 30",10, 135, 20, DARKBLUE);

            DrawText("[d] Restart scene", 20, GetScreenHeight() - 70, 20, BLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;
    
        case SCENE_SOCCER:
            DrawText("Soccer Game", 10, 110, 20, DARKBLUE);
            DrawText("- Testing collisons", 10, 135, 20, DARKBLUE);

            DrawText("[f] Restart scene", 20, GetScreenHeight() - 70, 20, BLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            DrawText("________________________________________________________________", 10, GetScreenHeight() - 40, 50, GREEN);
            break;

        case SCENE_CRADLE:
            DrawText("Newton's Cradle", 10, 110, 20, DARKBLUE);
            DrawText("- Observe the forces of 2 particles ", 10, 135, 20, DARKBLUE);

            DrawText("[g] Restart scene", 20, GetScreenHeight() - 70, 20, BLUE);
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);
            break;

        case SCENE_PATCHNOTES:
            DrawText("New in Version 0.13B:", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 300, 30, BLUE);
            DrawText("- Physics was altered", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 250, 20, BLUE);
            DrawText(" ", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 200, 20, BLUE);
            DrawText(" ", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 150, 20, BLUE);
            DrawText(" ", GetScreenWidth()/2 - 300, GetScreenHeight()/2 - 100, 20, BLUE);
            break;   

        default:
            DrawText("[z] Return to Menu", 20, GetScreenHeight() - 40, 20, BLUE);  
            break;
    }
 }