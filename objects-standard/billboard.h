// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// billboard.h
//
// This file declares a class for a billboarded quad


#ifndef _billboard_H
#define _billboard_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"
#include "texture.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class billboard : public object3d
{
	private:
					bool		cylindrical;
					bool		useAlpha;
					
	public:
								billboard(texture *t, bool billboardMode);
			virtual void		privateDraw(bool);		

};


#endif // _billboard_H

