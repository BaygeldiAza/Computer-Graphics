## Project Title

Helicopter Rescue Scene

## Project Description

This project is a 3D computer graphics animation created for **KXC354 Computer Graphics & Animation - Assignment 2**.

The scene includes two main composite objects:

1. **Medic Helicopter**
2. **Lego Person**

The helicopter is built using several child objects such as the fuselage, tail boom, rotors, landing skids, medical cross markings, and search light.  
The Lego person is built using a torso, head, arms, hips, neck, and legs.

The project demonstrates the use of parent-child hierarchy, composite objects, sweeps, extrusions, cubes, spheres, textures, and animation.

---

## Main Features

### 1. Medic Helicopter

The helicopter is created as a composite object called `composite1`.

It includes:

- Fuselage
- Second fuselage part
- Front windows
- Tail boom
- Tail fin
- Tail rotor hub
- Tail rotor blades
- Main rotor mast
- Main rotor hub
- Four main rotor blades
- Landing skid struts
- Left and right landing skids
- Red medical cross markings
- Search light

The helicopter uses both **extrusion** and **sweep** objects.

### 2. Lego Person

The Lego person is created as a composite object called `composite2`.

It includes:

- Torso
- Head
- Hips
- Neck
- Left arm
- Right arm
- Left leg
- Right leg

The Lego person can walk and fall during the animation.

---

## Hierarchical Structure

### Medic Helicopter

```text
MedicHelicopter
├── fuselage
│   ├── fuselage2
│   │   ├── Fwin2
│   │   ├── skidStrut3
│   │   ├── skidStrut4
│   │   ├── skidLeft
│   │   └── redCross2
│   │       └── medStripe2
│   ├── Fwin
│   ├── tailBoom
│   │   └── tailFin
│   │       └── tailRotorHub
│   │           ├── tailRotorBlade1
│   │           └── tailRotorBlade2
│   ├── mainRotorMast
│   │   └── mainRotorHub
│   │       ├── blade1
│   │       ├── blade2
│   │       ├── blade3
│   │       └── blade4
│   ├── skidStrut1
│   ├── skidStrut2
│   ├── skidRight
│   └── redCross
│       └── medStripe1
└── searchLight
