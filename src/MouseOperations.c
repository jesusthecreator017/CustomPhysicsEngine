#include "particle.h"
#include "raylib.h"


void PickUpParticle(Particle particles[])
{
    int condition = 0;

    //Loops through the number of particles.
    for(int k = 0; k < PARTICLE_NUM; k++)
    {
        float distance = sqrtf(powf(particles[k].position.x - GetMouseX(), 2) + powf(particles[k].position.y - GetMouseY(), 2)); //Calculates the distance between the cursor and the particle.
        //If the cursor is within that particles domain, the particles location is changed to the cursor and moves with it.
        if(distance <= particles[k].radius && condition != 1){
                condition = 1;                                          //This prevents the cursor from moving onto another ball, when you haven't released the mouse button.
                particles[k].position.x = GetMouseX();                  //Change particle to X value of mouse location.
                particles[k].position.y = GetMouseY();                  //Change particle to Y value of mouse location.
                particles[k].velocity.x = 0;                            //Change velocity to 0.
                particles[k].velocity.y = 0;                            //Change velocity to 0.
        }

    }
}