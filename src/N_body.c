#include <stdio.h>
#include <stdlib.h>

#include "in_out.h"
#include "init_condition.h"
#include "integrator.h"
#include "allocator.h"



int main(int argc, char* argv[]){

    if (argc != 2){
        // the code wants only a .txt file as input
        fprintf(stderr,"The program needs exactly one argument, which is the input file. EXITING...\n");
        exit(1);
    }

    printf("\n\n");
    printf("*********************************************************************\n");
    printf("This program simulates a N-body problem for a uniform density,\n");
    printf("and equal masses, star system, using the Velocity Verlet algorithm.\n");
    printf("*********************************************************************\n\n");


    struct parameters input;
    Vector3D pos; // particles system positions
    Vector3D v; // particles system velocities
    Vector3D acc; // particles system accelerations
    double* masses; //particles system masses 


    input = check_input_file(argv[1]);
    check_dir();
    print_log(input);

    alloc_Vector3D(&pos, input.N);
    alloc_Vector3D(&v, input.N);
    alloc_Vector3D(&acc, input.N);
    alloc_array(&masses, input.N);

    generating_equal_masses(masses,input.N);
    initial_positions_generator(pos, masses,input.N);
    initial_velocity_generator(v,masses,input.N);

    Velocity_Verlet(pos,v,acc,masses,input);

    free_Vector3D(&pos);
    free_Vector3D(&v);
    free_Vector3D(&acc);
    free_array(&masses);

    return EXIT_SUCCESS;
}


