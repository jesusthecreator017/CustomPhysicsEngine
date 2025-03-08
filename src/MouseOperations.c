#include "MouseOperations.h"

void PickUpParticle(Particle particles[], int* condition){
    static Vector2 prevMousePos = {0, 0};
    Vector2 currentMousePos = {GetMouseX(), GetMouseY()};
    
    for (int k = 0; k < PARTICLE_NUM; k++)
    {
        if (particles[k].isGrabbed == 1)
        {
            // Calculate a reasonable velocity based on mouse movement
            Vector2 mouseVelocity = Vector2Subtract(currentMousePos, prevMousePos);
            
            // Update particle position
            particles[k].position = currentMousePos;
            
            // Update oldPosition to create appropriate velocity
            particles[k].oldPosition = Vector2Subtract(particles[k].position, mouseVelocity);
        }
        else
        {
            float distance = sqrtf(powf(particles[k].position.x - GetMouseX(), 2) + 
                                  powf(particles[k].position.y - GetMouseY(), 2));

            if (distance <= particles[k].radius && *condition != 1)
            {
                *condition = 1;
                particles[k].isGrabbed = 1;
                
                // When grabbing, keep oldPosition close to maintain low velocity
                particles[k].oldPosition = particles[k].position;
                particles[k].position = currentMousePos;
            }
        }
    }
    
    // Update previous mouse position for next frame
    prevMousePos = currentMousePos;
}