#ifndef IN_OUT_H
#define IN_OUT_H

#include "allocator.h"

struct parameters{
    int N;
    double t;
    double dt;
    double epsilon;
    int N_file;
    };


struct parameters check_input_file(const char* input_file);
int parse_line(const char* buffer, char* element, char* typeof_element, void* target);
void check_dir();
void print_data(Vector3D positions, Vector3D velocities, int number_of_bodies, double time, int file_number);
void print_log(struct parameters);
void print_energy(double total_energy, double energy_error, double Q_value, double time, int file_number);
void print_masses(double*,int);

#endif
