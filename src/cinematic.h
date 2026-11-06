#ifndef CINEMATIC_H
#define CINEMATIC_H

#include "allocator.h"

double kinetic_energy(Vector3D velocities, const double* masses, int number_of_bodies);
double potential_energy(Vector3D positions, const double* masses, double softening, int number_of_bodies);
void acceleration(Vector3D positions, Vector3D accelerations, const double* masses, double softening, int number_of_bodies);

#endif