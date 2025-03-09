#include "debug.h"

// Takes in the info about a particle, and returns it accordingly
void ObjectInfo(DebugData data, ObjectType type){
    if(type == TYPE_PARTICLE){
        // Draw Background Rec First
        DrawRectangleV((Vector2){5, 5}, (Vector2){400, 100}, (Color){100, 100, 100, 50});
        DrawText(TextFormat("Position: (%.2f, %.2f)", data.position.x, data.position.y), 10, 10, 21, BLACK);
        DrawText(TextFormat("Velocity: (%.2f, %.2f)", data.velocity.x, data.velocity.y), 10, 30, 21, BLACK);
        DrawText(TextFormat("Color: (R: %i, G: %i, B: %i, A: %i)", data.color.r, data.color.g, data.color.b, data.color.a), 10, 50, 21, BLACK);
        DrawText(TextFormat("Radius: %.2f", data.radius), 10, 70, 21, BLACK);
    } else {
        return;
    }
}