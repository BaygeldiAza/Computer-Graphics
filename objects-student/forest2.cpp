// =========================================================================================
// KXC354 - Computer Graphics & Animation - 2013
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// forest2.cpp
//
// This file declares the class for a forest2 object.  A forest2 consists of a collection
// of trees, which are implemented using textured billboards.
//
// This class demonstrates a composite object, and hence overrides hide() and show()
// so that all trees can be sent a hide or show message when the forest2 receives
// a hide or show message



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "forest2.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
// instantiates the compoent parts of the forest2, sets their colour and relative locations,
// and then establishes appropriate parentage
// Note that we do not need to push the forest2 object onto the shapeVector, because all each
// tree is a billboard object, and billboards get added to the vector as they are instantiated
// -----------------------------------------------------------------------------------------
forest2::forest2(int numTrees)
{
	treeCount = numTrees;
	treeTexture = new texture ("tree.tga", kContinuousTone, kMipmaps);
	trees.resize(treeCount);
	
	for (int i=0; i<treeCount; i++)
	{
		trees[i] = new billboard(treeTexture, kCylindricalBillboarding);
		float x = randomFloat(-30,-10);
		float z = randomFloat(-50, 50);
		float height = randomFloat(1.0, 4);
		float width = height * randomFloat(1.0, 2.0);
		
		// notice here that we use the width value for both the X and Z scaling - if you don't
		// do this (ie., use 1.0 for the Z scaling) the shape will distort as you move arount it
		trees[i]->setDeformation(width, height, width);
		trees[i]->setPosition(x, height, z);
	}
	
}




// -----------------------------------------------------------------------------------------
// hide & show
// -----------------------------------------------------------------------------------------
// these methods override those of object3d, and cascade the hide and show message to
// all of the trees
// -----------------------------------------------------------------------------------------
void forest2::hide()
{
	for (int i=0; i<treeCount; i++)
		trees[i]->hide();
}

void forest2::show()
{
	for (int i=0; i<treeCount; i++)
		trees[i]->show();
}





