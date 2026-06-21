// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// tsphere.cpp
//
// This file defines the class for tsphere objects.  A tsphere is a sphere
//  that can have textures mapped onto it's surface.
//
// The constructor takes one optional parameter:
// density - the mesh density for mesh density
//
// NOTE - because of the high levels of distortion at the top and bottom of a
// sphere with respect to the texture map (the top and bottom rows of pixels in the
// texture map must be squeezed into a single pixel at each end of the sphere), the
// top and bottom of the sphere may not look right (this becomes more apparent
// as the level of detail at the top and bottom of the texture increases).  One
// workaround is to keep the sphere density relatively high, and don't
// zoom in too close to the top or bottom vertices.



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "tsphere.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;





// -----------------------------------------------------------------------------------------
// constructors
// -----------------------------------------------------------------------------------------

tsphere::tsphere()
{
	builder(40);
}

tsphere::tsphere(int r)
{
	builder(r);
}



// -----------------------------------------------------------------------------------------
// the "real" constructor
// not exported,  but called by the real constructor
// -----------------------------------------------------------------------------------------

void tsphere::builder(int r)
{

	setName("tsphere");

	// save the density and polygonCount for later use (note that the faceCount remains
	// at zero, as this shape has no stored geometry)
	density = r;
	polygonCount = (density * (density - 2)) + (density * 2);


	// create a list and start recording into it
	listName = glGenLists(1);
	if (listName == 0) fatal("tsphere constructor couldn't create an OpenGL list");
	glNewList(listName, GL_COMPILE);
	

	// calculate how width (and height) of each quad on the texture;
	float textureQuadSize = 1.0 / density;


	// generate the vertex information to build a 1-unit sphere centred on the origin
	float zStepSize = 180.0 / density;			// rotate around Z axis by this amount
	float zRot = 90.0;							// starting Z rot at the north pole
	
	// outer loop - rotation about the Z axis
	// we start at the north pole and sweep down to the last disc above the south pole
	// (we don't need to go to the south pole as part of the loop, as we're sending two discs 
	// worth of vertices each time through the loop - the upper and lower sides of each quad)
	for (int i=0; i < density; i++)
	{
		glBegin(GL_QUAD_STRIP);								// for every row, start a new triangle strip
		
		// inner loop - rotation about the Y axis
		// note that we actually go around one more step than you might expect, to close the strip
		for (int j=0; j <= density; j++)
		{
			// calculate angle around Y axis
			float yRot = j * (360.0 / density);
			
			// calculate x, y & z coordinates for top and bottom of quad
			float upperX = cos_d(zRot) * cos_d(yRot);
			float upperZ = cos_d(zRot) * sin_d(yRot);
			float upperY = sin_d(zRot);
			
			float lowerX = cos_d(zRot - zStepSize) * cos_d(yRot);
			float lowerZ = cos_d(zRot - zStepSize) * sin_d(yRot);
			float lowerY = sin_d(zRot - zStepSize);

			// the correct order for a quad strip is to send the lower vertices first (in this case)
			// prior to sending each vertex, we send the normal at this vertex, and the texture coordinate
			// (for the normal, we just send the vertex data, since they're the same thing on the sphere surface)
			glTexCoord2f(1.0 - (textureQuadSize * j), 1.0 - (textureQuadSize * (i+1)));
			glNormal3f(lowerX, lowerY, lowerZ);
			glVertex3f(lowerX, lowerY, lowerZ);
			glTexCoord2f(1.0 - (textureQuadSize * j), 1.0 - (textureQuadSize * i));
			glNormal3f(upperX, upperY, upperZ);
			glVertex3f(upperX, upperY, upperZ);

		}
		glEnd();
		zRot = zRot - zStepSize;					// step down to the next row of quads
	}

	// stop recording the list
	glEndList();
	
	diffuseShading = false;

	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



