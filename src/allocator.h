#ifndef ALLOCATOR_H
#define ALLOCATOR_H

typedef struct {
    double* raw_data; // data buffer
    double* x;
    double* y;
    double* z;
} Vector3D;

void alloc_Vector3D(Vector3D*, int);
void free_Vector3D(Vector3D*);
void alloc_array(double**,int);
void free_array(double**);

#endif 