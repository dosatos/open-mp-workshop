#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 729
#define reps 100 
#include <omp.h> 

double a[N][N], b[N][N], c[N];
int jmax[N];

double loop1_time;
double loop2_time;
double total_time;
double valid1_sum;
double valid2_sum;
int num_threads;
char *schedule_kind;
int chunk_size;

void init1(void);
void init2(void);
void loop1(void);
void loop2(void);
void valid1(void);
void valid2(void);

void run(void);
void check_best_performance(void);

int main(int argc, char *argv[]) { 
  num_threads = omp_get_num_threads();
  schedule_kind = getenv("SCHED");
  chunk_size = atoi(getenv("CHUNK_SIZE"));
  run();
  printf("%s, %s, %d,  %f, %f, %f, %f, %f\n", getenv("OMP_NUM_THREADS"), schedule_kind, chunk_size, loop1_time, loop2_time, total_time, valid1_sum, valid2_sum);
} // main func


/* This function runs a scenario and stores the performance results into array (performance_results).
 * num_threads: number of threads, 
 * schedule_kind: scheduling type,
 * chunk_size: chunk size.
 */
void run(void) {
  double start1,start2,end1,end2;
  int r;

  init1();
  start1 = omp_get_wtime(); 
  for (r=0; r<reps; r++){ 
    loop1();
  } 
  end1  = omp_get_wtime();
  valid1(); 

  init2();
  start2 = omp_get_wtime(); 
  for (r=0; r<reps; r++){ 
    loop2();
  } 
  end2  = omp_get_wtime(); 
  valid2(); 

  loop1_time = (float)(end1-start1);
  loop2_time = (float)(end2-start2);
  total_time = (float)(end2-start2+end1-start1);
}

/* Below are the init, loop, valid functions
 * The following adjustements were made:
 * init stores the result in a vraiable instead of printing
 * loops is parallelised
 * valid stores the result in a vraiable instead of printing
 */

void init1(void){
  int i,j; 

  double st_init1, et_init1;

  st_init1  = omp_get_wtime(); 

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      a[i][j] = 0.0; 
      b[i][j] = 3.142*(i+j); 
    }
  }

  et_init1  = omp_get_wtime(); 
  // printf("Init time for 1: %f\n", (float)(st_init1-et_init1)); 

} // init1

void init2(void){ 
  int i,j, expr; 
  double st_init2, et_init2;

  st_init2  = omp_get_wtime(); 

  for (i=0; i<N; i++){ 
    expr =  i%( 3*(i/30) + 1); 
    if ( expr == 0) { 
      jmax[i] = N;
    }
    else {
      jmax[i] = 1; 
    }
    c[i] = 0.0;
  } 

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      b[i][j] = (double) (i*j+1) / (double) (N*N); 
    }
  }

  et_init2  = omp_get_wtime(); 
  // printf("Init time for 2: %f\n", (float)(st_init2-et_init2)); 
 
} // init2

void loop1(void) { 
  int i,j;
  
  //schedule type is set automatically above

  #pragma omp parallel for\
  schedule(runtime)\
  shared(a, b)\
  private(j)\
  default(none)
  for (i=0; i<N; i++){ 
    for (j=N-1; j>i; j--){
      a[i][j] += cos(b[i][j]);
    } 
  }

} // loop1



void loop2(void) {
  int i,j,k; 
  double rN2; 

  rN2 = 1.0 / (double) (N*N);

  //schedule type is set automatically above

  #pragma omp parallel for\
  shared(a, b, c, jmax, rN2)\
  schedule(runtime)\
  private(i, j, k)\
  default(none)
  for (i=0; i<N; i++){ 
    for (j=0; j < jmax[i]; j++){
      for (k=0; k<j; k++){ 
        c[i] += (k+1) * log (b[i][j]) * rN2;
      } 
    }
  }
} // loop2

void valid1(void) { 
  int i,j; 
  double suma; 
  
  suma= 0.0; 
  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      suma += a[i][j];
    }
  }
  // printf("Loop 1 check: Sum of a is %lf\n", suma);
  valid1_sum = suma;

} // valid1


void valid2(void) { 
  int i; 
  double sumc; 
  
  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  // printf("Loop 2 check: Sum of c is %f\n", sumc);
  valid2_sum = sumc;
} // valid2



