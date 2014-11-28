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
	// TODO - done
	//set random seed
	srand(time(NULL));
	// set baked fps
	bake_fps = 10;

	// set number of initial_state
	n = 300;
    particles = new Particle[n];

	simulate = false;
	// store the initial state, deep copy
	initial_state = new Particle[n];
	for (int i = 0; i < n; i++){
		float newPosVectors[3] = { particles[i].getPositionVectors()[0], particles[i].getPositionVectors()[1], particles[i].getPositionVectors()[2] };
		float newVelVectors[3] = { particles[i].getVelocityVectors()[0], particles[i].getVelocityVectors()[1], particles[i].getVelocityVectors()[2] };
		float newForVectors[3] = { particles[i].getForceVectors()[0], particles[i].getForceVectors()[1], particles[i].getForceVectors()[2] };
		initial_state[i].setPositionVectors(newPosVectors);
		initial_state[i].setVelocityVectors(newVelVectors);
		initial_state[i].setForceVectors(newForVectors);
	}
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO - done
	delete[] particles;
	delete[] initial_state;

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	bake_start_time = t;

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
	bake_end_time = t;
    resetSimulation(t);
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// reset the particle status to its initial state
	for (int i = 0; i < n; i++){
		float newPosVectors[3] = { initial_state[i].getPositionVectors()[0], initial_state[i].getPositionVectors()[1], initial_state[i].getPositionVectors()[2] };
		float newVelVectors[3] = { initial_state[i].getVelocityVectors()[0], initial_state[i].getVelocityVectors()[1], initial_state[i].getVelocityVectors()[2] };
		float newForVectors[3] = { initial_state[i].getForceVectors()[0], initial_state[i].getForceVectors()[1], initial_state[i].getForceVectors()[2] };
		particles[i].setPositionVectors(newPosVectors);
		particles[i].setVelocityVectors(newVelVectors);
		particles[i].setForceVectors(newForVectors);
	}
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update initial_state **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	// TODO
	if (simulate){
		// no need to update if the particles are already baked
		float roundedTime = round(t*bake_fps) / bake_fps;
		if (bakedParticles.count(roundedTime)){
			return;
		}

		// compute new force

		for (int i = 0; i < n; i++){
			// clear force accumulator
			float newForVectors[3] = { 0.0, 0.0, 0.0 };
			particles[i].setPositionVectors(newForVectors);

			float* derivatives = new float[6 * n];
			float totalForce[3] = { 0.0, 0.0, 0.0 };
			// gravity force
			float gravityForce[3] = { 0.0, particles[i].getMass() * (-1) * 9.8, 0.0 };
			totalForce[0] += gravityForce[0];
			totalForce[1] += gravityForce[1];
			totalForce[2] += gravityForce[2];

			// air drag
			float airDragCoeff = 10;
			float airDrag[3] = { airDragCoeff*particles[i].getVelocityVectors()[0], airDragCoeff*particles[i].getVelocityVectors()[1], airDragCoeff*particles[i].getVelocityVectors()[2] };
			totalForce[0] += airDrag[0];
			totalForce[1] += airDrag[1];
			totalForce[2] += airDrag[2];

			//update particles derivatives
			derivatives[0] = particles[i].getVelocityVectors()[0];
			derivatives[1] = particles[i].getVelocityVectors()[1];
			derivatives[2] = particles[i].getVelocityVectors()[2];
			derivatives[3] = totalForce[0] / particles[i].getMass();
			derivatives[4] = totalForce[1] / particles[i].getMass();
			derivatives[5] = totalForce[2] / particles[i].getMass();

			float newPosVectors[3];
			float newVelVectors[3];
			newPosVectors[0] = particles[i].getPositionVectors()[0] + 1.0 / bake_fps*derivatives[0];
			newPosVectors[1] = particles[i].getPositionVectors()[1] + 1.0 / bake_fps*derivatives[1];
			newPosVectors[2] = particles[i].getPositionVectors()[2] + 1.0 / bake_fps*derivatives[2];
			newVelVectors[0] = particles[i].getVelocityVectors()[0] + 1.0 / bake_fps*derivatives[3];
			newVelVectors[1] = particles[i].getVelocityVectors()[1] + 1.0 / bake_fps*derivatives[4];
			newVelVectors[2] = particles[i].getVelocityVectors()[2] + 1.0 / bake_fps*derivatives[5];

			// reset the position of the particles if it exceeds the limit
			double limit = 5.0;
			if (newPosVectors[0] > limit || newPosVectors[1] > limit || newPosVectors[2] > limit){
				newPosVectors[0] = initial_state[i].getPositionVectors()[0];
				newPosVectors[1] = initial_state[i].getPositionVectors()[1];
				newPosVectors[2] = initial_state[i].getPositionVectors()[2];
				newVelVectors[0] = initial_state[i].getVelocityVectors()[0];
				newVelVectors[1] = initial_state[i].getVelocityVectors()[1];
				newVelVectors[2] = initial_state[i].getVelocityVectors()[2];
			}
			// update particles state
			particles[i].setPositionVectors(newPosVectors);
			particles[i].setVelocityVectors(newVelVectors);
			particles[i].setForceVectors(totalForce);

			delete[] derivatives;
		}
		

		bakeParticles(roundedTime);
	}

}


/** Render initial_state */
void ParticleSystem::drawParticles(float t)
{
	if (simulate){
		float roundedTime = round(t*bake_fps) / bake_fps;
		Particle* p = particles;
		// check if the initial_state are already baked at this time
		if (!bakedParticles.empty() && bakedParticles.count(roundedTime)){
			p = bakedParticles.at(roundedTime);
		}

		// draw shape
		double size = 0.05;
		float grayColor = (rand() % 100) / 100.0;
		setDiffuseColor(grayColor,grayColor,grayColor);
		for (int i = 0; i < n; i++){
			float* location = p[i].getPositionVectors();
			glPushMatrix();
			glTranslated(location[0], location[1], location[2]);
			glTranslated(-size / 2, -size / 2, -size / 2);
			drawSphere(size);
			//drawBox(size, size, size);
			glPopMatrix();
		}
	}

}



/** Adds the current configuration of initial_state to
  * your data structure for storing baked initial_state **/
void ParticleSystem::bakeParticles(float t) 
{
	
	// insert configuration of current initial_state to the bakedParticles
	float roundedTime = round(t*bake_fps) / bake_fps;
	Particle* currentParticles = new Particle[n];
	for (int i = 0; i < n; i++){
		float newPosVectors[3] = { particles[i].getPositionVectors()[0], particles[i].getPositionVectors()[1], particles[i].getPositionVectors()[2] };
		float newVelVectors[3] = { particles[i].getVelocityVectors()[0], particles[i].getVelocityVectors()[1], particles[i].getVelocityVectors()[2] };
		float newForVectors[3] = { particles[i].getForceVectors()[0], particles[i].getForceVectors()[1], particles[i].getForceVectors()[2] };
		currentParticles[i].setPositionVectors(newPosVectors);
		currentParticles[i].setVelocityVectors(newVelVectors);
		currentParticles[i].setForceVectors(newForVectors);
	}
	bakedParticles.insert(std::pair<float, Particle*>(roundedTime, currentParticles));
}

/** Clears out your data structure of baked initial_state */
void ParticleSystem::clearBaked()
{

	// TODO
	for (std::map<float, Particle*>::iterator it = bakedParticles.begin(); it != bakedParticles.end(); ++it){
		delete[] it->second;
	}
	bakedParticles.clear();
}

// functions from the given pdf (Physically Based Modeling: Principles and Practice)
/* gather state from the initial_state into dst */
void ParticleSystem::getState(float *dst){
	for (int i = 0; i < n; i++){
		*(dst++) = particles[i].getPositionVectors()[0];
		*(dst++) = particles[i].getPositionVectors()[1];
		*(dst++) = particles[i].getPositionVectors()[2];
		*(dst++) = particles[i].getVelocityVectors()[0];
		*(dst++) = particles[i].getVelocityVectors()[1];
		*(dst++) = particles[i].getVelocityVectors()[2];
	}
}

/* scatter state from src into the initial_state */
void ParticleSystem::setState(float *src){
	for (int i = 0; i < n; i++){
		particles[i].setPositionVectors(src);
		src += 3;
		particles[i].setVelocityVectors(src);
	}
}




