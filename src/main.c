// Import main header file
#include "particle.h"
#include "raylib.h"

Particle particles[MAX_PARTICLES];

int main(void){
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Test Physics Engine");

    // Initialize Particles
    initializeParticles(particles, PARTICLE_NUM);

    // Main Game Loop
    while (!WindowShouldClose()){
        // Deltat time is used everywhere
        float dt = GetFrameTime();

        // Update Logic Here
        updateParticles(particles, PARTICLE_NUM, dt);

        // Apply Constraints
        constrainParticles(particles, PARTICLE_NUM);

        // Render Logic Here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
            DrawText("Test Physics Engine", 10, 10, 20, BLACK);

            renderParticles(particles, PARTICLE_NUM);
        EndDrawing();
    }
    

    // FREE EVERYTHING
    CloseWindow();
    return 0;
}