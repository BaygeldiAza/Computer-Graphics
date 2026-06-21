// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// disc.cpp
//
// This file defines a class for textured discs. 
//
// The disc is oriented in the X-Z plane, and only the "up faces" are
// rendered.  The single normal that is sent is an "up" normal.
//
// This class creates a display list, and records the texture coordinates and 
// disc vertex data into this.  The privateDraw() method binds to the texture
// (if one is defined), and then plays the list back.  If no texture was bound,
// the plane draws as a white square.
//
// Two (optional) parameters may be used:
// divisions	- the number of "pie slices" for the disc
// rings		- the number of concentric rings for the disc


// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "disc.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructors
// -----------------------------------------------------------------------------------------
disc::disc() : object3d()
{
	setName("disc");
	builder(16, 1);								// construct a default disc if no parameters supplied
}

disc::disc(int d, int r) : object3d()
{
	builder(d, r);
}



// -----------------------------------------------------------------------------------------
// the "real" disc constructor
// a private function called by the actual constructors
// -----------------------------------------------------------------------------------------

void disc::builder(int d, int r)
{
	if (r < 1) r = 1;							// ensure at least one ring
	divisions = d;								// save these parameters for later
	rings = r;
	polygonCount = divisions * rings;


	// number of concentric rings
	float ringSize = 1.0/rings;
	

	// create a list and start recording into it
	listName = glGenLists(1);
	if (listName == 0) fatal("disc constructor couldn't create an OpenGL list");
	glNewList(listName, GL_COMPILE);



	// do the centre fan first
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 1, 0);					// we only need to send a single normal ("up")
	glTexCoord2f(0.5, 0.5);					// center vertex is in the centre of the texture
	glVertex3f(0.0, 0.0, 0.0);				// center vertex itself (the origin)

	for (int i=0; i <= divisions; i++)
	{
		float angle=i * (360.0/divisions);		// calculate the vertices surrounding the origin
		float x = ringSize * cos_d(angle);
		float z = ringSize * sin_d(angle);
		glTexCoord2f((-x+1)/2.0, (-z+1)/2.0);
		glVertex3f(-x,  0.0, z);
	}
	glEnd();
	

	// now do the outer concentric rings (if there are any)
	for (int ring=1; ring<rings; ring++)
	{	
		glBegin(GL_QUAD_STRIP);
		for (int i=0; i <= divisions; i++)
		{
			float angle=i * (360.0/divisions);
			float innerX = (ringSize * ring) * cos_d(angle);
			float innerZ = (ringSize * ring) * sin_d(angle);
			float outerX = (ringSize * (ring+1)) * cos_d(angle);
			float outerZ = (ringSize * (ring+1)) * sin_d(angle);
			
			glTexCoord2f((-innerX+1)/2.0, (-innerZ+1)/2.0);
			glVertex3f(-innerX,  0.0, innerZ);
			glTexCoord2f((-outerX+1)/2.0, (-outerZ+1)/2.0);
			glVertex3f(-outerX,  0.0, outerZ);
		}
		glEnd();
	}

	// stop recording the list
	glEndList();

	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}





