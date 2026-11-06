# N-Body simulation

This program is a simple N-body simulation that solves the problem of simulating the motion of a system of particles under the influence of gravity forces. The simulation uses the Velocity Verlet integration method to compute the positions and velocities of the particles over time. All units are adimensional. The total mass of the system is M = 1, each body has a mass of m = 1/N, the gravitational constant is G = 1, and the starting positions are created sampling a uniform density distribution. 

Get the project:

```bash
git clone https://github.com/matteo-contini/N-Body-Sim.git
```

## Compilation

Before running the simulation, you need to compile the code. You can do this by running the following command in the project directory:

```bash
make all
```

The `makefile` is set up to support two compilers: `gcc` and the Intel compiler `icx`. You can choose which one to use by setting the `COMP` variable in the `makefile`. By default, it is set to use `gcc`. If you have an Intel machine, the use of the Intel compiler is recommended. 
The code also supports multithreading using OpenMP. You can enable it by setting the `OPENMP` variable in the `makefile` to `on` (`off` to turn it off). 
The executable `N_body` will be created in the `bin` directory.

To remove the object files and the executable, you can run:

```bash
make clean
```

## Parameters file
The simulation needs a parameters file to run. It has to be a .txt file with the following format:

```txt
N=1000
t=100.0
dt=0.001
epsilon=0.0012
N_file=100
```

Where:
- `N` is the number of particles in the simulation.
- `t` is the total time of the simulation.
- `dt` is the time step of the simulation.
- `epsilon` is the softening parameter used to avoid singularities in the force calculation.
- `N_file` is the number of output files to be generated during the simulation.

Be careful when setting the parameters, as they can significantly affect the performance and accuracy of the simulation. Expressions like `N = 1000` is not valid, you have to write `N=1000` without spaces. An example of `parameters.txt` file is provided in the `bin` directory.

## Execution
To run the simulation, you need to provide the path to the parameters file as an argument. You can do this by running the following command in the project directory:

```bash
./bin/N_body path/to/parameters.txt
```

If you have enabled OpenMP, you can also set the number of threads to use by setting the `OMP_NUM_THREADS` environment variable before running the simulation. For example, to use 4 threads, you can run:

```bash
export OMP_NUM_THREADS=4
./bin/N_body path/to/parameters.txt
``` 
The total number of usable threads depends on your machine.

## Output
The output of the code is stored in the `data` directory. The output files are named `step_XXXX.dat`, where `XXXX` is the step number. Each output file contains the positions and velocities of all particles at a corresponding time step saved in `time_stamp.dat` file. The output files are in the following format:    

```dat
star_id x y z vx vy vz
```
- A `simulation_log.dat` file is also generated containing the parameters used for the simulation.
- Energy information such has total energy, energy relative error (energy conservation) and Q value are stored in the `energy.dat` file.
- Each mass of the particles is stored in the `masses.dat` file (for now they are all equal but feel free to modify the code to have different masses).

## GIFs
A python script is provided to create GIFs from the output files. It reads the `data` directory and creates a GIF from the output files. Before the execution of the script, it is recommended to create your own Python environment and use the requirements.txt file inside your folder, here an example:

```bash
python -m venv .myenv
source .myenv/bin/activate
python -m pip install -r path/to/requirements.txt
```

You can run the script by executing the following command in the project directory:

```bash
source .myenv/bin/activate
python3 create_gif.py 
```

and it will create a GIF named `Animated_N_body.gif`. 


