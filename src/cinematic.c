#include <math.h>

#include "cinematic.h"
#include "allocator.h"


/***********************************************************ACCELERATION************************************************************* */
void acceleration(Vector3D pos, Vector3D a, const double* masses, double epsilon, int N){

    double eps2 = epsilon * epsilon;

// parallelization of the acceleration calculation using OpenMP

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < N ; i++){

        double x_i, y_i, z_i;
        double ax_i, ay_i, az_i;

        x_i = pos.x[i];
        y_i = pos.y[i];
        z_i = pos.z[i]; 
        
        ax_i = 0.0;
        ay_i = 0.0;
        az_i = 0.0;

        for(int j = 0; j < N ; j++){

            if(j == i){
                continue;
            }

            double r_x, r_y, r_z;

            r_x = pos.x[j] - x_i;
            r_y =  pos.y[j] - y_i;
            r_z = pos.z[j] - z_i;

            double dist, dist_sqr, dist_cubed, inv_dist_cubed;
            double factor;

            // the complete expression for the acceleration is: a_i = sum_j (m_j * r_ij / |r_ij|^3), where r_ij = r_j - r_i

            dist_sqr = r_x * r_x + r_y * r_y + r_z * r_z + eps2;
            dist = sqrt(dist_sqr);
            dist_cubed = dist_sqr * dist;
            inv_dist_cubed = 1.0 / dist_cubed;

            factor = masses[j] * inv_dist_cubed;

            ax_i += factor * r_x;
            ay_i += factor * r_y;
            az_i += factor * r_z;
        }

        a.x[i] = ax_i;
        a.y[i] = ay_i;
        a.z[i] = az_i;
    }
}



/**********************************************************KINETIC ENERGY********************************************************** */
double kinetic_energy(Vector3D v, const double* masses, int N){

    double v_sqr;
    double T = 0.0;
    // we try to use SIMD vectorization to speed up the kinetic energy calculation

    #pragma omp simd reduction(+:T)
    for(int i = 0; i < N; i++){
        v_sqr = v.x[i] * v.x[i] + v.y[i] * v.y[i] + v.z[i] * v.z[i];
        T += 0.5 * masses[i] * v_sqr;
        }

    return T;    
}
/**********************************************************POTENTIAL ENERGY********************************************************* */
double potential_energy(Vector3D pos, const double* masses, double epsilon, int N){

    double eps2 = epsilon * epsilon;
    double U = 0.0;

    // computing the potential energy using OpenMP parallelization and 
    // a reduction clause to sum the contributions from each thread
    #pragma omp parallel for schedule(guided) reduction(+:U)
    for(int i = 0; i < N - 1; i++){

        double x_i, y_i, z_i;
        double m_i;

        x_i = pos.x[i];
        y_i = pos.y[i];
        z_i = pos.z[i];
        m_i = masses[i];
        
        double u_i = 0.0;

        for(int j = i + 1; j < N ; j++){

            double r_x, r_y, r_z;

            r_x = pos.x[j] - x_i;
            r_y = pos.y[j] - y_i;
            r_z = pos.z[j] - z_i;

            double dist, dist_sqr;

            dist_sqr = r_x * r_x + r_y * r_y + r_z * r_z + eps2;
            dist = sqrt(dist_sqr);

            u_i +=  masses[j] / dist;
        }

        U += m_i * u_i;
    }
    return U;
}


