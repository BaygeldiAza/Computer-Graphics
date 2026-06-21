// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// particlesys.h
//
// This file declares the class for a particle system.  Note that this class is normally
// subclassed


#ifndef _particlesys_H
#define _particlesys_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"
#include "particle.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class particlesys : public object3d
{
	protected:
		particle		*particles;						// array of particles

		unsigned int    particleCount;					// the total number of particles
		unsigned int	aliveAtStart;					// the number of particles alive when the system is inited
		bool			shuttingDown;					// set true to prevent new births
		
		float			minLifeSpan;					// minimum time a particle can live
		float			maxLifeSpan;					// maximum time a particle can live
		
		float			minBirthSize;					// smallest size of a particle when it is created
		float			maxBirthSize;					// largest size of a particle when it is created
		float			maxSize;						// maximum size a particle can be
		
		float			minBirthSpeed;					// minimum speed of a particle when created
		float			maxBirthSpeed;					// maximum speed of a particle when created
		float			minMaxSpeed;					// (least) maximum speed a particle can have
		float			maxMaxSpeed;					// (greatest) maximum speed a particle can have
		
		float			newParticlesToEmit;				// no. of particles to emit during a single animate call
		float			birthRate;						// no. of new particles emitted per second
														
		bool			acceleration;					// acceleration switch
		vectr			accelerationVectr;				// acceleration vector
		
		bool			useAlpha;						// controls blending mode (add/blend)
		
		int             framesWide;						// sub-image arrangement in the texture
		int             framesHigh;
			
				
				
	public:
						particlesys(unsigned int particleCount);	// construct a system of n particles
		virtual			~particlesys();								// destructor
		
		virtual void	initSystem(void);					// resets the entire particle system
		virtual void	initParticle(unsigned int i);		// resets a single particle
		virtual void	setBirthRate(float theRate)			// changes the birth rate 
						{ birthRate = theRate; }
		virtual void	animate(void);						// animates each particle's position, etc.
				void	runDown(void)						// prevent future particle births (let system die off)
						{ shuttingDown = true; }
							
		virtual void	privateDraw(bool drawMode);

};


#endif // _particlesys_H

