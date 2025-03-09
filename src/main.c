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
        
        if (!engine.isActive) {
            DrawText("Welcome to our Custom Physics Engine!", GetScreenWidth()/2 - 300, GetScreenHeight()-550, 30, BLUE);
            DrawText("Created by Jesus, Chase, Alvaro.", GetScreenWidth()/2 - 300, GetScreenHeight()-450, 25, BLUE);
            DrawText("Version 0.1 - 03/09", GetScreenWidth()/2 - 300, GetScreenHeight()-400, 25, BLUE);
            DrawText("[a] To Enter Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4 + 300, 25, BLUE);
        } else {
            DrawText("[z] To Exit Pendulum Mode", GetScreenWidth()/2 - 200, GetScreenHeight()/4 + 300, 25, BLUE);
            engine.Render(&engine);
        }
        
        EndDrawing();
    }
    
    // Clean up
    engine.Shutdown(&engine);
    CloseWindow();
    
    return 0;
}
