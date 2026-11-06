#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "allocator.h"
#include "in_out.h"

void Velocity_Verlet(Vector3D positions, Vector3D velocities, Vector3D accelerations,
    const double* masses, struct parameters params);

#endif