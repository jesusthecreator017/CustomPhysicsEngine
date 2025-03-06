// Import main header file
#include "particle.h"
#include "raylib.h"
#include "MouseOperations.h"

int main(void){
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Test Physics Engine");

    // Array of particles
    //Particle particles[MAX_PARTICLES];

    //int condition = 0;

    /*
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
    */

    // Test Particles
    Particle p1;
    Particle p2;

    InitParticle(
        &p1,                                                                                    // Particle Reference
        (Vector2){300, 300},                                                                    // Position
        (Vector2){300, 300},
        (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255},   // Color
        GetRandomValue(5, 20),                                                                  // Mass
        0.5f,                                                                                   // Restitution (Bounciness)
        GetRandomValue(10, 30)                                                                   // Radius
    );

    InitParticle(
        &p2,                                                                                    // Particle Reference
        (Vector2){400, 500},                                                                    // Position
        (Vector2){400, 500},
        (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255},   // Color
        GetRandomValue(2, 15),                                                                  // Mass
        0.5f,                                                                                   // Restitution (Bounciness)
        GetRandomValue(5, 25)                                                                   // Radius
    );


    // Main Game Loop
    while (!WindowShouldClose()){
        // Deltat time is used everywhere
        float dt = GetFrameTime();

        ConstrainParticle(&p1);
        ConstrainParticle(&p2);
        
        p1.Update(&p1, dt);
        p2.Update(&p2, dt);

        

        // Update
        /*
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            current->Update(current, dt);
        }

        //Checks to see whether the user clicked on the left mouse button. 
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            PickUpParticle(particles, &condition); //(Found in MouseOperations.c)
        }
        */
        // Check Collisions
        /*
        for (int i = 0; i < PARTICLE_NUM; i++) {
            for (int j = i + 1; j < PARTICLE_NUM; j++) {  // Compare each pair only once
                Particle* p1 = &particles[i];
                Particle* p2 = &particles[j];
                
                if (ParticleVsParticle(p1, p2)) {
                    ResolveCollision(p1, p2);
                }
            }
        }
            */
        /*
        // Apply Constraints
        for(int i = 0; i < PARTICLE_NUM; i++){
            Particle* current = &particles[i];
            ConstrainParticle(current);
        }
        */
       
        // Render Logic Here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
        
            DrawLineEx((Vector2){0, 610}, (Vector2){1300, 610}, 20, BLUE);
            p1.Render(&p1);
            p2.Render(&p2);

            // Debug Rendering
            //ObjectInfo(particles[1].info, particles[1].type);

            // Render Objects Here
            /*
            for(int i = 0; i < PARTICLE_NUM; i++){
                Particle* current = &particles[i];
                current->Render(current);
            }
            */

            
        EndDrawing();
    }
    

    // FREE EVERYTHING
    CloseWindow();

    return 0;
}