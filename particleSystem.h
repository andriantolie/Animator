/***********************
 * ParticleSystem class
 ***********************/

/**
 * The particle system class simply "manages" a collection of particles.
 * Its primary responsibility is to run the simulation, evolving particles
 * over time according to the applied forces using Euler's method.
 * This header file contains the functions that you are required to implement.
 * (i.e. the rest of the code relies on this interface)
 * In addition, there are a few suggested state variables included.
 * You should add to this class (and probably create new classes to model
 * particles and forces) to build your system.
 */

#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "vec.h"
#include <map>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <FL/gl.h>
#include "modelerdraw.h"

// data structure for the particle
class Particle{
	public:
		Particle(){
			// build a particle with random mass, velocity and force (velocity and force are always going upward)
			// I'm not really sure whether this initial velocity and force could give the intended behaviour, feel free to change its initial value
			m = (rand() % 33 + 33) / 100.0;
			x[0] = 0.0;
			x[1] = 0.0;
			x[2] = 0.0;
			v[0] = (rand() % 101) / 100.0;
			v[1] = (rand() % 6)/10.0 + 3;
			v[2] = (rand() % 101 - 50) / 50.0;
			f[0] = 0.0;
			f[1] = 0.0;
			f[2] = 0.0;
		}
		float getMass() { return m; }
		float* getPositionVectors(){ return x; }
		float* getVelocityVectors(){ return v; }
		float* getForceVectors(){ return f; }
		void setPositionVectors(float* newX){ 
			x[0] = newX[0]; 
			x[1] = newX[1];
			x[2] = newX[2];
		}
		void setVelocityVectors(float* newV){
			v[0] = newV[0];
			v[1] = newV[1];
			v[2] = newV[2];
		}
		void setForceVectors(float* newF){
			f[0] = newF[0];
			f[1] = newF[1];
			f[2] = newF[2];
		}
	private:
		float m; /* mass */
		float x[3]; /* position vector */
		float v[3]; /* velocity vector */
		float f[3]; /* force accumulator*/
};

class ParticleSystem {

public:

	/** Constructor **/
	ParticleSystem();


	/** Destructor **/
	virtual ~ParticleSystem();


	// functions from the given pdf (Physically Based Modeling: Principles and Practice)
	void getState(float *dst);
	void setState(float *src);

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawParticles(float t);

	// This fxn should save the configuration of all particles
	// at current time t.
	virtual void bakeParticles(float t);

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	virtual void computeForcesAndUpdateParticles(float t);

	// This function should reset the system to its initial state.
	// When you need to reset your simulation, PLEASE USE THIS FXN.
	// It sets some state variables that the UI requires to properly
	// update the display.  Ditto for the following two functions.
	virtual void resetSimulation(float t);

	// This function should start the simulation
	virtual void startSimulation(float t);

	// This function should stop the simulation
	virtual void stopSimulation(float t);

	// This function should clear out your data structure
	// of baked particles (without leaking memory).
	virtual void clearBaked();	



	// These accessor fxns are implemented for you
	float getBakeStartTime() { return bake_start_time; }
	float getBakeEndTime() { return bake_end_time; }
	float getBakeFps() { return bake_fps; }
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }



protected:
	
	int n;	// number of particles
	Particle* particles; // particles contained
	Particle* initial_state; // initial state of the contained particles
	std::map<float,Particle*> bakedParticles; //container of baked particles



	/** Some baking-related state **/
	float bake_fps;						// frame rate at which simulation was baked
	float bake_start_time;				// time at which baking started 
										// These 2 variables are used by the UI for
										// updating the grey indicator 
	float bake_end_time;				// time at which baking ended


	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)

};


#endif	// __PARTICLE_SYSTEM_H__
