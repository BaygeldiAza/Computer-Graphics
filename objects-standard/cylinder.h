// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// cylinder.h
//
// This file declares a class for textured cylinders, which are
// derived from generic object3d objects


#ifndef _cylinder_H
#define _cylinder_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class cylinder : public object3d
{
	private:
				int     density;							// saved mesh density
				void 	builder(int r);
				
				
	public:
						cylinder();							// default constructor
						cylinder(int density);				// constructor with density

};



#endif // _cylinder_H

