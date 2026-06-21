#pragma once
#ifndef _CAR_H
#define _CAR_H

#include "object3d.h"
#include "extrusion.h"
#include "sweep.h"
#include "cube.h"

class car : public object3d
{
public:
    car();
    virtual void hide(void);
    virtual void show(void);

    void animateDrive(float time);

    // Moves car toward (tx,ty,tz) at crashSpeed; returns true when arrived
    bool crashToward(float tx, float ty, float tz, float deltaTime, float speed = 30.0f);

private:
    extrusion* bodyL;
    extrusion* bodyR;

    sweep* wheelFL, * wheelFR, * wheelRL, * wheelRR;
    sweep* sideL, * sideR;

    cube* light1, * light2;
    cube* windscreen;

    float       driveSpeed;
    float       currentDistance;
    float       wheelAngle;     // persistent wheel rotation for crashToward
};

#endif // _CAR_H