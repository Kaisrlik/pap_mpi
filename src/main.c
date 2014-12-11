#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>

#define BUCKET 10

#define DEBUG 1
//      a = malloc(sizeof(uint8_t) * SIZEX);

inline int max(int * a, int len){
   int temp = 0;
   for (int i = 0; i < len; i++)
      if(a[i] > temp)
         temp = a[i];
   return temp+1;
}

#if DEBUG >= 0
#define loadData()\
   {\
      total = (int *) calloc(sizeof(int) , max(a, SIZEX) );\
      t2 = (int *) calloc(sizeof(int) , max(a, SIZEX) );\
   }
#else
#define loadData()\
   {\
      SIZEX = argv[0];\
      SIZEY = argv[1];\
      a = argv[3];\
      total = calloc(sizeof(int), argv[2]);\
      t2 = (int *) calloc(sizeof(int) , argv[2] );
#endif


void histogram(int * dat, int * t, const double c, const int a, const int b){
//   #pragma opm parallel default(shared) shared(nbucket, a, b)
//   {
      #pragma omp parallel for
         for(int i = a; i < b; i++){
            printf("%d\n", dat[i]);
            t[(int)(dat[i]/c)]++;
         }
//   }
}

int main(int argc, char *argv[]) {
   int numprocs, rank, namelen;
   int iam = 0, np = 1;
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   int SIZEX = 10, SIZEY;
   int * a, *total, *t2;
      SIZEX = 10;
      int x[10] = {1,0,5,4,2, 1,3,1,10,10};
      a = x;
   loadData();

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Get_processor_name(processor_name, &namelen);

 #pragma omp parallel default(shared) private(iam, np)
  {
    np = omp_get_num_threads();
    iam = omp_get_thread_num();
    printf("Hello from thread %d out of %d from process %d out of %d on %s \n", iam, np, rank, numprocs, processor_name);
  }

   MPI_Bcast(a, SIZEX, MPI_INT, 0, MPI_COMM_WORLD); //distibute data

   int i1 = rank * (SIZEX / numprocs);
   int i2 = (rank + 1) * (SIZEX / numprocs);
   histogram(a, total, SIZEX/BUCKET, i1, i2);


   MPI_Reduce(&total[i1], &t2[i1], i2-i1+1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Finalize();

   for (int i = 0; i < max(a,SIZEX); i++) {
      printf("%d ", t2[i]);
   }
   printf("\n");

   return 0;
}
