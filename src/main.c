// Import main header file
#include "particle.h"
#include "raylib.h"
#include "MouseOperations.h"

int main(void){
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Test Physics Engine");

    // Array of particles
    Particle particles[MAX_PARTICLES];

    for(int i = 0; i < PARTICLE_NUM; i++){
        InitParticle(
            &particles[i],                                                                          // Particle Reference
            (Vector2){GetRandomValue(10, GetScreenWidth()), GetRandomValue(10, GetScreenHeight())}, // Position
            (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255},   // Color
            GetRandomValue(2, 15),                                                                  // Mass
            1.0f,                                                                                   // Restitution (Bounciness)
            GetRandomValue(5, 25)                                                                   // Radius
        );
    }

    // Main Game Loop
    while (!WindowShouldClose()){
        // Deltat time is used everywhere
        float dt = GetFrameTime();

        
        // Update
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            current->Update(current, dt);
        }

        //Checks to see whether the user clicked on the left mouse button. 
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            PickUpParticle(particles); //(Found in MouseOperations.c)
        }
    
        
        // Check Collisions
        for (int i = 0; i < PARTICLE_NUM; i++) {
            for (int j = i + 1; j < PARTICLE_NUM; j++) {  // Compare each pair only once
                Particle* p1 = &particles[i];
                Particle* p2 = &particles[j];
                
                if (ParticleVsParticle(p1, p2)) {
                    ResolveCollision(p1, p2);
                }
            }
        }
        
        // Apply Constraints
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            ConstrainParticle(current);
        }
        
        // Render Logic Here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
        
            DrawLineEx((Vector2){0, 610}, (Vector2){1300, 610}, 20, BLUE);

            // Debug Rendering
            ObjectInfo(particles[1].info, particles[1].type);

            // Render Objects Here
            for(int i = 0; i < PARTICLE_NUM; i++){
                Particle* current = &particles[i];
                current->Render(current);
            }
        EndDrawing();
    }
    

    // FREE EVERYTHING
    CloseWindow();

    return 0;
}