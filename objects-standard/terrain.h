// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// terrain.h
//
// This file declares the class for terrain objects.  A terrain is a "bumpy" ground
// surface.  The bump map is defined by a BMP file.

#ifndef _terrain_H
#define _terrain_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class terrain : public object3d
{
	private:
				int			width;
				int			depth;
			
	public:
							terrain(string theFilename);

		virtual void		privateDraw(bool drawMode);		

				float		rawelevation(float x, float z);
};


#endif // _terrain_H

