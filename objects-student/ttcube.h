// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// ttcube.h
//
// This file declares the class for ttcube objects, based on the
// generic 3D object


#ifndef _ttcube_H
#define _ttcube_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------

class ttcube : public object3d
{
	public:
						ttcube();
						virtual void privateDraw(bool drawMode);

};


#endif // _ttcube_H

