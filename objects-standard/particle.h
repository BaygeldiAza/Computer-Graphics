// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// particle.h
//
// This file declares the class for a single particle.  Used by the "particlesys" class.


#ifndef _particle_H
#define _particle_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "vectr.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class particle
{
	public:
				vectr	position;			// the position of the particle
				vectr	velocity;			// the velocity (signed speed in each direction) of the particle
				float	maxSpeed;			// how fast the particle can move
				float	age;				// how long the particle has been alive
				float	lifeSpan;			// how long the particle will be alive
				float	size;				// a size multiplier for the particle (uniform for X and Y)
				float	growthRate;			// how much the particle can grow (per second)

						particle(void);		// constructor
		virtual			~particle();		// destructor
};


#endif // _particle_H

