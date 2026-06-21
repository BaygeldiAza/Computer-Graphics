// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// particlesys.cpp
//
// This file defines the class for particlesys objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "constants.h"
#include "particlesys.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern unsigned long	gPolygonCount;
extern bool				gWireFrameOnly;			// if true, only draw the outline of the particles
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

particlesys::particlesys(unsigned int n)
{
	setName("particlesys");

	// the numbers here are pretty arbitrary - most should be overridden
	// in the constructor of derived particle systems
	
	particleCount = n;
	particles = new particle [particleCount];
	aliveAtStart = 0;
	
	minLifeSpan = 1.0;
	maxLifeSpan = 2.0;
	
	maxSize = 0.8;
	minBirthSize = 0.05;
	maxBirthSize = 0.3;
	
	minMaxSpeed = 1.2;
	maxMaxSpeed = 1.6;
	minBirthSpeed = 0.2;
	maxBirthSpeed = 0.4;
	
	birthRate = 100;
	
	acceleration = true;
	accelerationVectr.set(0.0, -2.0, 0.0);
	
	useAlpha = false;
	shuttingDown = false;
	
	framesWide = 1;
	framesHigh = 1;
	
	objectOpacity = kTransparentMode;
	
	autoBindToTexture = false;
	
	initSystem();
	
	// put the system onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

particlesys::~particlesys()
{
	delete [] particles;
}




// -----------------------------------------------------------------------------------------
// (re)initialise the entire particle system
// -----------------------------------------------------------------------------------------

void particlesys::initSystem(void)
{
	for (unsigned int i=0; i < aliveAtStart; i++)
		initParticle(i);
		
	for (unsigned int i=aliveAtStart; i < particleCount; i++)
		particles[i].lifeSpan = 0.0;
	
	newParticlesToEmit = 0;
	
	shuttingDown = false;
}




// -----------------------------------------------------------------------------------------
// (re)initialise a single particle
// -----------------------------------------------------------------------------------------

void particlesys::initParticle(unsigned int i)
{
	vectr direction;
	
	// a default particle starts at the origin, but with a random XY directions, Z-clamped
	particles[i].position.set(0, 0, 0);
	direction.set(randomFloat(-1.0, 1.0), randomFloat(-1.0, 1.0), 0.0);
	direction.reduceToUnit();

	// set velocity from speed and direction
	float speed = randomFloat(minBirthSpeed, maxBirthSpeed);
	particles[i].velocity = direction * speed;
	particles[i].maxSpeed = randomFloat(minMaxSpeed, maxMaxSpeed);
	
	// set the particle's initial age, and the maximum time it can live
	particles[i].age = 0.0;
	particles[i].lifeSpan = randomFloat(minLifeSpan, maxLifeSpan);
	
	// set the particle's initial size, and growth rate over one second
	// (growth rate is additive, not multiplicative)
	particles[i].size = randomFloat(minBirthSize, maxBirthSize);
	particles[i].growthRate = 0.5;
}




// -----------------------------------------------------------------------------------------
// animate the particles
// -----------------------------------------------------------------------------------------

void particlesys::animate(void)
{
	
	// determine how many new particles we have to emit in this session
	newParticlesToEmit += birthRate * kTimeBetweenAnimates;
	if (newParticlesToEmit > particleCount)
		newParticlesToEmit = particleCount;
	
	
	for (unsigned int i=0; i < particleCount; i++)
	{
		if (particles[i].lifeSpan > 0)
		{
			// move the particle according to its current velocity 
			vectr distanceThisFrame = particles[i].velocity * kTimeBetweenAnimates;
			particles[i].position = particles[i].position + distanceThisFrame;

			// age the particle
			particles[i].age += kTimeBetweenAnimates;
			if (particles[i].age >= particles[i].lifeSpan)
				particles[i].lifeSpan = 0.0;						// kill off
			
			// grow the particle
			if (particles[i].size < maxSize)
				particles[i].size += (particles[i].growthRate * kTimeBetweenAnimates);
			
			// change velocity if acceleration is being modelled
			if (acceleration)
			{
				vectr accelerationEffectVectr = accelerationVectr * kTimeBetweenAnimates;
				// accelerate then check for overspeed
				particles[i].velocity = particles[i].velocity + accelerationEffectVectr;
				// slightly obtuse approach because float argument to vector multiply by float
				// is passed by reference instead of value 
				float speedRatio = particles[i].maxSpeed / particles[i].velocity.length();
				if(speedRatio < 1.0)
					particles[i].velocity = particles[i].velocity * speedRatio;
			}
		}
		else
		
		{
			// consider giving birth to a new particle
			if (!shuttingDown && (newParticlesToEmit > 1.0))
			{
				initParticle(i);
				newParticlesToEmit -= 1.0;
			}
		}
	}
}



// -----------------------------------------------------------------------------------------
// custom draw method
// -----------------------------------------------------------------------------------------

void particlesys::privateDraw(bool drawMode)
{

	// if a texture has been defined, bind to it and set the appropriate wrap and filter options
	if (theTexture && !gWireFrameOnly)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, theTexture->id());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, theTexture->bestMagFilter());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, theTexture->bestMinFilter());
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(textureScaleX, textureScaleY, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}
	
	
	// begin billboarding
	beginSphericalBillboard(position);


	glDisable(GL_LIGHTING);
	
	// changing the blending mode here might be dangerous - it might alter the way objects
	// drawn after this object are rendered
	if (useAlpha)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	
	glBegin(GL_QUADS);
	for (unsigned int i=0; i < particleCount; i++)
	{
		if (particles[i].lifeSpan > 0.0)
		{
			float size = particles[i].size;
			float texWidth = (1.0 / framesWide);
			float texLeft = texWidth * (i % framesWide);
			float texRight = texLeft + texWidth;
			float texBottom = 0.0;
			float texTop = 1.0;
			float alpha;
			
			// set the alpha value so that particles fade in for the first .25 seconds of their lives,
			// and from that point fade out slowly over the rest of their lives
			if (particles[i].age < .25)
				alpha = (particles[i].age / .25);
			else
				alpha = 1.0-(particles[i].age/particles[i].lifeSpan);
			
			glColor4f(1.0, 1.0, 1.0, alpha);
	
			glTexCoord2f(texLeft, texBottom);					// bottom left of the texture and quad
			glVertex3f(	particles[i].position.data[0] - size, 
						particles[i].position.data[1] - size,  
						particles[i].position.data[2]);

			glTexCoord2f(texRight, texBottom);					// bottom right of the texture and quad
			glVertex3f(	particles[i].position.data[0] + size, 
						particles[i].position.data[1] - size,  
						particles[i].position.data[2]);

			glTexCoord2f(texRight, texTop);						// top right of the texture and quad
			glVertex3f(	particles[i].position.data[0] + size,
						particles[i].position.data[1] + size,
						particles[i].position.data[2]);

			glTexCoord2f(texLeft, texTop);						// top left of the texture and quad
			glVertex3f(	particles[i].position.data[0] - size,  
						particles[i].position.data[1] + size,  
						particles[i].position.data[2]);
			
			gPolygonCount++;									// ensure onscreen polygon count is accurate
		}
	}
	
	glEnd();
	
	
	if (theTexture && !gWireFrameOnly) glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	
}


