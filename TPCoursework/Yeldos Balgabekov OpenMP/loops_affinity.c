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

void init1(void);
void init2(void);
void loop1(int left, int right);
void loop2(int left, int right);
void valid1(void);
void valid2(void);
void run(int loop);


int main(int argc, char *argv[]) {   
  double start1,start2,end1,end2;
  int r;

  init1();
  start1 = omp_get_wtime(); 
  for (r=0; r<reps; r++){ 
    run(1);
  } 
  end1  = omp_get_wtime();
  valid1(); 
  printf("Total time for %d reps of loop 1 = %f\n",reps, (float)(end1-start1)); 

  init2();
  start2 = omp_get_wtime(); 
  for (r=0; r<reps; r++){ 
    run(2);
  } 
  end2  = omp_get_wtime(); 
  valid2();

  loop1_time = (float)(end1-start1);
  loop2_time = (float)(end2-start2);
  total_time = (float)(end2-start2+end1-start1);
  printf("Total time for %d reps of loop 2 = %f\n",reps, (float)(end2-start2)); 
  
  // printf("%s, 0, 0, %f, %f, %f, %f, %f\n", 
  //        getenv("OMP_NUM_THREADS"), 
  //        loop1_time, 
  //        loop2_time, 
  //        total_time, 
  //        valid1_sum, 
  //        valid2_sum);

} // main func


void run(int loop) {

  int number_of_threads = omp_get_max_threads();
  int size = (int) ceil((double) N / (double) number_of_threads);
  float proportion = 1.0 / number_of_threads;
  int left_pointers[number_of_threads];
  int right_pointers[number_of_threads];
  int thread_id;

  #pragma omp parallel default(none) shared(loop, left_pointers, right_pointers, number_of_threads, size, proportion)
  {

    int thread_id = omp_get_thread_num();
    left_pointers[thread_id] = thread_id * size;
    
    if((thread_id + 1) * size > N) {
      right_pointers[thread_id] = N;
    } else {
      right_pointers[thread_id] = (thread_id + 1) * size;
    }
    
    // An initial and the following workload will be given below in 
    // the critical section. Also this helps to gradually complete the
    // tasks and leave opportunity to other threads to help later on.
    int left_edge = 0; 
    int right_edge = 0;

    // repeat untill 
    int done = 0;
    while(done != 1){

      if(loop==1){
        loop1(left_edge, right_edge);  
      } else {
        loop2(left_edge, right_edge);
      } 

      #pragma omp critical
      {
        int slow_id = -1; // id of the thread to help to
        int slow_thread_items = 0; // number of items to help with
        int chunk_size;

        // The thread will work with it's own tasks
        // until they will be exhausted.
        if (right_pointers[thread_id] - left_pointers[thread_id] > 0)
        {
          slow_id = thread_id;
        } else {
          // Once finished with own tasks it will help to a "slowest" thread 
          // (to the one with the maximum count of tasks)
          int i;
          for(i=0; i < number_of_threads; i++) {
            int items_count = right_pointers[i] - left_pointers[i];
            if(items_count>slow_thread_items){
              // find thread with largest iterations count
              slow_id = i;
            }
          }
        }

        if(slow_id == -1) {
          done = 1;
        } else {
          slow_thread_items = right_pointers[slow_id] - left_pointers[slow_id];
          chunk_size = (int) ceil(proportion * slow_thread_items);
          // Reassign the last "chunk_size" of undone actions 
          // of the slow thread
          right_edge = right_pointers[slow_id];
          left_edge = right_pointers[slow_id] - chunk_size;
          right_pointers[slow_id] = right_pointers[slow_id] - chunk_size;
        }
      }
    }
  }
}


void init1(void){
    int i,j; 

    for (i=0; i<N; i++){ 
        for (j=0; j<N; j++){ 
            a[i][j] = 0.0; 
            b[i][j] = 3.142*(i+j); 
        }
    }

}


void init2(void){ 
    int i,j, expr; 

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
 
} 


void loop1(int left, int right) {
  int i,j;
  
  for (i=left; i<right; i++){ 
    for (j=N-1; j>i; j--){
      a[i][j] += cos(b[i][j]);
    } 
  }
} // loop1


void loop2(int left, int right) {
  int i,j,k; 
  double rN2; 

  rN2 = 1.0 / (double) (N*N);

  for (i=left; i<right; i++){ 
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
  printf("Loop 1 check: Sum of a is %lf\n", suma);
  // valid1_sum = suma;

} // valid1


void valid2(void) { 
  int i; 
  double sumc; 
  
  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  printf("Loop 2 check: Sum of c is %f\n", sumc);
  // valid2_sum = sumc;
} // valid2



