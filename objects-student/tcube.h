// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// tcube.h
//
// This file declares the class for tcube objects, based on the
// generic 3D object


#ifndef _tcube_H
#define _tcube_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------

class tcube : public object3d
{
	public:
						tcube();
						virtual void privateDraw(bool drawMode);

};


#endif // _tcube_H

