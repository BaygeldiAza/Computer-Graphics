// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// disc.h
//
// This file declares a class for textured discs, which are
// derived from generic object3d objects


#ifndef _disc_H
#define _disc_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class disc : public object3d
{
	private:
				int     divisions;                      // number of pie slices
				int     rings;                          // number of concentric rings
				void	builder(int d, int r);          // function to actually create geometry
				
				
	public:
						disc();							// default constructor
						disc(int d, int r);             // constructor with params

};


#endif // _disc_H

