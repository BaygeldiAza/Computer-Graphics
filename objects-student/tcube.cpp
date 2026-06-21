// =========================================================================================
// Computer Graphics  - 2019

// 3D Engine Code
// =========================================================================================
//

//
// tcube.cpp
//
// This file defines the class for textured cubes



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "tcube.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern unsigned long	gPolygonCount;			// counts how many polygons get displayed each frame
extern bool             gWireFrameOnly;         // solid or wireframe drawing mode
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
tcube::tcube()
{
	// Initialise the object's state
	setColour(1, 1, 1, 1.0);					// default colour

	vertexCount = 8;
	faceCount = 6;
	polygonCount = 6;

	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);


	// Initialise the tcube's vertices to create a tcube centered around the origin
	vertices[0].set(-1,  1,  1);				// front, top, left
	vertices[1].set( 1,  1,  1);				// front, top, right
	vertices[2].set( 1, -1,  1);				// front, bot, right
	vertices[3].set(-1, -1,  1);				// front, bot, left
	vertices[4].set(-1,  1, -1);				// back,  top, left
	vertices[5].set( 1,  1, -1);				// back,  top, right
	vertices[6].set( 1, -1, -1);				// back,  bot, right
	vertices[7].set(-1, -1, -1);				// back,  bot, left


	// now set up the faces - note that the vertex order is always
	// specified counter-clockwise when that face is viewed front on
	faces[0].init(0, 3, 2, 1);					// front
	faces[1].init(1, 2, 6, 5);					// right
	faces[2].init(5, 6, 7, 4);					// back
	faces[3].init(4, 7, 3, 0);					// left
	faces[4].init(4, 0, 1, 5);					// top
	faces[5].init(3, 7, 6, 2);					// bottom
	
	
	// calculate the face and vertex normals
	calculateNormals();
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




// -----------------------------------------------------------------------------------------
// privateDraw method
// -----------------------------------------------------------------------------------------
void tcube::privateDraw(bool drawMode)
{

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


	glBegin(GL_QUADS);

	// front face 
	glNormal3fv(faces[0].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[0].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0,0.0);
	glVertex3fv(vertices[faces[0].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0,0.0);
	glVertex3fv(vertices[faces[0].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0 , 1);
	glVertex3fv(vertices[faces[0].vertex(3)].coordinate.data);		// top right
	
	// right face
	glNormal3fv(faces[1].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[1].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[faces[1].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0, 0);
	glVertex3fv(vertices[faces[1].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0 , 1);
	glVertex3fv(vertices[faces[1].vertex(3)].coordinate.data);		// top right
	
	// back face
	glNormal3fv(faces[2].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[2].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[faces[2].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(vertices[faces[2].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0, 1);
	glVertex3fv(vertices[faces[2].vertex(3)].coordinate.data);		// top right
    
	// left face
	// right face
	glNormal3fv(faces[3].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[3].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[faces[3].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0 , 0);
	glVertex3fv(vertices[faces[3].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0 , 1);
	glVertex3fv(vertices[faces[3].vertex(3)].coordinate.data);		// top right
	// top face
	glNormal3fv(faces[4].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[4].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[faces[4].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0, 0);
	glVertex3fv(vertices[faces[4].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0 , 1);
	glVertex3fv(vertices[faces[4].vertex(3)].coordinate.data);		// top right
	// bottom face
	glNormal3fv(faces[5].faceNormal.data);
	glTexCoord2f(0.0, 1);
	glVertex3fv(vertices[faces[5].vertex(0)].coordinate.data);		// top left
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(vertices[faces[5].vertex(1)].coordinate.data);		// bottom left
	glTexCoord2f(1.0, 0);
	glVertex3fv(vertices[faces[5].vertex(2)].coordinate.data);		// bottom right
	glTexCoord2f(1.0, 1);
	glVertex3fv(vertices[faces[5].vertex(3)].coordinate.data);		// top right
	glEnd();
    
	// disable texturing
	if (theTexture && !gWireFrameOnly) glDisable(GL_TEXTURE_2D);

}


