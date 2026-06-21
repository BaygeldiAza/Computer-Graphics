#pragma once
#ifndef _composite2_H
#define _composite2_H

#include "object3d.h"
#include "extrusion.h"
#include "sweep.h"
#include "cube.h"
#include "ttcube.h"
#include "texture.h"

class composite2 : public object3d
{
public:
    composite2();
    virtual void hide(void);
    virtual void show(void);

    void animateWalk(float deltaTime);
    void fall(float deltaTime);         // tips over 90 degrees and sinks to ground
    void startWalking() { isWalking = true; }
    void stopWalking() { isWalking = false; }
    bool isFalling() { return fallInProgress; }
    bool isFallen() { return fallDone; }

    bool isWalking;

private:
    ttcube* head;
    sweep* headStud;
    cube* eyeL, * eyeR, * smile;
    extrusion* torso;
    sweep* leftArm, * rightArm;
    cube* hips, * neck;
    sweep* leftLeg, * rightLeg;
    texture* tex;

    float bobPhase;
    float walkSpeed;

    // fall state
    bool  fallInProgress;
    bool  fallDone;
    float fallAngle;        // current rotation around Z during fall (0 -> -90)
    float fallY;            // current Y offset while sinking
};

#endif // _composite2_H