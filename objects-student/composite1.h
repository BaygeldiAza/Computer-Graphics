#pragma once
#ifndef _composite1_H
#define _composite1_H

#include "object3d.h"
#include "extrusion.h"
#include "sweep.h"
#include "cube.h"
#include "sphere.h"
#include "vectr.h"

class composite1 : public object3d
{
public:
    composite1();
    virtual void    hide(void);
    virtual void    show(void);

    void            animateFlight(float time);
    void            startFlight() { isFlying = true; }
    void            stopFlight() { isFlying = false; }

    // Fly smoothly toward a world-space target; returns true when arrived
    bool            flyTo(float tx, float ty, float tz, float deltaTime, float speed = 8.0f);
    // Convenience overload accepting a vectr
    bool            flyTo(vectr target, float deltaTime, float speed = 8.0f);

    cube* getFuselageRef() { return nullptr; }
    sweep* getMainRotorHub() { return mainRotorHub; }
    sweep* getTailRotorHub() { return tailRotorHub; }

private:
    extrusion* fuselage, * fuselage2, * Fwin, * Fwin2;
    extrusion* tailBoom;
    cube* tailFin;
    sweep* tailRotorHub;
    cube* tailRotorBlade1;
    cube* tailRotorBlade2;
    cube* mainRotorMast;
    sweep* mainRotorHub;
    cube* blade1, * blade2, * blade3, * blade4;
    cube* skidStrut1, * skidStrut2, * skidStrut3, * skidStrut4;
    extrusion* skidLeft, * skidRight;
    cube* redCross, * redCross2;
    cube* medStripe1, * medStripe2;
    sphere* searchLight;

    bool        isFlying;
    float       rotorAngle;
    float       tailRotorAngle;
    float       hoverPhase;
};

#endif // _composite1_H