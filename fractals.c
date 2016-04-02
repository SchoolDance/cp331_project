#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

//int * arrayer(int);
//int cmpfunc (const void *, const void *);

int main (int argc, char** argv) {
  //SET THIS TO 1 IF YOU WANT TO SEE DEBUGGING INFO
  int debug = 0;
  
  int p;
  int rank;
  
  MPI_Status status[2];
  
  //initialize MPI
  MPI_Init(&argc, &argv); // start mpi
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // process rank
  MPI_Comm_size(MPI_COMM_WORLD, &p); // find out the number of process
  
  //BRING ALL THE RESULTS TOGETHER
  //if this is proc 0, listen for arrays from other procs
  /*if (rank == 0) {
    //get the array from each source and add it to the final array
    for (int source = 1; source < p; source++) {
      //get the size of the array
      MPI_Recv (&length, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status[0]);

      //store the incoming array
      int tempArray[length];
      MPI_Recv (&tempArray, length, MPI_INT, source, 1, MPI_COMM_WORLD, &status[1]);      
    }    
  }
  else {
    //send size of array
    MPI_Send(&newLength, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    //send array
    MPI_Send(&merged, newLength, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }*/

  //cleanup
  MPI_Finalize();
}

/*int cmpfunc (const void * a, const void * b) {
   return (*(int*)a - *(int*)b);
}

int * arrayer (int size) {
  int *arr = malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    arr[i] = rand() % 10000;
  }

  qsort(arr, size, sizeof(int), cmpfunc);
  return arr;
}*/
