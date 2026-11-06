#ifndef INIT_CONDITION_H
#define INIT_CONDITION_H

#include "allocator.h"

void generating_equal_masses(double* masses,int number_of_bodies);
void initial_positions_generator(Vector3D pos,const double* masses, int number_of_bodies);
void initial_velocity_generator(Vector3D v, const double* masses, int number_of_bodies);

#endif