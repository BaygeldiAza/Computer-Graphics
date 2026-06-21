// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// cylinder.cpp
//
// This file defines a class for (optionally textured) cylinders (if a texture has been
// assigned to the cylinder, it is drawn, otherwise a non-textured cylinder is drawn).
//
// This class was developed as a demonstration of texturing.  It currently does
// not draw the end-caps.
//
// This class doesn't create any stored-geometry - just a display list of the vertex
// data and texture coordinates.  Because of this, it can't show face normals if this
// feature is turned on ("n" key).
//
// One (optional) parameter may be used:
// density	- the mesh density for radial sweep



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "cylinder.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
cylinder::cylinder() : object3d()
{
	setName("cylinder");
	builder(16);								// if no density provided, default to 16
}


cylinder::cylinder(int r) : object3d()
{
	builder(r);
}




// -----------------------------------------------------------------------------------------
// the "real" constructor
// not exported,  but called by the real constructor
// -----------------------------------------------------------------------------------------

void cylinder::builder(int r)
{
	polygonCount = density = r;	

	// create a list and start recording into it
	listName = glGenLists(1);
	if (listName == 0) fatal("tsphere constructor couldn't create an OpenGL list");
	glNewList(listName, GL_COMPILE);
	

	// we draw the cylinder as a strip of quads
	glBegin(GL_QUAD_STRIP);
	
	for (int i=0; i <= density; i++)
	{
		float angle=i * (360.0/density);
		float x = -cos_d(angle);
		float z = sin_d(angle);
		
		// normal for lighting
		glNormal3f(x, 0, z);

		// texture and geometry top coordinate
		glTexCoord2f((angle/360.0), 1.0);
		glVertex3f(x,  1.0, z);

		// texture and geometry bottom coordinate
		glTexCoord2f((angle/360.0), 0.0);
		glVertex3f(x, -1.0, z);
	}
	glEnd();
	
	// stop recording the list
	glEndList();
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




