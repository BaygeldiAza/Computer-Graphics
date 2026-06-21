// =========================================================================================
// KXC354 - Computer Graphics & Animation - 2013
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// forest2.h
//
// This file declares the class for a forest2 object.  A forest2 consists of a collection
// of trees, which are implemented using textured billboards.
//
// This class demonstrates a composite object, and hence overrides hide() and show()
// so that all trees can be sent a hide or show message when the forest2 receives
// a hide or show message

 

#ifndef _forest2_H
#define _forest2_H




// -----------------------------------------------------------------------------------------
// system includes
// -----------------------------------------------------------------------------------------

// STL
#include <vector>
using namespace std;



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"
#include "billboard.h"
#include "texture.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class forest2 : public object3d
{
	public:
				int					treeCount;
				vector<billboard*>	trees;
				texture				*treeTexture;
				
									forest2(int treeCount);				// the constructor
									
		virtual void				hide(void);
		virtual void				show(void);
	
};


#endif // _forest2_H

