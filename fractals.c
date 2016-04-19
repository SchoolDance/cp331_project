#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include "mpi.h"

#define MIN(a,b) (a < b ? a : b)

int calculateMag (double, double, double complex, int);

int main (int argc, char** argv) {
  //SET THIS TO 1 IF YOU WANT TO SEE DEBUGGING INFO
  int debug = 1;

  //fractal specific variables
  double x1 = -1.5, y1 = -1.5, x2 = 1.5, y2 = 1.5; //area of the fractal visible
  int width;   //x-resolution
  int height;  //y-resolution
	double c1, c2;  //2 components of the complex number C
  int numColours;

  //initialize C from the command line
  sscanf(argv[1],"%lf",&c1);
  sscanf(argv[2],"%lf",&c2);
  sscanf(argv[3],"%i",&numColours);
  sscanf(argv[4], "%d", &width);
  sscanf(argv[5], "%d", &height);

  //create complex C
  double complex c = c1 + c2 * I;

  //MPI specific variables
  int p;
  int rank;

  MPI_Status status[2];

  //initialize MPI
  MPI_Init(&argc, &argv); // start mpi
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // process rank
  MPI_Comm_size(MPI_COMM_WORLD, &p); // find out the number of process

  int chunk = floor(height / p);
  int r = fmod(height, p);
  int starty = rank * chunk + MIN(rank, r);
  int endy = (rank + 1) * chunk + MIN((rank + 1), r);

  int magnitudes[endy - starty][width];

  int i, j;
  double deltax = (x2 - x1) / width;
  double deltay = (y2 - y1) / height;
  double x, y;
  y = y1 + starty * deltay;

  for (j = starty; j < endy; j++) {
    x = x1;
    for (i = 0; i < width; i++) {
      int mag = calculateMag(x, y, c, numColours);
      magnitudes[j - starty][i] = mag;

      x += deltax;
    }
    y += deltay;
  }


  if (debug && rank > 0) {
    for (j = starty; j < endy; j++) {
      for (i = 0; i < width; i++) {
        // printf("RANK: %d - (%d,%d,%d)\n", rank, j, i, magnitudes[j - starty][i]);
        printf("%d %d %d\n", j, i, magnitudes[j - starty][i]);
      }
    }
    printf("\n\n");
  }


  //BRING ALL THE RESULTS TOGETHER
  //if this is proc 0, listen for arrays from other procs
  if (rank == 0) {
    int finalMags[height][width];
    int offset = endy;

    // copy merged into final array
    for(j = 0; j < endy; j++) {
      for(i = 0; i < width; i++) {
        finalMags[j][i] = magnitudes[j][i];
      }
    }

    //get the array from each source and add it to the final array
    for (int source = 1; source < p; source++) {
      int length;

      //get the size of the array
      MPI_Recv (&length, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status[0]);

      //store the incoming array
      for (j = 0; j < length; j++) {
        int tempArray[width];
        MPI_Recv (&tempArray, width, MPI_INT, source, 1, MPI_COMM_WORLD, &status[1]);

        //add this received array to the final array
        for(int i = 0; i < width; i++) {
          finalMags[offset + j][i] = tempArray[i];
        }
      }

      //increment the offset for inserting into the final array
      offset += length;
    }

    if (debug) {
      for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
          printf("%d %d %d\n", j, i, finalMags[j][i]);
        }
      }
    }

    // printf("DONE");
  }
  else {
    //send the number of rows
    int numRows = endy - starty;
    MPI_Send(&numRows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    //send rows
    for (j = 0; j < numRows; j++) {
      int tempArray[width];
      for (i = 0; i < width; i++) {
        tempArray[i] = magnitudes[j][i];
      }

      MPI_Send(&tempArray, width, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
  }

  //cleanup
  MPI_Finalize();
}

int calculateMag(double z1, double z2, double complex c, int numColours) {
  int i;
  double complex z = z1 + z2 * I;

  for (i = 0; i < numColours; i++) {
    double newx = z1 * z1 - z2 * z2 + creal(c);
    double newy = 2 * z1 * z2 + cimag(c);
    z1 = newx;
    z2 = newy;

    //breakout if value is outside of radius 2
    if (z1 * z1 + z2 * z2 > 4)
      return i;
  }
  return numColours - 1;
}
