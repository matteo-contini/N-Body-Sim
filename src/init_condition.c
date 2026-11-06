#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "in_out.h"
#include "init_condition.h"
#include "allocator.h"


/************************************************************GENERATING EQUAL MASSES************************************************* */
// The total mass of the system is M=1, so each star has mass 1/N
void generating_equal_masses(double* masses, int N){
    
    // we generate equal mass stars. The system has a total mass M=1, 
    // so each star has mass 1/N
    for(int i = 0; i < N; i++){
        masses[i] = 1.0 / N; 
    }
    print_masses(masses, N);
}
/************************************************************GENERATING INITIAL POSITIONS******************************************** */
// stars are generated in an uniform density sphere with radius R=1

void initial_positions_generator(Vector3D pos, const double* masses, int N){
    double r, theta, phi; //we generate the positions in spherical coordinates
    int seed;
    double x_cm, y_cm, z_cm;

    seed = time(NULL);
    srand(seed);

    for(int i = 0; i < N ; i++){
        //we sample a uniform spherical distribution
        r = pow((double)rand() / RAND_MAX, 1.0/3.0);
        theta = acos(1 - 2 * (double)rand() / RAND_MAX);
        phi = (double)rand() * 2 * M_PI / (RAND_MAX);

        pos.x[i] = r * sin(theta) * cos(phi);
        pos.y[i] = r * sin(theta) * sin(phi);
        pos.z[i] = r * cos(theta);
    }

    // evaluation of the center of mass 

    x_cm = 0.0;
    y_cm = 0.0;
    z_cm = 0.0;

    for(int i = 0; i < N; i++){
        x_cm += masses[i] * pos.x[i];
        y_cm += masses[i] * pos.y[i];
        z_cm += masses[i] * pos.z[i];
    }

    // coordinate system change

    for(int i = 0; i < N; i++){
        pos.x[i] -= x_cm;
        pos.y[i] -= y_cm;
        pos.z[i] -= z_cm;
    }
}
/***********************************************************GENERATING INITIAL VELOCITIES********************************************** */
// the velocities components are uniformly distributed between -0.5 and 0.5 
void initial_velocity_generator(Vector3D v, const double* masses, int N){

    double vx_cm,vy_cm,vz_cm;
    int seed;

    seed=time(NULL);
    srand(seed);

    for(int i = 0; i < N; i++){
        v.x[i] = (1.0 - 2.0 * (double)rand() / RAND_MAX) * 0.5;
        v.y[i] = (1.0 - 2.0 * (double)rand() / RAND_MAX) * 0.5;
        v.z[i] = (1.0 - 2.0 * (double)rand() / RAND_MAX) * 0.5;
    }

    vx_cm = 0.0;
    vy_cm = 0.0;
    vz_cm = 0.0;

    for(int i = 0; i < N; i++){
        vx_cm += masses[i] * v.x[i];
        vy_cm += masses[i] * v.y[i];
        vz_cm += masses[i] * v.z[i];
        }

    for(int i = 0; i < N; i++){
        v.x[i] -= vx_cm;
        v.y[i] -= vy_cm;
        v.z[i] -= vz_cm;
    }
}
/******************************************************************************************************************** */