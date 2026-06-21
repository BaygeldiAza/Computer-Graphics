// *****************************************************************************************
// KXC354 - Computer Graphics & Animation
// Assignment 2
// *****************************************************************************************
//
// car.cpp
//
// Limousine composite object.
//
// Coordinate convention:
//   X = right/left   (positive = right)
//   Y = up/down      (positive = up)
//   Z = front/back   (positive = toward viewer)
//
// *****************************************************************************************

#include "prefix.h"
#include "car.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;


car::car()
{
    setName("Limousine");

    // =================================================================================
    // BODY LEFT SIDE  (extrusion)
    // Side silhouette profile swept along Z to form the left body panel.
    // =================================================================================
    bodyL = new extrusion("limo_body_profile.txt", "limo_body_path.txt");
    bodyL->setColour(1, 0, 0);     // black limo
    bodyL->setRotation('y', 90.0f);
    bodyL->setDeformation(1.0f, 1.5f, 1.0f);
    bodyL->setPosition(5.0f, 0.55f, -1.4f);
    bodyL->setParent(this);
    bodyL->useSmoothShading();

    // =================================================================================
    // BODY RIGHT SIDE  (extrusion — mirror profile)
    // =================================================================================
    bodyR = new extrusion("limo_body_profile2.txt", "limo_body_path.txt");
    bodyR->setColour(1, 0, 0);
    bodyR->setRotation('y', -90);
    bodyR->setDeformation(1.0f, 1.5f, 1.0f);
    bodyR->setPosition(5.0f, 0.55f, -1.4f);            // same position as bodyL
    bodyR->setParent(this);
    bodyR->useSmoothShading();

    sideL = new sweep("limo_body_profile.txt", 100);
    sideL->setColour(0.08f, 0.08f, 0.08f, 1.0f);       // black limo
    sideL->setRotation('y', 90.0f);
    sideL->setDeformation(0.1, 1.15, 0.79);
    sideL->setPosition(0, 0, -2.1);
    sideL->setParent(bodyL);


    sideR = new sweep("limo_body_profile.txt", 100);
    sideR->setColour(0.08f, 0.08f, 0.08f, 1.0f);       // black limo
    sideR->setRotation('y', 90.0f);
    sideR->setDeformation(0.1, 1.15, 0.79);
    sideR->setPosition(0, 0, 2.1);
    sideR->setParent(bodyL);


    windscreen = new cube;
    windscreen->setColour(0, 0, 0);
    windscreen->setPosition(-1.3, 1.87, 0);
    windscreen->setDeformation(0.01, 1, 2);
    windscreen->setRotation('z', -75);
    windscreen->setParent(bodyL);

    light1 = new cube;
    light1->setColour(0, 0, 0);
    light1->setPosition(-4, 1.2, -1.5);
    light1->setDeformation(0.01, 0.3, 0.3);
    light1->setRotation('z', -60);
    light1->setParent(bodyL);

    light2 = new cube;
    light2->setColour(0, 0, 0);
    light2->setPosition(4, 1.2, -1.5);
    light2->setDeformation(0.01, 0.3, 0.3);
    light2->setRotation('z', 60);
    light2->setParent(bodyR);



    // =================================================================================
    // WHEELS  (sweep x4)
    // Each wheel is a tyre cross-section swept 360 degrees around Y.
    // =================================================================================
    wheelFL = new sweep("limo_wheel_profile.txt", 32);
    wheelFL->setColour(0.12f, 0.12f, 0.12f, 1.0f);      // dark rubber
    wheelFL->setDeformation(1.0f, 1.0f, 1.0f);
    wheelFL->setPosition(-2, 0.2, 2.3);          // front-left
    wheelFL->setRotation('x', 90);
    wheelFL->setParent(bodyL);

    wheelFR = new sweep("limo_wheel_profile.txt", 32);
    wheelFR->setColour(0.12f, 0.12f, 0.12f, 1.0f);
    wheelFR->setDeformation(1.0f, 1.0f, 1.0f);
    wheelFR->setPosition(-2, 0.2, 2.3);          // front-right
    wheelFR->setParent(bodyR);
    wheelFR->setRotation('x', 90);

    wheelRL = new sweep("limo_wheel_profile.txt", 32);
    wheelRL->setColour(0.12f, 0.12f, 0.12f, 1.0f);      // dark rubber
    wheelRL->setDeformation(1.0f, 1.0f, 1.0f);
    wheelRL->setPosition(2, 0.2, 2.3);          // front-left
    wheelRL->setRotation('x', 90);
    wheelRL->setParent(bodyL);

    wheelRR = new sweep("limo_wheel_profile.txt", 32);
    wheelRR->setColour(0.12f, 0.12f, 0.12f, 1.0f);
    wheelRR->setDeformation(1.0f, 1.0f, 1.0f);
    wheelRR->setPosition(2, 0.2, 2.3);           // rear-right
    wheelRR->setParent(bodyR);
    wheelRR->setRotation('x', 90);
    // Initialize animation variables
    driveSpeed = 0.2;
    currentDistance = 0.0f;
    wheelAngle = 0.0f;

    gShapeVector.push_back(this);
}
void car::animateDrive(float time)
{
    // Simple straight driving
    currentDistance = driveSpeed * time;
    float x, y, z;
    getPosition(x, y, z);
    // Move forward in Z direction
    setPosition(x, y, z + currentDistance);

    // Rotate wheels
    float wheelRotation = currentDistance * 50.0f;
    wheelFL->setRotation('y', -90 + wheelRotation, 'x', 90);
    wheelFR->setRotation('y', -90 + wheelRotation, 'x', 90);
    wheelRL->setRotation('y', -90 + wheelRotation, 'x', 90);
    wheelRR->setRotation('y', -90 + wheelRotation, 'x', 90);
}
// -----------------------------------------------------------------------------------------
// crashToward
//
// Moves the car directly toward (tx, ty, tz) at the given speed.
// Spins the wheels as it goes. Returns true when within 1.0 unit of target.
// -----------------------------------------------------------------------------------------


bool car::crashToward(float tx, float ty, float tz, float deltaTime, float speed)
{
    float x, y, z;
    getPosition(x,y,z);

    float dx = tx - x;
    float dy = ty - y;
    float dz = tz - z;

    float dist = sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist < 1.0f)
    {
        setPosition(tx, ty-1, tz);
        return true;    // arrived — caller should trigger lego->fall()
    }

    float step = speed * deltaTime;
    if (step > dist) step = dist;

    x += (dx / dist) * step;
    y += (dy / dist) * step;
    z += (dz / dist) * step;
    setPosition(x-0.1,0,z);

    // Spin wheels proportional to distance covered
    wheelAngle += step * 50.0f;
    if (wheelAngle > 360.0f) wheelAngle -= 360.0f;

    wheelFL->setRotation('y', -90.0f + wheelAngle, 'x', 90.0f);
    wheelFR->setRotation('y', -90.0f + wheelAngle, 'x', 90.0f);
    wheelRL->setRotation('y', -90.0f + wheelAngle, 'x', 90.0f);
    wheelRR->setRotation('y', -90.0f + wheelAngle, 'x', 90.0f);

    return false;
}



void car::hide(void)
{
    bodyL->hide();
    bodyR->hide();
    wheelFL->hide();
    wheelFR->hide();
    wheelRL->hide();
    wheelRR->hide();
    light1->hide();
    light2->hide();
    windscreen->hide();
    object3d::hide();
}


void car::show(void)
{
    bodyL->show();
    bodyR->show();
    wheelFL->show();
    wheelFR->show();
    wheelRL->show();
    wheelRR->show();
    light1->show();
    light2->show();
    windscreen->show();
    object3d::show();
}

