#**************************************************************************************
#                           MAKEFILE for the N_body program
#                           author: Matteo Contini
#***************************************************************************************

COMP ?= icx
OPENMP ?= on

ifeq ($(strip $(COMP)),icx)
  ifeq ($(strip $(OPENMP)),on)
    CFLAG = -O3 -xHost -qopenmp -Wall
  else ifeq ($(strip $(OPENMP)),off)
    CFLAG = -O3 -xHost -Wall
  else
    $(error Unsupported OPENMP option. Please use on or off.)
  endif
else ifeq ($(strip $(COMP)),gcc)
  ifeq ($(strip $(OPENMP)),on)
    CFLAG = -O3 -Wall -march=native -fopenmp
  else ifeq ($(strip $(OPENMP)),off)
    CFLAG = -O3 -Wall -march=native
  else
    $(error Unsupported OPENMP option. Please use on or off.)
  endif
else
  $(error Unsupported compiler. Please use gcc or icx.)
endif

# source code
SRC_DIR=src/
SRC=$(SRC_DIR)cinematic.c \
	$(SRC_DIR)init_condition.c \
	$(SRC_DIR)in_out.c\
	$(SRC_DIR)integrator.c\
	$(SRC_DIR)allocator.c\
	$(SRC_DIR)N_body.c


# object files
OBJ=$(SRC_DIR)cinematic.o \
	$(SRC_DIR)init_condition.o \
	$(SRC_DIR)in_out.o\
	$(SRC_DIR)integrator.o\
	$(SRC_DIR)allocator.o\
	$(SRC_DIR)N_body.o

EXE=N_body # executable name


all: $(EXE)  # target: the executable file

$(EXE):$(OBJ)  # we need to compile the object files to create the executable

	$(COMP) $(CFLAG) -o bin/$(EXE) $(OBJ) -lm

%.o:%.c

	$(COMP) $(CFLAG) -c $< -o $@  

clean:
	rm -f src/*.o

