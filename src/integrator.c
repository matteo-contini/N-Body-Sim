#include <stdio.h>
#include <math.h>

#include "in_out.h"
#include "cinematic.h"
#include "integrator.h"
#include "allocator.h"



/*********************************************************VERLET DELLE VELOCITA'***************************************************** */
void Velocity_Verlet(Vector3D pos, Vector3D v, Vector3D a, const double* masses, struct parameters input){

    int N_step;
    int step_trigger,step_print;
    int counter_file = 0;
    double time = 0.0;
    
    N_step = (int)(input.t / input.dt); //integration steps
    step_print = (int)(N_step / input.N_file); //basic unit for print stepping
    step_trigger = step_print; // when reached, that particular step gets printed

    double T;
    double U;
    double E, E0;
    double Q;
    double energy_error = 0.0;

    T = kinetic_energy(v, masses, input.N);
    U = potential_energy(pos, masses, input.epsilon, input.N);
    E0 = T - U; 
    Q = 2 * T / fabs(U);

    print_data(pos,v,input.N,time,counter_file);
    print_energy(E0, energy_error, Q, time, counter_file);

    printf("\rSimulation in progress, please wait    ");

    acceleration(pos, a, masses, input.epsilon, input.N); // acceleration at the first step
    
    double dt_half = 0.5 * input.dt;    

    for(int k = 1; k <= N_step; k++){

        time+=input.dt;

        // updating first the velocities
        // x(n+1) = x(n) + v(n) * dt + 0.5 * a(n) * dt^2
        // v(n+1) = v(n) + 0.5 * (a(n) + a(n+1)) * dt
        // now  x(n+1) = x(n) + dt * [v(n) + 0.5 * a(n) * dt]
        // and v(n+1) = [v(n) + 0.5 * dt * a(n)] + 0.5 * dt * a(n+1)
        // we can compute first the term in square brackets, then update the positions, 
        // then compute the new accelerations, and finally update the velocities with the correction term
        // thus we can avoid storing the previous accelerations, and save memory

        #pragma omp simd
        for(int i = 0; i < input.N; i++){

            v.x[i] = v.x[i] + a.x[i] * dt_half;
            v.y[i] = v.y[i] + a.y[i] * dt_half;
            v.z[i] = v.z[i] + a.z[i] * dt_half;
         
            pos.x[i] = pos.x[i] + input.dt * v.x[i];
            pos.y[i] = pos.y[i] + input.dt * v.y[i];
            pos.z[i] = pos.z[i] + input.dt * v.z[i];
         }

        
        // computing the new accelerations
        acceleration(pos, a, masses, input.epsilon, input.N);
        
        // updating the velotities with a correction term
        #pragma omp simd
        for(int j = 0; j < input.N; j++){
            v.x[j] = v.x[j] + dt_half * a.x[j];
            v.y[j] = v.y[j] + dt_half * a.y[j];
            v.z[j] = v.z[j] + dt_half * a.z[j];
        }

        // data gest printed at certain steps, to avoid too many files being created 
        // and to avoid slowing down the simulation too much
         if(k == step_trigger){
            counter_file++;

            if ((counter_file % 4) == 0){
                printf("\rSimulation in progress, please wait    ");
                fflush(stdout);
            }
            else if ((counter_file % 4) == 1){
                printf("\rSimulation in progress, please wait.   ");
                fflush(stdout);
            }
            else if ((counter_file % 4) == 2){
                printf("\rSimulation in progress, please wait..  ");
                fflush(stdout);
            }
            else if ((counter_file % 4) == 3){
                printf("\rSimulation in progress, please wait... ");
                fflush(stdout);
            }
            
            print_data(pos,v,input.N,time,counter_file);
            T = kinetic_energy(v, masses, input.N);
            U = potential_energy(pos, masses, input.epsilon, input.N);
            E = T - U;
            Q = 2 * T / fabs(U);
            energy_error = fabs((E - E0) / E0);
            print_energy(E, energy_error, Q, time, counter_file);
            step_trigger += step_print;
        }
    }
    printf("\rSimulation ended SUCCESFULLY!      \n");
}
