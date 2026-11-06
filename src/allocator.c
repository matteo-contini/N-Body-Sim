#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"


void alloc_Vector3D(Vector3D* v, int N){
    // key idea, an entire memory segment for a 3D vector
    v->raw_data = calloc(3 * N, sizeof(double)); 
    
    if (v->raw_data == NULL){
        fprintf(stderr, "A memory allocation error occured during the execution! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    v->x = v->raw_data;
    v->y = v->raw_data + N;
    v->z = v->raw_data + 2 * N;
}


void free_Vector3D(Vector3D *v){
    free(v->raw_data);  
    v->x = NULL;
    v->y = NULL;
    v->z = NULL;
}


void alloc_array(double **arr, int N){
    *arr = calloc(N, sizeof(double));
    
    if(*arr==NULL){
        fprintf(stderr,"An allocation error occured in the code. EXITING...\n");
        exit(EXIT_FAILURE);
    }
}


void free_array(double **arr){
    free(*arr);
    *arr = NULL;
}






