# Set up the flags you want in FFLAGS and compile with compiler specified in FC
# All objects will be compiled with the options specified in FFLAGS
#

#flags for Cray
FFLAGS =
FC = ftn

#flags for Intel 
#FFLAGS = -O3  -openmp
#FC = ifort

# flags for PGI 
#FFLAGS = -O3  -mp  
#FC = pgf90

# flags for GNU 
#FFLAGS = -O3  -fopenmp  
#FC = gfortran

# Suffixes rules to control how .o and .mod files are treated
.SUFFIXES:
.SUFFIXES: .f90 .o .mod 
.f90.o:
	$(FC) $(FFLAGS)  -c $< 
.f90.mod:
	$(FC) $(FFLAGS)  -c $< 
#
#
# Declare all the separate files in one as OBJ
# These are the files created by fsplit and are what the executable depends on
#
OBJ = mod_setprecision.o fcc.o forces.o dfill.o domove.o dscal.o main.o \
      mkekin.o mxwell.o prnout.o velavg.o random.o 
#
##########################################################
#
# Rules for making executable and cleaning directory
#
# This rule says that the executable md depends on the object files in OBJ
# and that they need to be complied then linked to produce the executable.
#
md:	$(OBJ)
	$(FC) $(FFLAGS) -o $@ $(OBJ) 

mod_setprecision.o mod_setprecision.mod: mod_setprecision.f90        
	$(FC) $(FFLAGS) -c mod_setprecision.f90 

#
# This is a rule for cleaning out object files and the executable.
# If the comand "make clean" is typed it will work
#
clean:
	rm *.o *.mod md
#


