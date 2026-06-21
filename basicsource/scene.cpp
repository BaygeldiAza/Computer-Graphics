// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// scene.cpp
// *****************************************************************************************


// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------

#include "prefix.h"
#include "constants.h"
#include "utility.h"
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "scene.h"
#include "snd.h"
#include "skybox.h"
#include "fog.h"
#include "viewfrustum.h"

#include "sweep.h"
#include "extrusion.h"
#include "sphere.h"
#include "terrain.h"
#include "tsphere.h"
#include "disc.h"
#include "tcube.h"
#include "forest.h"
#include "forest2.h"
#include "composite2.h"
#include "car.h"
#include "composite1.h"

// -----------------------------------------------------------------------------------------
// global variables
// -----------------------------------------------------------------------------------------
extern ProgramMode      gProgramMode;
extern cameraClass      gCamera;
extern object3d* gSky;
extern fog* gFog;
extern viewfrustum* gViewFrustum;
extern float            gCurrentFrameTime;

extern vector<object3d*> gShapeVector;
typedef vector<object3d*>::iterator shapeVectorIterator;

// ---- scene objects ----
tcube* theGround, * street, * helipad, * roof;
texture* texGround, * treeTexture, * texS, * tex, * texHelpad, * texR;
forest* f;
forest2* f2;
tcube* hosp;
composite2* lego;
car* C1;
light* ambient;
light* light0;
light* light1;
composite1* heli;
snd* s, * s1;
// one-time trigger guards
static bool fallStarted = false;
static bool heliLaunched = false;


// -----------------------------------------------------------------------------------------
// constructScene
// -----------------------------------------------------------------------------------------

void constructScene()
{
    ambient = new light(GL_LIGHT_MODEL_AMBIENT);
    light0 = new light(GL_LIGHT0);
    light1 = new light(GL_LIGHT1);

    theGround = new tcube;
    texGround = new texture("terrain.jpg");
    theGround->setTexture(texGround, 8, 8);
    theGround->setPosition(0.0f, 0, 0.0f);
    theGround->setDeformation(150, 0.1f, 150);

    street = new tcube;
    texS = new texture("street.jpg");
    street->setTexture(texS, 1, 8);
    street->setPosition(0.0f, 0.1f, 0.0f);
    street->setDeformation(7, 0.1f, 150);

    hosp = new tcube;
    tex = new texture("hospital.jpg");
    hosp->setTexture(tex, 0.5f, 2);
    hosp->setPosition(18, 20, -70);
    hosp->setDeformation(10, 20, 10);

    helipad = new tcube;
    texHelpad = new texture("helipad.jpg");
    helipad->setTexture(texHelpad);
    helipad->setPosition(0.0f, 25, 0.0f);
    helipad->setDeformation(10.0f, 0.08f, 10.0f);
    helipad->setParent(hosp);

    heli = new composite1;
    heli->setPosition(18, 46.7f, -70);
    heli->setScale(1.2f);
    heli->setRotation('y', 180);

    roof = new tcube;
    texR = new texture("medic.png");
    roof->setTexture(texR);
    roof->setPosition(0.0f, 22.5f, 0.0f);
    roof->setDeformation(10.0f, 2.5f, 10.0f);
    roof->setParent(hosp);

    f = new forest(50);
    f2 = new forest2(50);

    lego = new composite2;
    lego->setPosition(25, 1.6f, 29);
    lego->setRotation('y', -90);

    C1 = new car;
    C1->setPosition(-8, 0, -80);

    gSky = new skybox("sky22-");

    s = new snd("Car_Crash.ogg");
    s1 = new snd("Helicopter.ogg");
}


// -----------------------------------------------------------------------------------------
// resetScene
// -----------------------------------------------------------------------------------------

void resetScene()
{
    gCamera.setPosition(0, 5, 60);

    ambient->setColour(0.35f, 0.35f, 0.35f, 1.0f);

    light0->turnOn();
    light0->setPosition(-8.0f, 20.0f, -5.0f);
    light0->setColour(0.85f, 0.85f, 0.80f, 1.0f);
    light0->setSpecularColour(1.0f, 1.0f, 0.95f, 1.0f);
    light0->makePositional();
    light0->setLinearAttenuation(0.01f);

    light1->turnOn();
    light1->setPosition(12.0f, 8.0f, 10.0f);
    light1->setColour(0.45f, 0.50f, 0.60f, 1.0f);
    light1->setSpecularColour(0.2f, 0.2f, 0.3f, 1.0f);
    light1->makePositional();
    light1->setLinearAttenuation(0.03f);
}


// -----------------------------------------------------------------------------------------
// animateForNextFrame
//
// SEQUENCE  (all times in seconds)
//
//  0  – 3   : Shot 1  — Lego walks forward, camera on him
//  3  – 4   : Shot 2  — Camera shifts to car, lego still walks
//  4  – 5.5 : Shot 3  — Car crashes into lego, lego falls
//  5.5– 7   : Shot 4  — Wide shot, both still
//  7  – 8   : Shot 5  — Camera on helicopter, rotors spin up, heli lifts off
//  8  – 12  : Shot 6  — Helicopter flies from hospital to above lego
//  12 – 13.5: Shot 6b — Helicopter hovers above lego (pause)
//  13.5– 17 : Shot 7  — Helicopter flies from lego to hospital
//  17 – 19  : Shot 7b — Helicopter descends onto helipad, rotors slow
//  19+      : Finished
//
// -----------------------------------------------------------------------------------------

void animateForNextFrame(float time, long frame)
{
    static float lastTime = 0.0f;
    float deltaTime = time - lastTime;
    if (deltaTime > 0.1f) deltaTime = 0.1f;    // clamp large gaps (e.g. after pause)
    lastTime = time;
    // lego position — used across multiple shots
    float lx, ly, lz;
    lego->getPosition(lx, ly, lz);
    // ------------------------------------------------------------------
    // SHOT 1  (0 – 3 s)
    // Lego person walks forward.  Camera focuses on him.
    // ------------------------------------------------------------------
    if (time > 0 && time <= 3)
    {
        // Camera: lego camera
        gCamera.setPosition(-9, 4, 29);
        gCamera.setTarget(lego);

        lego->startWalking();
        lego->animateWalk(deltaTime);

        // Rotors idle on the roof while we wait
        heli->animateFlight(time);
    }

    // ------------------------------------------------------------------
    // SHOT 2  (3 – 4 s)
    // Camera shifts to car.  Lego keeps walking.
    // ------------------------------------------------------------------
    if (time > 3 && time <= 4)
    {
        // Camera: car camera
        gCamera.setPosition(9, 4, -70);
        gCamera.setTarget(C1);

        lego->animateWalk(deltaTime);
        C1->animateDrive(time);

        heli->animateFlight(time);
    }

    // ------------------------------------------------------------------
    // SHOT 3  (4 – 5.5 s)
    // Car crashes toward lego.  When it arrives lego falls.
    // Camera shows both.
    // ------------------------------------------------------------------
    if (time > 4 && time <= 5.5f)
    {
        // Camera: wide shot showing both
        gCamera.setPosition(15, 8, 50);
        gCamera.setTarget(lego);
        lego->animateWalk(deltaTime);
        // Get lego's current world position as the crash target
        float lx, ly, lz;
        lego->getPosition(lx, ly, lz);

        // Car drives hard toward lego — stop 2 units short of him
        bool arrived = C1->crashToward(lx - 2.0f, ly, lz, deltaTime, 30.0f);

        if (arrived && !fallStarted)
        {
            //lego->stopWalking();
            fallStarted = true;
        }

        if (fallStarted)
            lego->fall(deltaTime);

        heli->animateFlight(time);
        s->startSound();
    }

    // ------------------------------------------------------------------
    // SHOT 4  (5.5 – 7 s)
    // Wide shot.  Nothing moves.  Let the moment breathe.
    // ------------------------------------------------------------------
    if (time > 5.5f && time <= 7)
    {
        gCamera.setPosition(15, 8, 50);
        gCamera.setTarget(lego);

        // Finish fall animation if it wasn't quite done yet
        if (!lego->isFallen())
            lego->fall(deltaTime);

        heli->animateFlight(time);
    }
    // ------------------------------------------------------------------
    // SHOT 5  (7 – 14 s)
    // Wide shot of the accident.
    // Camera slowly orbits around the scene for dramatic effect.
    // ------------------------------------------------------------------
    if (time > 7 && time <= 12)
    {
        // Orbit camera — angle increases over time for a slow pan
        float orbitAngle = (time - 11.0f) * 18.0f;     // 0 -> 72 degrees over 4 sec
        float orbitR = 20.0f;  // Changed from 35.0f to 20.0f for closer view
        float camX = lx + orbitR * sinf(orbitAngle * 3.14159f / 180.0f);
        float camZ = lz + orbitR * cosf(orbitAngle * 3.14159f / 180.0f);
        gCamera.setPosition(camX, 8, camZ);
        gCamera.setTarget(lego);

        // Finish fall if not complete yet
        if (!lego->isFallen())
            lego->fall(deltaTime);

        heli->animateFlight(time);
    }
    // ------------------------------------------------------------------
    // SHOT 6  (14 – 16 s)
    // Low close-up on the fallen lego from a different angle.
    // Very dramatic — low angle showing both car and lego on ground.
    // ------------------------------------------------------------------
    if (time > 12 && time <= 14)
    {
        gCamera.setPosition(lx + 8, 4, lz + 8);
        gCamera.setTarget(lego);

        heli->animateFlight(time);
    }
    // ------------------------------------------------------------------
    // SHOT 7  (14 – 16 s)
    // Camera cuts to helicopter on hospital roof.
    // Rotors spin up from idle to full speed.
    // Helicopter lifts off the helipad.
    // ------------------------------------------------------------------
    if (time > 14 && time <= 16)
    {
        gCamera.setPosition(40, 55, -55);
        gCamera.setTarget(heli);

        heli->startFlight();
        heli->animateFlight(time);

        // Smooth lift-off from pad height to cruise
        float t = (time - 14.0f) / 2.0f;       // 0 -> 1
        float liftY = 46.7 + t * 15.3;
        float hx, hy, hz;
        heli->getPosition(hx, hy, hz);
        heli->setPosition(hx, liftY, hz);
        s->stopSound();
        s1->startSound();
    }
    // ------------------------------------------------------------------
    // SHOT 8  (16 – 22 s)
    // Helicopter flies from hospital toward the accident scene.
    // Travels at cruise height. Camera on heli the whole way.
    // ------------------------------------------------------------------
    if (time > 16 && time <= 22)
    {
        gCamera.setPosition(40, 55, -55);
        gCamera.setTarget(heli);

        heli->animateFlight(time);

        // Fly to a point above and slightly past lego at cruise height
        heli->flyTo(lx + 3.0f, 62.0f, lz, deltaTime, 9.0f);
    }

    // ------------------------------------------------------------------
    // SHOT 9  (22 – 32 s)
    // Helicopter descends next to the fallen person and touches the ground.
    // Camera close — shows heli landing beside lego.
    // ------------------------------------------------------------------
    if (time > 22 && time <= 32)
    {
        gCamera.setPosition(lx + 20, 8, lz + 20);
        gCamera.setTarget(heli);

        heli->animateFlight(time);

        // Descend from cruise height down to ground level (Y=2) beside lego
        heli->flyTo(lx + 4.0f, 2, lz, deltaTime, 8.0f);
        s1->stopSound();
        s1->startSound();
    }
    
    // ------------------------------------------------------------------
    // SHOT 10  (32 – 35 s)
    // Helicopter sits on the ground next to lego.
    // Rotors slow slightly (still running but patient is boarding).
    // Camera holds the moment — showing both heli and lego.
    // ------------------------------------------------------------------
    if (time > 32 && time <= 35)
    {
        gCamera.setPosition(-10, 4, 55);
        gCamera.setTarget(lego);

        // Rotors still spinning but heli is grounded — keep animateFlight for rotors
        heli->animateFlight(time);
        s1->stopSound();
        
    }
    // ------------------------------------------------------------------
    // SHOT 11  (35 – 38 s)
    // Helicopter lifts off and flies FORWARD away from accident
    // in the direction of the hospital (not backward).
    // It rises while moving forward — never reversing.
    // Camera pulls back to show the departure.
    // ------------------------------------------------------------------
    if (time > 35 && time <= 38)
    {
        lego->hide();
        gCamera.setPosition(lx - 10, 15, lz + 35);
        gCamera.setTarget(heli);

        heli->startFlight();
        heli->animateFlight(time);

        // Fly away from hospital: +Z and up to cruise height
        heli->flyTo(lx + 4.0f, 62.0f, lz + 22.0f, deltaTime, 10.0f);
        s1->stopSound();
        s1->startSound();
    }

    // ------------------------------------------------------------------
    // SHOT 12  (38 – 40 s)
    // Helicopter rotates 180 degrees in place to face the hospital.
    // Y rotation: 180 (facing away) -> 0 (facing hospital at -Z).
    // ------------------------------------------------------------------
    if (time > 38 && time <= 40)
    {
        gCamera.setPosition(lx - 10, 20, lz + 35);
        gCamera.setTarget(heli);

        heli->animateFlight(time);

        float t = (time - 38.0f) / 2.0f;     // 0 -> 1 over 2 seconds
        float yaw = 180.0f - t * 180.0f;        // 180 -> 0
        heli->setRotation('y', yaw);
        s1->stopSound();
        s1->startSound();
    }

    // ------------------------------------------------------------------
    // SHOT 13  (40 – 44 s)
    // Helicopter faces hospital — flies straight toward it at cruise height.
    // Camera in front of hospital showing heli approaching.
    // ------------------------------------------------------------------
    if (time > 40 && time <= 44)
    {
        gCamera.setPosition(18, 35, 10);
        gCamera.setTarget(heli);

        heli->animateFlight(time);

        heli->flyTo(18.0f, 62.0f, -70.0f, deltaTime, 11.0f);
        s1->stopSound();
        s1->startSound();
    }

    // ------------------------------------------------------------------
    // SHOT 14  (44 – 47 s)
    // Helicopter descends vertically onto the helipad from directly above.
    // ------------------------------------------------------------------
    if (time > 44 && time <= 47)
    {
        gCamera.setPosition(40, 60, -55);
        gCamera.setTarget(heli);

        heli->animateFlight(time);

        float t = (time - 44.0f) / 3.0f;   // 0 -> 1 over 3 seconds
        float landY = 62.0f - t * 15.3f;        // 62 -> 46.7
        heli->setPosition(18.0f, landY, -70.0f);
        s1->stopSound();
        s1->startSound();
    }

    // ------------------------------------------------------------------
    // SHOT 15  (47 – 50 s)
    // Helicopter rests on helipad. Rotors spool down.
    // Wide final shot showing the full hospital.
    // ------------------------------------------------------------------
    if (time > 47 && time <= 50)
    {
        gCamera.setPosition(50, 60, -30);
        gCamera.setTarget(heli);

        heli->setPosition(18.0f, 46.7f, -70.0f);
        heli->stopFlight();
        heli->animateFlight(time);
        s1->stopSound();
    }

    // ------------------------------------------------------------------
    // FINISHED
    // ------------------------------------------------------------------
    if (time >= 51)
        gProgramMode = kpmFinished;
}