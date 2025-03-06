#include "particle.h"
#include "main.h"

void PickUpParticle(Particle particles[], int * condition)
{
    //Loops through the number of particles.
    for(int k = 0; k < PARTICLE_NUM; k++)
    {
        if(particles[k].isGrabbed == 1)
        {
            particles[k].position.x = GetMouseX();                  //Change particle to X value of mouse location.
            particles[k].position.y = GetMouseY();                  //Change particle to Y value of mouse location.
                 
        }
        else
        {
            float distance = sqrtf(powf(particles[k].position.x - GetMouseX(), 2) + powf(particles[k].position.y - GetMouseY(), 2)); //Calculates the distance between the cursor and the particle.

            //If the cursor is within that particles domain, the particles location is changed to the cursor and moves with it.
            if(distance <= particles[k].radius && *condition != 1){
                *condition = 1;                                         //Sets the condition to 1, meaning a particle is being grabbed.
                particles[k].isGrabbed = 1;                             //This prevents the cursor from moving onto another ball, when you haven't released the mouse button.
                particles[k].position.x = GetMouseX();                  //Change particle to X value of mouse location.
                particles[k].position.y = GetMouseY();                  //Change particle to Y value of mouse location.
               
            }
        }
    }
}

    