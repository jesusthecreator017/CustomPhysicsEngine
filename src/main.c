#include "engine.h"

int main(void) {
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Physics Engine");
    
    // Create and initialize engine
    Engine engine;
    InitEngine(&engine, PARTICLE_NUM, STICK_NUM);
    
    // Main Game Loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Handle user input
        engine.HandleInput(&engine);
        
        // Update physics
        engine.Update(&engine, dt);
        
        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
            
            // Draw UI text
            if (!engine.isActive) {
                DrawText("[a] To Enter Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4, 25, BLUE);
            } else {
                DrawText("[z] To Exit Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4 + 50, 25, BLUE);
                
                // Render physics objects
                engine.Render(&engine);
            }
        EndDrawing();
    }
    
    // Clean up
    engine.Shutdown(&engine);
    CloseWindow();
    
    return 0;
}