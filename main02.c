#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>
#include <time.h>
 
bool jePrvocislo(long x){
  for(int i = x-1;i>1;i--){
    if((x%i) == 0)return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  int numprocs, rank, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int start = 1000;
  int end = 1100;
  int counter = 0;
  struct timespec start, stop;
  clock_gettime(CLOCK_REALTIME, &start);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);
 
  MPI_Bcast(start, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(end, 1, MPI_INT, 0, MPI_COMM_WORLD);
//  printf("Hello from process %d out of %d on %s\n",rank, numprocs, processor_name);

  #pragma omp parallel for num_threads(1) shared (counter)
  for(long i = start;i<end;i++){
    if(jePrvocislo(i)){
      printf("%ld \n",i);
      counter++;
    }
  }
  if(rank!=0)MPI_Reduce(&counter, &counter2, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();

  if(rank == 0){
      clock_gettime(CLOCK_REALTIME, &stop);
      double elapsed = ( stop.tv_sec - start.tv_sec )*1000 + (double)( stop.tv_nsec - start.tv_nsec )/(double)1000000;
      printf("%.6f", elapsed);
   }
  return 0;
}


