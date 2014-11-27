#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	// TODO
	//set random seed
	srand(time(NULL));
	// set number of particles
	n = 1000;
	particles = new Particle[n];
}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	delete[] particles;

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	// TODO
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{

	// TODO
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{

	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
}

// functions from the given pdf (Physically Based Modeling: Principles and Practice)
/* gather state from the particles into dst */
Particle* ParticleSystem::getState(float *dst){
	for (int i = 0; i < n; i++){
		*(dst++) = particles[i].getPositionVectors[0];
		*(dst++) = particles[i].getPositionVectors[1];
		*(dst++) = particles[i].getPositionVectors[2];
		*(dst++) = particles[i].getVelocityVectors[0];
		*(dst++) = particles[i].getVelocityVectors[1];
		*(dst++) = particles[i].getVelocityVectors[2];
	}
}

/* scatter state from src into the particles */
void ParticleSystem::setState(float *src){
	for (int i = 0; i < n; i++){
		particles[i].getPositionVectors[0] = *(src++);
	    particles[i].getPositionVectors[1] = *(src++);
		particles[i].getPositionVectors[2] = *(src++);
		particles[i].getVelocityVectors[0] = *(src++);
		particles[i].getVelocityVectors[1] = *(src++);
		particles[i].getVelocityVectors[2] = *(src++);
	}
}




