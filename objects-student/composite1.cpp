// *****************************************************************************************
// *****************************************************************************************
// KXC354 - Computer Graphics & Animation
// Assignment 2
// *****************************************************************************************
//
// composite1.cpp
//
// Medic helicopter composite object.
//
// Coordinate convention (all measurements in world units, helicopter centred at origin):
//   X  = right/left  (positive = right)
//   Y  = up/down     (positive = up)
//   Z  = front/back  (positive = back / toward tail)
//
// *****************************************************************************************

#include "prefix.h"
#include "composite1.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;


// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

composite1::composite1()
    : isFlying(false), rotorAngle(0.0f), tailRotorAngle(0.0f), hoverPhase(0.0f)
{
    setName("MedicHelicopter");

    // =================================================================================
    // FUSELAGE  (extrusion)
    // Profile: side silhouette of the helicopter body in the XY plane
    // Path:    sweeps that profile along Z to create the 3D body, tapering at edges
    // =================================================================================
    fuselage = new extrusion("heli_body_profile.txt", "heli_body_path.txt");
    fuselage->setColour(1, 1, 1);
    fuselage->setSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);
    fuselage->setShininess(40.0f);
    fuselage->setRotation('y', 90);
    fuselage->setDeformation(1.0f, 1.5f, 0.7);
    fuselage->useSmoothShading();
    fuselage->setPosition(-0.5f, 0.7f, -1.8);
    fuselage->setParent(this);

    fuselage2 = new extrusion("heli_body_profile2.txt", "heli_body_path.txt");
    fuselage2->setColour(1, 1, 1);
    fuselage2->setSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);
    fuselage2->setShininess(40.0f);
    fuselage2->setRotation('y', 180);               // same rotation as fuselage1
    fuselage2->setDeformation(1.0f, 1.5f, 0.7);
    fuselage2->setPosition(0, 0, 1);   // same position as fuselage1
    fuselage2->useSmoothShading();
    fuselage2->setParent(fuselage);


    Fwin = new extrusion("heli_fwin_profile.txt", "heli_body_path.txt");
    Fwin->setColour(0.55f, 0.75f, 0.90f, 0.35f);
    Fwin->setSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);
    Fwin->setShininess(40.0f);
    Fwin->setDeformation(1.0f, 1.5f, 0.7);
    Fwin->setPosition(0, 0.0f, 0);
    Fwin->setParent(fuselage);
    Fwin->useSmoothShading();

    Fwin2 = new extrusion("heli_fwin_profile2.txt", "heli_body_path.txt");
    Fwin2->setColour(0.55f, 0.75f, 0.90f, 0.35f);
    Fwin2->setSpecularColour(0.5f, 0.5f, 0.5f, 1.0f);
    Fwin2->setShininess(40.0f);
    Fwin2->setDeformation(1.0f, 1.5f, 0.6);
    Fwin2->setPosition(0, 0, 0.3);   // same position as fuselage1
    Fwin2->setParent(fuselage2);
    Fwin2->useSmoothShading();
    // =================================================================================
    // TAIL BOOM  (extrusion)
    // Circular profile extruded and tapered to form the long tail tube
    // =================================================================================
    tailBoom = new extrusion("heli_tailboom_profile.txt", "heli_tailboom_path.txt");
    tailBoom->setColour(0.8f, 0.0f, 0.0f);
    tailBoom->setSpecularColour(0.4f, 0.4f, 0.4f, 1.0f);
    tailBoom->setShininess(30.0f);
    // Tilt slightly upward toward the tail (tail boom angles up on real helicopters)
    tailBoom->setRotation('y', -90);
    tailBoom->setPosition(-3, 0, 0.5);            // attaches at rear of fuselage
    tailBoom->setParent(fuselage);

    // =================================================================================
    // TAIL FIN  (cube)
    // Vertical stabiliser at the end of the tail boom
    // =================================================================================
    tailFin = new cube;
    tailFin->setColour(0.85f, 0.85f, 0.85f, 1.0f);
    tailFin->setDeformation(0.08f, 1.2f, 0.6f);
    tailFin->setPosition(0, 0.5, 4.5);             // at the tip of the tail boom
    tailFin->setParent(tailBoom);

    // =================================================================================
    // TAIL ROTOR HUB  (sweep)
    // A flat disc created by sweeping the hub profile 360 degrees around Y
    // Requirement: one sweep
    // =================================================================================
    tailRotorHub = new sweep("heli_rotor_hub_profile.txt", 24);
    tailRotorHub->setColour(0.3f, 0.3f, 0.3f, 1.0f);   // dark grey metal
    tailRotorHub->setDeformation(0.4f, 0.4f, 0.4f);
    tailRotorHub->setRotation('z', 90.0f);               // spin axis aligned with X
    tailRotorHub->setPosition(0.2, 0.7f, 0);       // side of tail fin
    tailRotorHub->setParent(tailFin);

    // =================================================================================
    // TAIL ROTOR BLADES  (cubes)
    // Two flat blades parented to the tail rotor hub
    // =================================================================================
    tailRotorBlade1 = new cube;
    tailRotorBlade1->setColour(0.2f, 0.2f, 0.2f, 1.0f);
    tailRotorBlade1->setDeformation(0.06f, 0.8f, 0.12f);
    tailRotorBlade1->setRotation('z', 90.0f, 'y', 90);            // perpendicular to blade1
    tailRotorBlade1->setPosition(0.0f, 0.0f, 0.0f);
    tailRotorBlade1->setParent(tailRotorHub);

    tailRotorBlade2 = new cube;
    tailRotorBlade2->setColour(0.2f, 0.2f, 0.2f, 1.0f);
    tailRotorBlade2->setDeformation(0.06f, 0.8f, 0.12f);
    tailRotorBlade2->setRotation('z', 90.0f);            // perpendicular to blade1
    tailRotorBlade2->setPosition(0.0f, 0.0f, 0.0f);
    tailRotorBlade2->setParent(tailRotorHub);

    // =================================================================================
    // MAIN ROTOR MAST  (cube)
    // Vertical cylinder approximated by a thin tall cube
    // =================================================================================
    mainRotorMast = new cube;
    mainRotorMast->setColour(0.25f, 0.25f, 0.25f, 1.0f);
    mainRotorMast->setDeformation(0.15f, 0.9f, 0.15f);
    mainRotorMast->setPosition(-0.8, 3, 0.3f);      // top of fuselage, slightly forward
    mainRotorMast->setParent(fuselage);

    // =================================================================================
    // MAIN ROTOR HUB  (sweep)
    // Flat disc at top of mast, swept 360 degrees
    // Requirement: one sweep (second sweep)
    // =================================================================================
    mainRotorHub = new sweep("heli_rotor_hub_profile.txt", 32);
    mainRotorHub->setColour(0.3f, 0.3f, 0.3f, 1.0f);
    mainRotorHub->setSpecularColour(0.7f, 0.7f, 0.7f, 1.0f);
    mainRotorHub->setShininess(70.0f);
    mainRotorHub->setDeformation(0.6f, 0.6f, 0.6f);
    mainRotorHub->setPosition(0, 1, 0.0f);       // sits on top of the mast
    mainRotorHub->setParent(mainRotorMast);

    // =================================================================================
    // MAIN ROTOR BLADES  (cubes x4)
    // Four long thin blades radiating from the hub
    // =================================================================================
    blade1 = new cube;
    blade1->setColour(0.15f, 0.15f, 0.15f, 1.0f);
    blade1->setDeformation(3.8f, 0.04f, 0.22f);
    blade1->setPosition(2.0f, 0.0f, 0.0f);              // extends along +X
    blade1->setParent(mainRotorHub);

    blade2 = new cube;
    blade2->setColour(0.15f, 0.15f, 0.15f, 1.0f);
    blade2->setDeformation(3.8f, 0.04f, 0.22f);
    blade2->setPosition(-2.0f, 0.0f, 0.0f);             // extends along -X
    blade2->setParent(mainRotorHub);

    blade3 = new cube;
    blade3->setColour(0.15f, 0.15f, 0.15f, 1.0f);
    blade3->setDeformation(0.22f, 0.04f, 3.8f);
    blade3->setPosition(0.0f, 0.0f, 2.0f);              // extends along +Z
    blade3->setParent(mainRotorHub);

    blade4 = new cube;
    blade4->setColour(0.15f, 0.15f, 0.15f, 1.0f);
    blade4->setDeformation(0.22f, 0.04f, 3.8f);
    blade4->setPosition(0.0f, 0.0f, -2.0f);             // extends along -Z
    blade4->setParent(mainRotorHub);


    // =================================================================================
    // LANDING GEAR — STRUTS  (cubes x4)
    // Four diagonal/vertical struts connecting fuselage to skid runners
    // =================================================================================
    skidStrut1 = new cube;
    skidStrut1->setColour(0, 0, 0, 1.0f);
    skidStrut1->setDeformation(0.1f, 0.8f, 0.1f);
    skidStrut1->setRotation('x', -15.0f);
    skidStrut1->setPosition(0, -1.2, 1.7);       // front-left strut
    skidStrut1->setParent(fuselage);

    skidStrut2 = new cube;
    skidStrut2->setColour(0, 0, 0, 1.0f);
    skidStrut2->setDeformation(0.1f, 0.8f, 0.1f);
    skidStrut2->setRotation('x', -15.0f);
    skidStrut2->setPosition(-2.5, -1.2, 1.5);       // front-left strut
    skidStrut2->setParent(fuselage);

    skidStrut3 = new cube;
    skidStrut3->setColour(0, 0, 0, 1.0f);
    skidStrut3->setDeformation(0.1f, 0.8f, 0.1f);
    skidStrut3->setRotation('x', -15.0f);
    skidStrut3->setPosition(0, -1.2, 1.7);       // front-left strut
    skidStrut3->setParent(fuselage2);

    skidStrut4 = new cube;
    skidStrut4->setColour(0, 0, 0, 1.0f);
    skidStrut4->setDeformation(0.1f, 0.8f, 0.1f);
    skidStrut4->setRotation('x', -15.0f);
    skidStrut4->setPosition(2.5, -1.2, 1.5);       // front-left strut
    skidStrut4->setParent(fuselage2);


    // =================================================================================
    // LANDING SKID RUNNERS  (extrusions)
    // Long horizontal tubes extruded from circular profile
    // Requirement: extrusions (second and third extrusions)
    // =================================================================================


    skidRight = new extrusion("heli_skid_profile.txt", "heli_skid_path.txt");
    skidRight->setColour(0, 0, 0, 1.0f);
    skidRight->setSpecularColour(0.3f, 0.3f, 0.3f, 1.0f);
    skidRight->setShininess(20.0f);
    skidRight->setRotation('y', 90.0f);
    skidRight->setPosition(-3.5, -2, 1.7);           // right side
    skidRight->setParent(fuselage);

    skidLeft = new extrusion("heli_skid_profile.txt", "heli_skid_path.txt");
    skidLeft->setColour(0, 0, 0, 1.0f);
    skidLeft->setSpecularColour(0.3f, 0.3f, 0.3f, 1.0f);
    skidLeft->setShininess(20.0f);
    skidLeft->setRotation('y', 90.0f);                     // runner runs along Z (front-back)
    skidLeft->setPosition(-0.6, -2, 1.7);           // left side, below fuselage
    skidLeft->setParent(fuselage2);




    // =================================================================================
    // MEDIC MARKINGS
    // =================================================================================

    //// Red cross on the side of the fuselage (vertical bar)
    redCross = new cube;
    redCross->setColour(0.85f, 0.05f, 0.05f, 1.0f);       // medical red
    redCross->setRotation('x', -30);
    redCross->setDeformation(0.1, 0.4, 0.04);
    redCross->setPosition(-0.7, 1.5, 1.8);            // left fuselage side
    redCross->setParent(fuselage);

    // Red stripe along the fuselage (left)
    medStripe1 = new cube;
    medStripe1->setColour(0.85f, 0.05f, 0.05f, 1.0f);
    medStripe1->setDeformation(0.5, 0.1, 0.04);
    medStripe1->setPosition(0, 0, 0);
    medStripe1->setParent(redCross);

    //// Red cross on the side of the fuselage (vertical bar)
    redCross2 = new cube;
    redCross2->setColour(0.85f, 0.05f, 0.05f, 1.0f);       // medical red
    redCross2->setRotation('x', -30);
    redCross2->setDeformation(0.1, 0.4, 0.04);
    redCross2->setPosition(0.7, 1.5, 1.8);            // left fuselage side
    redCross2->setParent(fuselage2);

    // Red stripe along the fuselage (left)
    medStripe2 = new cube;
    medStripe2->setColour(0.85f, 0.05f, 0.05f, 1.0f);
    medStripe2->setDeformation(0.5, 0.1, 0.04);
    medStripe2->setPosition(0, 0, 0);
    medStripe2->setParent(redCross2);

    // =================================================================================
    // SEARCHLIGHT  (sphere)
    // Medical/rescue searchlight mounted under the nose
    // =================================================================================
    searchLight = new sphere(8);
    searchLight->setColour(0.95f, 0.90f, 0.70f, 1.0f);    // warm yellow lens tint
    searchLight->setEmissiveColour(0.6f, 0.6f, 0.4f, 1.0f);
    searchLight->setSpecularColour(1.0f, 1.0f, 0.8f, 1.0f);
    searchLight->setShininess(100.0f);
    searchLight->setDeformation(0.25f, 0.18f, 0.25f);
    searchLight->setPosition(0.0f, -0.35f, -3.1f);         // under the nose
    searchLight->setParent(this);

    // Register this composite with the global shape vector so it gets drawn
    gShapeVector.push_back(this);
}

// -----------------------------------------------------------------------------------------
// animateFlight
//
// Call once per frame with the current time (seconds).
// Spins the main and tail rotors. When isFlying is true, adds a gentle hover bob.
// -----------------------------------------------------------------------------------------

void composite1::animateFlight(float time)
{
    float mainRPM = isFlying ? 320.0f : 80.0f;
    float tailRPM = isFlying ? 1800.0f : 480.0f;

    rotorAngle = fmodf(time * mainRPM * 6.0f, 360.0f);
    tailRotorAngle = fmodf(time * tailRPM * 6.0f, 360.0f);

    mainRotorHub->setRotation('y', rotorAngle);

    // z=90 is the base orientation set in the constructor (aligns spin axis with X)
    // x=tailRotorAngle is the spin on top of that base
    tailRotorHub->setRotation('z', 90.0f, 'x', tailRotorAngle);

    // --- gentle hover bob when flying ---
    if (isFlying)
    {
        hoverPhase = time;
        float bob = sinf(hoverPhase * 1.8f) * 0.15f;
        vectr pos = getPosition();
        pos.data[1] = pos.data[1] + bob * 0.008f;
        setPosition(pos);
    }
}


// -----------------------------------------------------------------------------------------
// flyTo
//
// Smoothly moves the helicopter toward (tx, ty, tz) using lerp.
// Call every frame with deltaTime. Returns true when within 0.5 units of target.
// -----------------------------------------------------------------------------------------

bool composite1::flyTo(float tx, float ty, float tz, float deltaTime, float speed)
{
    float x, y, z;
    getPosition(x,y,z);

    float dx = tx - x;
    float dy = ty - y;
    float dz = tz - z;

    float dist = sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist < 0.5f)
    {
        // Snap to target and report arrival
        setPosition(tx, ty, tz);
        return true;
    }

    // Lerp: move a fraction of the remaining distance each frame
    float step = speed * deltaTime;
    if (step > dist) step = dist;   // don't overshoot

    x += (dx / dist) * step;
    y += (dy / dist) * step;
    z += (dz / dist) * step;
    setPosition(x,y,z);

    return false;
}

bool composite1::flyTo(vectr target, float deltaTime, float speed)
{
    return flyTo(target.data[0], target.data[1], target.data[2], deltaTime, speed);
}

// -----------------------------------------------------------------------------------------
// hide  —  hide all parts
// -----------------------------------------------------------------------------------------

void composite1::hide(void)
{
    fuselage->hide();
    tailBoom->hide();
    tailFin->hide();
    tailRotorHub->hide();
    tailRotorBlade1->hide();
    tailRotorBlade2->hide();
    mainRotorMast->hide();
    mainRotorHub->hide();
    blade1->hide();
    blade2->hide();
    blade3->hide();
    blade4->hide();
    skidStrut1->hide();
    skidStrut2->hide();
    skidStrut3->hide();
    skidStrut4->hide();
    skidLeft->hide();
    skidRight->hide();
    redCross->hide();
    medStripe1->hide();
    medStripe2->hide();
    searchLight->hide();
}


// -----------------------------------------------------------------------------------------
// show  —  show all parts
// -----------------------------------------------------------------------------------------

void composite1::show(void)
{
    fuselage->show();
    tailBoom->show();
    tailFin->show();
    tailRotorHub->show();
    tailRotorBlade1->show();
    tailRotorBlade2->show();
    mainRotorMast->show();
    mainRotorHub->show();
    blade1->show();
    blade2->show();
    blade3->show();
    blade4->show();
    skidStrut1->show();
    skidStrut2->show();
    skidStrut3->show();
    skidStrut4->show();
    skidLeft->show();
    skidRight->show();
    redCross->show();
    medStripe1->show();
    medStripe2->show();
    searchLight->show();
}


