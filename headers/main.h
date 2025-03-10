// Standard Includes
#include <stdio.h>

// Custom Includes
#include "raylib.h"
#include "raymath.h"

// Defines
#define MAX_PARTICLES 1000
#define PARTICLE_NUM 5
#define STICK_NUM 6
#define GRAVITY 200.81f
#define DAMPING_FACTOR .999f

// Physics Defines
#define PHYSICS_TIME_STEP 1.0f/60.0f  // Fixed time step at 60Hz
#define MAX_PHYSICS_STEPS 5           // Maximum number of physics steps per frame to prevent spiral of death
#define CONSTRAINT_ITERATIONS 8       // Number of iterations for constraint solving
#define MAX_VELOCITY 1.0f

// Common Vectors
#define V_ZERO (Vector2){0, 0}
#define V_ONE (Vector2){1, 1}

// Defines
#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720
#define FPS 60