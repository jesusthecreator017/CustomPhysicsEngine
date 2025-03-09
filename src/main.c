#include "engine.h"

int main(void) {
    // Initialize Window
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Physics Engine");
    
    // Create and initialize engine
    Engine engine;
    InitEngine(&engine, PARTICLE_NUM, STICK_NUM);
    
    // Initialize the first scene (explicitly call it here to be safe)
    engine.InitScene(&engine, SCENE_MENU);
    
    // Main Game Loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Handle user input
        engine.HandleInput(&engine);
        
        // Update physics (only if a scene is active)
        if (engine.isActive) {
            engine.Update(&engine, dt);
        }
        
        // Update scene-specific logic (check for NULL to prevent segfault)
        if (engine.UpdateScene != NULL) {
            engine.UpdateScene(&engine, dt);
        }
        
        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(GetScreenWidth() - 100, 10);
            
            // Render the current scene (check for NULL to prevent segfault)
            if (engine.RenderScene != NULL) {
                engine.RenderScene(&engine);
            } else {
                // Fallback rendering if RenderScene isn't available
                if (engine.isActive) {
                    engine.Render(&engine);
                } else {
                    DrawText("Scene system not initialized", 20, 20, 20, RED);
                }
            }
        EndDrawing();
    }
    
    // Clean up
    engine.Shutdown(&engine);
    CloseWindow();
    
    return 0;
}
