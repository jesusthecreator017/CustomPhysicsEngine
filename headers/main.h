// Standard Includes
#include <stdio.h>

// Custom Includes
#include "raylib.h"
#include "raymath.h"

// Defines
#define CONSTRAINT_ITERATIONS 3;
#define MAX_PARTICLES 1000
#define PARTICLE_NUM 5
#define STICK_NUM 6
#define GRAVITY 200.81f
#define DAMPING_FACTOR .999f


// Defines
#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720
#define FPS 60