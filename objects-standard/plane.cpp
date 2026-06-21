// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// plane.cpp
//
// This file defines a class for textured planes (textured on both sides)
//
// This class creates a display list, and records the texture coordinates and 
// plane vertex data into this.  The privateDraw() method binds to the texture
// (if one is defined), and then plays the list back.  If no texture was bound,
// the plane draws as a white square.
//
// Note that the texture on the "back" of the object is reversed - you might want to 
// modify this so that it looks the same from both sides.



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "plane.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructors
// -----------------------------------------------------------------------------------------
plane::plane() : object3d()
{
	setName("plane");

	polygonCount = 2;
	
	// create a list and start recording into it
	listName = glGenLists(1);
	if (listName == 0) fatal("plane constructor couldn't create an OpenGL list");
	glNewList(listName, GL_COMPILE);

	// note that we use 0.999 as the upper limit for the s and t axes
	// as some OpenGL implementations will actually display one row of
	// pixels from the bottom of the texture (ie., wrap around), if we use 1.0
	
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);							// front-facing normal

	glTexCoord2f(0.0, 0.0);							// bottom left of the texture and quad
	glVertex3f(-1.0, -1.0,  0.0);

	glTexCoord2f(0.999, 0.0);						// bottom right of the texture and quad
	glVertex3f( 1.0, -1.0,  0.0);

	glTexCoord2f(0.999, 0.999);						// top right of the texture and quad
	glVertex3f( 1.0,  1.0,  0.0);

	glTexCoord2f(0.0, 0.999);						// top left of the texture and quad
	glVertex3f(-1.0,  1.0,  0.0);

	glNormal3f(0, 0, -1);							// back-facing normal

	glTexCoord2f(0.999, 0.0);						// top left of the texture and quad
	glVertex3f( 1.0, -1.0,  0.0);

	glTexCoord2f(0.0, 0.0);							// top right of the texture and quad
	glVertex3f(-1.0, -1.0,  0.0);

	glTexCoord2f(0.0, 0.999);						// bottom right of the texture and quad
	glVertex3f(-1.0,  1.0,  0.0);

	glTexCoord2f(0.999, 0.999);						// bottom left of the texture and quad
	glVertex3f( 1.0,  1.0,  0.0);

	glEnd();

	// stop recording the list
	glEndList();

	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}





