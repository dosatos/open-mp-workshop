#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

# define NPOINTS 2000
# define MAXITER 2000


struct complex{
  double real;
  double imag;
};

int main(){
  int i, j, iter, numoutside = 0;
  double area, error, ztemp;
  struct complex z, c;
  double starttime, endtime;

/*
 *   
 *
 *     Outer loops run over npoints, initialise z=c
 *
 *     Inner loop has the iteration z=z*z+c, and threshold test
 */

  starttime = omp_get_wtime();

//#pragma omp parallel reduction(+:numoutside) private(i, j, c, z, ztemp, iter) default(none)
  #pragma omp parallel private(i, j, c, z, ztemp, iter) shared(numoutside)  default(none)
  {
  #pragma omp master
  {
  numoutside = 0;
  // int start = (omp_get_thread_num() * NPOINTS )/ omp_get_num_threads();
  // int end = ((omp_get_thread_num() + 1) * NPOINTS) / omp_get_num_threads();
  // printf("thread = %d, start = %d, end = %d\n", omp_get_thread_num(), start,  end);
  
  for (i=0; i<NPOINTS; i++) {
    #pragma omp task
    {
      int tmp = 0;
      for (j=0; j<NPOINTS; j++) {
        c.real = -2.0+2.5*(double)(i)/(double)(NPOINTS)+1.0e-7;
        c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
        z=c;
        for (iter=0; iter<MAXITER; iter++){
	  ztemp=(z.real*z.real)-(z.imag*z.imag)+c.real;
	  z.imag=z.real*z.imag*2+c.imag; 
	  z.real=ztemp; 
	  if ((z.real*z.real+z.imag*z.imag)>4.0e0) {
            // #pragma omp atomic
	    // numoutside++; 
            tmp++;
	    break;
	  } // if
        } // for iter
      } // for j
      #pragma omp atomic
      numoutside = numoutside + tmp;
    } // task
  } // for i
  } // master
  } // parallel

/*
 *  Calculate area and error and output the results
 */


    endtime = omp_get_wtime();
//    printf("%f\n", endtime);
    printf("Time: %f\n", endtime-starttime);
//    printf("Hello world\n");

    area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
    error=area/(double)NPOINTS;

    printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);

  }
