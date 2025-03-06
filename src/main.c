// Import main header file
#include "particle.h"
#include "MouseOperations.h"
#include "sticks.h"

int main(void){
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Test Physics Engine");

    int condition = 0;


    // Array of particles
    Particle particles[PARTICLE_NUM];
    Stick sticks[STICK_NUM];

    InitParticle(
        &particles[0],
        (Vector2){500, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.1f,
        10.0f,
        false
    );

    InitParticle(
        &particles[1],
        (Vector2){600, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.1f,
        10.0f,
        false
    );

    InitParticle(
        &particles[2],
        (Vector2){600, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.1f,
        10.0f,
        false
    );

    InitParticle(
        &particles[3],
        (Vector2){500, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.1f,
        10.0f,
        false
    );


    InitStick(&sticks[0], &particles[0].position, &particles[1].position, 100.0f);
    InitStick(&sticks[1], &particles[1].position, &particles[2].position, 100.0f);
    InitStick(&sticks[2], &particles[2].position, &particles[3].position, 100.0f);
    InitStick(&sticks[3], &particles[3].position, &particles[0].position, 100.0f);
    InitStick(&sticks[4], &particles[0].position, &particles[2].position, 141.0f);

    // Main Game Loop
    while (!WindowShouldClose()){
        // Deltat time is used everywhere
        float dt = GetFrameTime();
        dt = fminf(dt, 0.016f); // Clamp the DT to 60 FPS

        // Apply Constraints
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            ConstrainParticle(current);
        }

        // Update
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            current->Update(current, dt);
        }
        

        for(int i = 0; i < STICK_NUM; i++){
            sticks[i].Update(&sticks[i]);
        }

        //Checks to see whether the user clicked on the left mouse button. 
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            PickUpParticle(particles, &condition); //(Found in MouseOperations.c)
        }
        else
        {
            condition = 0;
            for(int a = 0; a < PARTICLE_NUM; a++)
            {
                particles[a].isGrabbed = 0;
            }
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
        
        // Render Logic Here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
            
            // Render Objects Here
            for(int i = 0; i < STICK_NUM; i++){
                sticks[i].Render(&sticks[i]);
            }

            for(int i = 0; i < PARTICLE_NUM; i++){
                Particle* current = &particles[i];
                current->Render(current);
            }
            
            // Draw Debug info ontop of everything
            ObjectInfo(particles[1].info, particles[1].type);
        EndDrawing();
    }
    
    // FREE EVERYTHING
    CloseWindow();

    return 0;
}