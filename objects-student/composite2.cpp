// *****************************************************************************************
// KXC354 - Computer Graphics & Animation
// Assignment 2
// *****************************************************************************************
//
// composite2.cpp
//
// Lego person composite object.
//
// *****************************************************************************************
#include "prefix.h"
#include "composite2.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;


// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

composite2::composite2() :bobPhase(0), isWalking(true), walkSpeed(1.0f),
fallInProgress(false), fallDone(false), fallAngle(0.0f), fallY(0.0f)
{
    setName("LegoPerson");
    // =================================================================================
    // TORSO  
    // =================================================================================
    torso = new extrusion("lego_torso_profile.txt", "lego_torso_path.txt");
    torso->setColour(0.53, 0.68, 0.81);
    torso->setSpecularColour(0.4f, 0.1f, 0.1f, 1.0f);
    torso->setRotation('x', -90.0f);                      
    torso->setDeformation(1, 0.7, 1);
    torso->setPosition(0, 0.5, 0);            
    torso->useSmoothShading();
    torso->setParent(this);

    // =================================================================================
    // HEAD 
    // =================================================================================
    head = new ttcube;
    tex = new texture("headd.jpg");
    head->setTexture(tex);
    head->setSpecularColour(0.6f, 0.5f, 0.0f, 1.0f);
    head->setShininess(50.0f);
    head->setRotation('x', 90.0f);                      
    head->setDeformation(0.3, 0.3, 0.3);
    head->setPosition(0.0f, -0.3, 1.3f);                
    head->setParent(torso);





    //=================================================================================
    //HIPS  (cube)
    //Dark grey block connecting the torso to the legs.
    //=================================================================================
    hips = new cube;
    hips->setColour(0.25, 0.30, 0.44);
    hips->setSpecularColour(0.2f, 0.2f, 0.2f, 1.0f);
    hips->setShininess(20.0f);
    hips->setDeformation(0.6, 0.25f, 0.3);
    hips->setPosition(0, -0.3, -0.3);
    hips->setParent(torso);
    //=================================================================================
    //neck  (cube)
    //=================================================================================
    neck = new cube;
    neck->setColour(0.53, 0.68, 0.81);
    neck->setSpecularColour(0.2f, 0.2f, 0.2f, 1.0f);
    neck->setShininess(20.0f);
    neck->setDeformation(0.5, 0.24, 0.2);
    neck->setPosition(0, -0.25, 0.8);
    neck->setParent(torso);

    // =================================================================================
    // ARMS  
    //=================================================================================
    leftArm = new sweep("pole.txt", 100);
    leftArm->setColour(0.53, 0.68, 0.81);     
    leftArm->setDeformation(0.13f, 0.65f, 0.13f);
    leftArm->setRotation('z', -10.0f, 'x', 90.0f);                    
    leftArm->setPosition(-0.72f, -0.3, 0.3);         
    leftArm->setParent(torso);
    leftArm->useSmoothShading();

    rightArm = new sweep("pole.txt", 100);
    rightArm->setColour(0.53, 0.68, 0.81);
    rightArm->setDeformation(0.13f, 0.65f, 0.13f);
    rightArm->setRotation('z', 10.0f, 'x', 90);                  
    rightArm->setPosition(0.72f, -0.3, 0.3);          
    rightArm->setParent(torso);
    rightArm->useSmoothShading();
    // =================================================================================
    // LEGS  
    // =================================================================================
    leftLeg = new sweep("pole.txt", 100);
    leftLeg->setColour(0.25, 0.30, 0.44);
    leftLeg->setSpecularColour(0.1f, 0.2f, 0.4f, 1.0f);
    leftLeg->setShininess(30.0f);
    leftLeg->setRotation('x', 90.0f);                    
    leftLeg->setDeformation(0.15, 0.8f, 0.15);
    leftLeg->setPosition(-0.4f, -0.3, -1.1);         
    leftLeg->setParent(torso);
    leftLeg->useSmoothShading();

    rightLeg = new sweep("pole.txt", 100);
    rightLeg->setColour(0.25, 0.30, 0.44);
    rightLeg->setSpecularColour(0.1f, 0.2f, 0.4f, 1.0f);
    rightLeg->setShininess(30.0f);
    rightLeg->setRotation('x', 90.0f);
    rightLeg->setDeformation(0.15, 0.8f, 0.15);
    rightLeg->setPosition(0.4f, -0.3, -1.1);         
    rightLeg->setParent(torso);
    rightLeg->useSmoothShading();

    // register with the global shape vector so it gets drawn
    gShapeVector.push_back(this);
}
void composite2::animateWalk(float deltaTime)
{

    if (!isWalking) return;

    bobPhase += deltaTime * walkSpeed * 3.0f;

    // Forward movement
    float forwardSpeed = walkSpeed * 5.0f;
    float x, y, z;
    getPosition(x, y, z);
    setPosition(x - forwardSpeed * deltaTime, y, z);
    

    float legPhase = bobPhase * 2.0f;

    // Left leg movement - more natural walking motion
    leftLeg->setRotation('x', 20 * sin(legPhase) - 90);


    // Right leg movement - opposite phase for natural walking
    rightLeg->setRotation('x', 20 * sin(legPhase + 3.14159f) - 90);


    // Arm swinging - more pronounced
    leftArm->setRotation('x', -20 * sin(legPhase + 3.14159f) - 90);
    rightArm->setRotation('x', -20 * sin(legPhase) - 90);



}

// -----------------------------------------------------------------------------------------
// fall
//
// Call every frame after the crash. Rotates the person -90 degrees around Z
// (tips over sideways) and lowers Y so they rest on the ground.
// -----------------------------------------------------------------------------------------

void composite2::fall(float deltaTime)
{
    if (fallDone) return;

    fallInProgress = true;
    isWalking = false;

    // Stop leg/arm animation — freeze limbs in neutral pose
    leftLeg->setRotation('x', -90.0f);
    rightLeg->setRotation('x', -90.0f);
    leftArm->setRotation('x', -90.0f, 'z', -10.0f);
    rightArm->setRotation('x', -90.0f, 'z', 10.0f);

    // Rotate body: tip toward -Z (fall forward), target = -90 degrees
    float fallSpeed = 120.0f;   // degrees per second
    fallAngle -= fallSpeed * deltaTime;
    if (fallAngle < -90.0f)
    {
        fallAngle = -90.0f;
        fallDone = true;
        fallInProgress = false;
    }

    // Also sink Y slightly so the person lies flat on the ground
 // When fallen at -90 deg around Z, the half-height of the figure (~1.5) becomes X offset
    float targetY = -0.2f;
    float x, y, z;
    getPosition(x, y, z);
    float currentY = y;
    if (currentY > targetY)
    {
        y -= 2 * deltaTime;
        if (y < targetY) y = targetY;
        setPosition(x+0.03, y, z+0.1);
    }

    setRotation('x', fallAngle);
}

// -----------------------------------------------------------------------------------------
// hide
// -----------------------------------------------------------------------------------------

void composite2::hide(void)
{
    head->hide();
    torso->hide();
    hips->hide();
    neck->hide();
    leftArm->hide();
    rightArm->hide();
    leftLeg->hide();
    rightLeg->hide();
    object3d::hide();
}


// -----------------------------------------------------------------------------------------
// show
// -----------------------------------------------------------------------------------------

void composite2::show(void)
{
    head->show();
    torso->show();
    hips->show();
    neck->show();
    leftArm->show();
    rightArm->show();
    leftLeg->show();
    rightLeg->show();
    object3d::show();
}

