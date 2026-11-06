#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "in_out.h"
#include "allocator.h"


/**********************************************************INPUT****************************************************************** */
struct parameters check_input_file(const char* input_file){

    struct parameters input;
    char buffer[60];
    int line_number = 0;
    int counter = 0;
    int NUM_EXPECTED_PARAM = 5;
    FILE* fp;

    printf("Scanning the input file...\n");
    fp = fopen(input_file,"r"); 
    //checking its existance
    if(fp == NULL){
        fprintf(stderr,"The parameters file which has been passed does not exist! Check its name! \n");
        exit(EXIT_FAILURE);
        }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        line_number++;

        // ignoring empty lines or comments
        if (buffer[0] == '\n' || buffer[0] == '#'){
            continue;
        }

        if (parse_line(buffer, "N", "%d", &input.N)){
            counter++;
        }
        else if ( parse_line(buffer, "t", "%lf", &input.t)){
            counter++;
        }
        else if (parse_line(buffer, "dt", "%lf", &input.dt)){
            counter++;
        }
        else if (parse_line(buffer, "epsilon", "%lf", &input.epsilon)){
            counter++;
        }
        else if (parse_line(buffer, "N_file", "%d", &input.N_file)){
            counter++;
        }
        else{
            fprintf(stderr,"Syntatic error in the line %d of the parameter file, you wrote: %s", line_number, buffer);
            fprintf(stderr,"EXITING...\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);

    if (counter != NUM_EXPECTED_PARAM){
        fprintf(stderr,"Error in the parameter file! Some parameter is missing!\n");
        fprintf(stderr,"Expected: %d\n", NUM_EXPECTED_PARAM);
        fprintf(stderr,"Found: %d\n", counter);
        fprintf(stderr,"Exiting...\n");
        exit(EXIT_FAILURE);
    }

    // now checks on the values before the simulation starts!
    // the total number of bodies

    if (input.N <= 0){
        fprintf(stderr, "The total number of boides must be a strictly positive, EXITING...\n");
        exit(EXIT_FAILURE);
    }
    else if (input.N < 2){
        fprintf(stderr, "There must be at least two bodies! EXITING...\n");
        exit(EXIT_FAILURE);
    }

    // now check on the time and the integration step

    if (input.t <= 0){
        fprintf(stderr, "The total time of integration must be positive! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    if (input.dt == 0){
        fprintf(stderr,"The integrations step cannot be equal to zero! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    else if (input.dt < 0){
        fprintf(stdout, "You wrote a negative integration step, it is allowed but was it intentional?\n");
    }
    else if (input.dt >= input.t){
        fprintf(stderr, "The integrations step is equal or greater thant the total integration time, EXITING...\n");
        exit(EXIT_FAILURE);
    }
    

    // now check on the softening lenght

    if (input.epsilon < 0){
        fprintf(stderr, "The softening parameter cannot be negative! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    if (input.epsilon >= 1.0){
        fprintf(stderr, "You wrote a softening lenght which is greater or equeal to the system scale R = 1, EXITING...\n");
        exit(EXIT_FAILURE);
    }


    // now check on the total file number

    if (input.N_file < 1){
        fprintf(stderr, "You need at least on output file! EXITING...\n");
        exit(EXIT_FAILURE);
    }

   
    printf("You selected N=%d, t=%f, dt=%f, epsilon=%lf, N_file=%d.\n",input.N,input.t,input.dt,input.epsilon,input.N_file);
    
    return input;    
}

/****************************************************************** ****************/
int parse_line(const char* buffer, char* element, char* typeof_element, void* target){

    char format_line[64];

    // using the element, typeof_element the formatting line is prepared
    snprintf(format_line, sizeof(format_line), "%s=%s", element, typeof_element);
    // with sscanf we check wheter the formatted line is present and return 1
    if (sscanf(buffer, format_line, target)==1){
        return 1;
    }
    return 0;
}

/************************************************************DIRECTORY CHECK********************************************************** */
// this function creates the data directory 
void check_dir(){
    int check;
    const char* directory = "data"; // data directory

    DIR* dir = opendir(directory); // I try to open it

    // if it does not exist, it is created
    if(dir == NULL){
       check = mkdir(directory, S_IRWXU | S_IRWXG | S_IRWXO); // the data are accesible to everyone on the machine
       // mkdir gives a -1 if something is wrong
       if(check == -1){
        fprintf(stderr,"Impossible to create the data directory, EXITING...\n");
        exit(EXIT_FAILURE);
       }
    // mkdir gives 0 if everything is ok
       if(check == 0){
        printf("The data directory has been created succesfully!\n");
       }
    }
    else{
        // the directory already exist
        printf("The data directory already exists.\n");
        closedir(dir);
    }


}

/*************************************************************PRINTING THE OUTPUT'******************************************************** */

void print_data(Vector3D pos, Vector3D v, int N, double t, int counter_file){
    
  
    const char *directory="data"; 
    char filename[100];
    char time_filename[100]; // buffer
    FILE *fp1, *fp2;

    snprintf(time_filename, sizeof(time_filename), "%s/time_stamp.dat", directory);
   
// creating and updating the time stamp file
    if (counter_file == 0){

        fp2 = fopen(time_filename,"w");

        if(fp2 == NULL){
            fprintf(stderr,"Fatal error in creating the time stamp file, EXITING...\n");
            exit(EXIT_FAILURE);
        }
        else{
            fprintf(fp2,"t\n");
            fprintf(fp2,"%.16e\n",t);
        }      
    }
    else{
        
        fp2 = fopen(time_filename,"a");
        
        if(fp2 == NULL){
            fprintf(stderr,"Fatal error in opening the time stamp file, EXITING...\n");
            exit(EXIT_FAILURE);
        }
        else{
            fprintf(fp2,"%.16e\n",t);
        }     
    }
    
    // The file name is created using the snprintf function "data/step_(number).dat"
    snprintf(filename, sizeof(filename), "%s/step_%d.dat", directory, counter_file);
    fp1 = fopen(filename,"w");
    if(fp1 == NULL){
        fprintf(stderr,"Fatal error in creating an output file, EXITING...\n");
        return;
    }

    fprintf(fp1,"star pos_x pos_y pos_z vx vy vz\n"); // file header

    for(int i = 0; i < N; i++){

        fprintf(fp1,"%d %.16e %.16e %.16e %.16e %.16e %.16e\n",i,pos.x[i],pos.y[i],pos.z[i],v.x[i],v.y[i],v.z[i]);

    }
    fclose(fp1);
    fclose(fp2);
}
/********************************************************PRINTING A LOG OF THE SETTED CONDITIONS************************************/
void print_log(struct parameters input){
    FILE* fdata;

    fdata = fopen("data/simulation_log.dat","w");
    if(fdata == NULL){
        fprintf(stderr,"An error occured during the creation of the log file, EXITING...\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fdata,"N t dt Nfile epsilon\n");
    fprintf(fdata,"%d %lf %lf %d %lf\n", input.N, input.t, input.dt, input.N_file, input.epsilon);
    printf("Log file created correctly!\n");
    fclose(fdata);
}

/************************************************************PRINTING MASSES***************************************************** */
void print_masses(double masses[], int N){
    int i;
    FILE *fmass;
    
    fmass = fopen("data/masses.dat","w"); 
    if(fmass == NULL){
        fprintf(stderr,"An error occured during the creation of the masses.dat file, EXITING...\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fmass,"star m\n");
    for(i=0;i<N;i++){
        fprintf(fmass,"%d %lf\n",i,masses[i]);
    }

    fclose(fmass);
}

/**********************************************************PRINTING ENERGY RELATED QUANTITIES*********************************************************** */
void print_energy(double E, double DE, double Q, double t, int counter_file){
    FILE* fenergy;
    
    //Here we print the starting energy data
    if(counter_file == 0){
        fenergy = fopen("data/energy.dat","w");
        if(fenergy==NULL){
            fprintf(stderr,"An error occured in the opening of energy.dat, EXITING...\n");
            exit(EXIT_FAILURE);
         }
    fprintf(fenergy,"E DE Q t\n");
    fprintf(fenergy,"%lf %lf %lf %lf\n",E,DE,Q,t);
    fclose(fenergy);
    }

    // here the other data using the append
    else{
        fenergy = fopen("data/energy.dat","a");
        if(fenergy==NULL){
            fprintf(stderr,"An error occured in the opening of energy.dat, EXITING...\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fenergy,"%lf %.9lf %lf %lf\n",E,DE,Q,t);
        fclose(fenergy);
    }
}