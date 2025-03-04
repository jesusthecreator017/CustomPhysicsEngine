// Import main header file
#include "particle.h"


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
            .5f,                                                                                    // Restitution
            GetRandomValue(5, 25)                                                                   // Radius
        );
    }

    // Main Game Loop
    while (!WindowShouldClose()){
        // Deltat time is used everywhere
        float dt = GetFrameTime();
        //Vector2 mousePos = GetMousePosition();

        // Update
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            current->Update(current, dt);
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