// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// tsphere.h
//
// This object is a textured sphere.
//
// The constructor takes one optional parameter - the resolution of the sphere
//
// NOTE - because of the high levels of distortion at the top and bottom of a
// sphere with respect to the texture map (the top and bottom rows of pixels in the
// texture map must be squeezed into a single pixel at each end of the sphere), the
// top and bottom of the sphere may not look right (this becomes more apparent
// as the level of detail at the top and bottom of the texture increases).  The
// workaround is to keep the sphere resolution relatively high, and don't
// zoom in too close to the top or bottom vertices.
//
// NOTE - textured spheres don't use the "Y" texture scale parameter - the X scale is used
// for both axes.


#ifndef _tsphere_H
#define _tsphere_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class tsphere : public object3d
{
	private:
				int         density;
				void 		builder(int r);

	public:
							tsphere();
							tsphere(int density);

};



#endif // _tsphere_H


