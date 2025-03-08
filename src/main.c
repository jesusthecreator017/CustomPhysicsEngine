// Import main header file
#include "MouseOperations.h"
#include "sticks.h"

int main(void){
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Test Physics Engine");

    int condition = 0;
    bool press = false;

    
    // Array of particles
    Particle particles[PARTICLE_NUM];
    Stick sticks[STICK_NUM];

    InitParticle(
        &particles[0],
        (Vector2){500, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &particles[1],
        (Vector2){600, 500},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &particles[2],
        (Vector2){600, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &particles[3],
        (Vector2){500, 400},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        false
    );

    InitParticle(
        &particles[4],
        (Vector2){500, 200},
        BLUE,
        GetRandomValue(10, 100),
        0.5f,
        10.0f,
        true
    );


    InitStick(&sticks[0], &particles[0], &particles[1], Vector2Distance(particles[0].position, particles[1].position), Vector2Distance(particles[0].position, particles[1].position) - 10.0f);
    InitStick(&sticks[1], &particles[1], &particles[2], Vector2Distance(particles[1].position, particles[2].position), Vector2Distance(particles[1].position, particles[2].position) - 10.0f);
    InitStick(&sticks[2], &particles[2], &particles[3], Vector2Distance(particles[2].position, particles[3].position), Vector2Distance(particles[2].position, particles[3].position) - 10.0f);
    InitStick(&sticks[5], &particles[4], &particles[2], Vector2Distance(particles[4].position, particles[2].position), Vector2Distance(particles[4].position, particles[2].position) - 10.0f);
    InitStick(&sticks[3], &particles[3], &particles[0], Vector2Distance(particles[3].position, particles[0].position), Vector2Distance(particles[3].position, particles[0].position) - 10.0f);
    InitStick(&sticks[4], &particles[0], &particles[2], Vector2Distance(particles[0].position, particles[2].position), Vector2Distance(particles[0].position, particles[2].position) / 2);

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

        // Update all sticks
        for (int j = 0; j < STICK_NUM; j++) {
            sticks[j].Update(&sticks[j]);
        }
        

        //Checks to see whether the user clicked on the left mouse button. 
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            PickUpParticle(particles, &condition); //(Found in MouseOperations.c)
        }
        else {
            //No particle is currently being picked up, therefore it resets every particle value to the beginning state.
            condition = 0;
            for(int a = 0; a < PARTICLE_NUM; a++)
            {
                particles[a].isGrabbed = 0;
            }
        }
                
        // Resolve collisions
        for (int j = 0; j < PARTICLE_NUM; j++) {
            for (int k = j + 1; k < PARTICLE_NUM; k++) {
                if (ParticleVsParticle(&particles[j], &particles[k])) {
                    ResolveCollision(&particles[j], &particles[k]);
                }
            }
        }
        
        
        // Render Logic Here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);

            if(press != 1) DrawText("[a] To Enter Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4, 25, BLUE); //Draws this when the program is in an inactive state (still in menu mode).
            else DrawText("[z] To Exit Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4 + 50, 25, BLUE);       //Draws this when the program is active. 

            int charPress = GetCharPressed();       //Gets the unicode value of the char that was pressed on the keyboard.

            //If the unicode is 97 ('a') then it draws the pendulum.
            if(charPress == 97 || press == true)      
            {
                if(charPress == 122) press = false;     //If the unicode is 122 ('z') then it goes into the inactive state.
                else
                {
                    //Global variable indicating program is active.
                    press = true;
                    
                    // Render Objects Here
                    for(int i = 0; i < STICK_NUM; i++){
                        sticks[i].Render(&sticks[i]);
                    }

                    for(int i = 0; i < PARTICLE_NUM; i++){
                        Particle* current = &particles[i];
                        current->Render(current);
                    }
                    
                    // Draw Debug info ontop of everything
                    if(condition == 1) ObjectInfo(particles[1].info, particles[1].type);
                }
            }
            
            
        EndDrawing();
    }
    
    // FREE EVERYTHING
    CloseWindow();

    return 0;
}