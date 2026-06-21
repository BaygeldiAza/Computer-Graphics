// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// billboard.cpp
//
// This file defines a class for a billboarded quad



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "billboard.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern bool				gShowLocalAxes;			// if true, draw local axes (2x2x2) as object is drawn
extern bool				gWireFrameOnly;			// if true, only draw the outline of the billboard
extern vector<object3d*> gShapeVector;





// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
billboard::billboard(texture *t, bool billboardMode)
{
	setName("billboard");
	polygonCount = 1;
	autoBindToTexture = false;					// let us control texture binding and use
	setTexture(t);
	setColour(1.0, 1.0, 1.0, 1.0);
	setSpecularColour(0.0, 0.0, 0.0, 1.0);
	objectOpacity = kTransparentMode;
	cylindrical = billboardMode;
	useAlpha = true;
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




// -----------------------------------------------------------------------------------------
// privateDraw method
// -----------------------------------------------------------------------------------------
void billboard::privateDraw(bool drawMode)
{
	// disable lighting, or objects will get light/dark as
	// they rotate away from or towards the light source
	glDisable(GL_LIGHTING);


	// begin billboarding
	if (cylindrical)
		beginCylindricalBillboard(position);
	else
		beginSphericalBillboard(position);
	

	// bind to the texture that defines our billboard
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
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	// draw the billboard as a quad
	
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);					// bottom left of the texture and quad
	glVertex3f(-1, -1, 0);

	glTexCoord2f(1, 0);					// bottom right of the texture and quad
	glVertex3f(	1, -1, 0);

	glTexCoord2f(1, 1);					// top right of the texture and quad
	glVertex3f(	1, 1, 0);

	glTexCoord2f(0, 1);					// top left of the texture and quad
	glVertex3f(-1, 1, 0);
	
	glEnd();
	
	
	if (theTexture && !gWireFrameOnly) glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}
