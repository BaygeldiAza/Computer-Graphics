// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// terrain.cpp
//
// This file declares the class for terrain objects.  A terrain is a "bumpy" ground
// surface.  The bump map is defined by a BMP file.



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "terrain.h"
#include "utility.h"
#include "platform.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern	vector<object3d*> gShapeVector;
extern	unsigned long	gPolygonCount;			// counts how many polygons get displayed each frame
extern	bool			gShowFaceNormals;		// if true, draw face normals




// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------

terrain::terrain(string theFilename)
{

	string			fileLocation;
	char			red, green, blue;

	ifstream 		theFile;
	bmpFileHeader	fileHeader;
	bmpInfoHeader	infoHeader;


	// get the full path to the terrain file
	fileLocation = pathToResourceDirectory() + theFilename;
	setName("terrain_" + theFilename);

	// read the BMP file header information
	theFile.open(fileLocation.c_str(), ios::in | ios::binary);
	if (!theFile)
	{
		string errMesg = "Unable to open terrain file ";
		errMesg.append(theFilename);
		fatal (errMesg);
	}
	theFile.read((char *)&fileHeader, bmpFileHeaderSize);
	theFile.read((char *)&infoHeader, bmpInfoHeaderSize);


#ifdef __BIG_ENDIAN__
	// swan "endian-ness" on MacOS X PPC
	endianSwapShort((unsigned char *)&fileHeader.type);
	endianSwapLong((unsigned char *)&infoHeader.size);
	endianSwapLong((unsigned char *)&infoHeader.width);
	endianSwapLong((unsigned char *)&infoHeader.height);
	endianSwapShort((unsigned char *)&infoHeader.planes);
	endianSwapShort((unsigned char *)&infoHeader.bitcount);
	endianSwapLong((unsigned char *)&infoHeader.compression);
	endianSwapLong((unsigned char *)&infoHeader.imageSize);
	endianSwapLong((unsigned char *)&infoHeader.horizPixelsPerMeter);
	endianSwapLong((unsigned char *)&infoHeader.vertPixelsPerMeter);
	endianSwapLong((unsigned char *)&infoHeader.colourTable);
	endianSwapLong((unsigned char *)&infoHeader.importantColours);
#endif


	// save the size of the terrain in our own data members
	width = infoHeader.width;
	depth = infoHeader.height;


	// do some basic checking on the BMP file
	if (fileHeader.type != 0x4D42) fatal ("tried to create a terrain from a non-BMP file");
//	if ((((long)width * (long)depth) *2) > 65535L)
//		fatal ("BMP too large to create a terrain - it would have too many vertices and/or faces");
	if (infoHeader.colourTable != 0)
		fatal ("trying to create a terrain from an 8-bit BMP file - please convert it to 24-bit");
	if (infoHeader.compression != 0)
		fatal ("trying to create a terrain from compressed BMP file - it must be uncompressed");
	
	
	
	// allocate space for the vertex and face lists
	vertexCount = width * depth;
	faceCount = (width-1) * (depth-1) * 2;
	polygonCount = faceCount;
	vertices.resize(vertexCount);
	faces.resize(faceCount);


	// now read image data from the file, 3 bytes at a time (RGB), and generate the terrain vertices
	unsigned int v = 0;
	for (int z = 0; z < depth; z++)
	{
		for (int x = 0; x < width; x++)
		{
			theFile.get(blue);
			theFile.get(green);
			theFile.get(red);

			// weight the colours to get the luminance as a single value between -1.0 and +1.0
			float bright = ((0.3*(unsigned char)red + 0.59*(unsigned char)green + 0.11*(unsigned char)blue)/127.5) - 1.0;
			vertices[v++].set(x-(width/2), bright, -z+(depth/2));
		}
	}
	
	if (theFile.eof()) fatal("somehow managed to read past the end of the terrain BMP file!");
	theFile.close();


	// now link the faces
	unsigned int f=0;
	for (int z = 0; z < depth-1; z++)
		for (int x = 0; x < width-1; x++)
		{
			int offset1 = x + (z * width);
			int offset2 = x + ((z+1) * width);
			faces[f++].init(offset1, offset2+1, offset2);
			faces[f++].init(offset1, offset1+1, offset2+1);
		}


	// calculate the face and vertex normals
	calculateNormals();
	diffuseShading = false;
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}






// -----------------------------------------------------------------------------------------
// custom draw method
// -----------------------------------------------------------------------------------------
void terrain::privateDraw(bool drawMode)
{

	// draw the terrain
	for (int z = 0; z < depth-1; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		
		for (int x = 0; x < width; x++)
		{
			// send the normal, vertex, and texture coordinate data
			int offset1 = x + (z * width);
			int offset2 = x + ((z+1) * width);
			glNormal3fv(vertices[offset2].normal.data);
			glTexCoord2f((1.0/(width-1)) * x, (1.0/(depth-1)) * (z + 1));
			glVertex3fv(vertices[offset2].coordinate.data);
			glNormal3fv(vertices[offset1].normal.data);
			glTexCoord2f((1.0/(width-1)) * x, (1.0/(depth-1)) * z);
			glVertex3fv(vertices[offset1].coordinate.data);
		}
		glEnd();
	}
	
	
	// finally, draw the face normals if this feature is turned on (this isn't absolutely
	// required, but it is useful for debugging, and it is normally handled by the 
	// object3d::privateDraw(), which we have overridden)
	if (gShowFaceNormals)
	{ 
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor4f(0.0, 0.0, 1.0, 1.0);
		for (unsigned int i=0; i<faceCount; i++)
		{
			vectr temp = faces[i].centroid;
			for (int j=0; j<3; j++) 
				temp.data[j] = temp.data[j] + (faces[i].faceNormal.data[j]/5.0);
			glVertex3fv(faces[i].centroid.data);
			glVertex3fv(temp.data);
		}
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnd();
		glEnable(GL_LIGHTING);
	}


}





// -----------------------------------------------------------------------------------------
// return the elevation of a point inside the terrain
// -----------------------------------------------------------------------------------------

float terrain::rawelevation(float x, float z)
{
	float theHeight = -100.0;

	// check that the input maps onto the terrain
	if	(
		(x < (-width/2)) || 
		(x > (width/2)-1) ||
		(z < (-depth/2)+1) ||
		(z > (depth/2))
		)
		return theHeight;


	// convert the input coordinates to array indices (i.e., the indices of the
	// vertices that surround the point
	int leftX = (int) floorf(x);
	int rightX = (int) ceilf(x);
	int farZ = (int) floorf(z);
	int nearZ = (int) ceilf(z);
	
	
	// convert these coordinates to "vertex array indices"
	int leftXindex = leftX + width/2;
	int rightXindex = rightX + width/2;
	int nearZindex = depth - (nearZ + depth/2);
	int farZindex = depth - (farZ + depth/2);
	
	
	// get the depth of each of the surrounding vertices from the vertex array
	// here, A, B, C and D refer to the same relative points as on the lecture notes
	float yA = vertices[(width * farZindex) + leftXindex].coordinate.data[1];
	float yB = vertices[(width * nearZindex) + leftXindex].coordinate.data[1];
	float yC = vertices[(width * farZindex) + rightXindex].coordinate.data[1];
	float yD = vertices[(width * nearZindex) + rightXindex].coordinate.data[1];
	

	// are we right on a point?
	if ((leftX == rightX) && (farZ == nearZ))
		return yA;
	
	
	// assuming point "A" is the origin, and work out how far the point is from it
	// so we can determine which triangle holds the point
	float dx = x - leftX;
	float dz = z - farZ;
	
	if ((dx + dz) <= 1)
		theHeight = yA + (dx * (yC - yA)) + (dz * (yB - yA));
	else
	{
		// recalculate dx and dz, using D as the origin
		dx = rightX - x;
		dz = nearZ - z;
		theHeight = yD + (dx * (yB - yD)) + (dz * (yC - yD));
	}
	
	return theHeight;
}






