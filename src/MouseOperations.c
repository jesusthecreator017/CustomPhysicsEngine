#include "particle.h"
#include "raylib.h"
#include "main.h"


void PickUpParticle(Particle particles[], int * condition)
{
    int index = 0;

    //Loops through the number of particles.
    for(int k = 0; k < PARTICLE_NUM; k++)
    {
        if(particles[k].isGrabbed == 1)
        {
                particles[k].position.x = GetMouseX();                  //Change particle to X value of mouse location.
                particles[k].position.y = GetMouseY();                  //Change particle to Y value of mouse location.
                particles[k].velocity.x = 0;                            //Change velocity to 0.
                particles[k].velocity.y = 0;                            //Change velocity to 0.
        }
        else
        {
            float distance = sqrtf(powf(particles[k].position.x - GetMouseX(), 2) + powf(particles[k].position.y - GetMouseY(), 2)); //Calculates the distance between the cursor and the particle.

            //If the cursor is within that particles domain, the particles location is changed to the cursor and moves with it.
            if(distance <= particles[k].radius && *condition != 1){
                *condition = 1;                                         //Sets the condition to 1, meaning a particle is being grabbed.
                index = k;                                              //Keeps track of the index of the particle that is currently being held.
                particles[k].isGrabbed = 1;                             //This prevents the cursor from moving onto another ball, when you haven't released the mouse button.
                particles[k].position.x = GetMouseX();                  //Change particle to X value of mouse location.
                particles[k].position.y = GetMouseY();                  //Change particle to Y value of mouse location.
                particles[k].velocity.x = 0;                            //Change velocity to 0.
                particles[k].velocity.y = 0;                            //Change velocity to 0.
            }
        }
        

    }

    //Checks to see if the left mouse button is still being held
    if(IsMouseButtonUp(MOUSE_LEFT_BUTTON))
    {
        *condition = 0;                         //Sets condition to 0.                     
        particles[index].isGrabbed = 0;         //Indicates that the particle is no longer being held onto.
    }
}