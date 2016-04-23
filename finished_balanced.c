#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <mpi.h>

/* MPI tags */
#define WORKTAG     1
#define DEADTAG     2

/* Image resolution */
#define HEIGHT      15000      //x-resolution
#define WIDTH       15000      //y-resolution       
#define NUMCOLORS   50;        // number of colors

/* Areas of fractal visible */
#define X1  -1.5
#define Y1  -1.5      
#define X2  1.5
#define Y2  1.5

typedef struct w_node {
  double        x_val;      // for calculating magnitude
  double        y_val;      // for calculating magnitude
  int           r_idx;      // original location for easy sorting
} worknode;


int calculateMag (double, double, double complex, int);

int main( int argc, char * argv[]){

    int rank; 

    double c1 = 1;          
    double c2 = 0;          

    /* create complex C */
    double complex c = c1 + c2 * I;

    /* initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        /* exec master - assign tasks to slave */
        master(c);
        
        } else {
        /* exec slave - compute point by point */
        slave(c);
    }
    
    /* clean up MPI */
    MPI_Finalize();
}

master(complex c)
{
    /* REGISTER work datatype */
    int nitems = 3;

    int             blocklengths[3] = {1,1,1};
    MPI_Datatype    types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Datatype    mpi_worknode_type;
    MPI_Aint        offsets[3];

    offsets[0] = offsetof(worknode, x_val);
    offsets[1] = offsetof(worknode, y_val);
    offsets[2] = offsetof(worknode, r_idx);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_worknode_type);
    MPI_Type_commit(&mpi_worknode_type);
    
    int             n_cpu, rank, work;
    double          result;
    MPI_Status      status;

    // final magnitude array
    int finalMags[HEIGHT * WIDTH];

    // get number of allocated nodes 
    MPI_Comm_size(MPI_COMM_WORLD, &n_cpu); 

    double deltax = (X2 - X1) / WIDTH;
    double deltay = (Y2 - Y1) / HEIGHT;
    double x, y;

    int row, col, idx;  // idx = linear index
    idx = 0;            // index of work
    y = Y1;             // initial y value

    /* WARNING! Requires minimum 2GB+ on node 0 to avoid overflows */
    worknode *workload = malloc(sizeof(worknode) * (HEIGHT * WIDTH));

    /* assemble work array */
    for (row = 0; row < HEIGHT; row++) {

        /* initial x value */
        x = X1;    
        for (col = 0; col < WIDTH ; col++) {

            workload[idx].x_val = x;           // for mag
            workload[idx].y_val = y;           // for mag
            workload[idx].r_idx = idx;         // original location for easy sorting
            idx++;
            x += deltax;
        }
        y += deltay;
    }

    int num_total = HEIGHT * WIDTH;         // number of pixels left
    int idx_work = 0;                       // current index
    
    /*  initialize all nodes with work */
    for (rank = 1; rank < n_cpu; ++rank) {
        
        worknode sender;
        sender = workload[idx_work];
        idx_work++;             // point to next

        /* send workload */        
        MPI_Send(&sender,       // pixel index 
        1,                      // 4 data item 
        mpi_worknode_type,      // struct
        rank,                   // destination in index rank of n_cpu 
        WORKTAG,                // worktag 
        MPI_COMM_WORLD);        // always use this   

    }

    /* Receive a result from any slave and dispatch a new work request */
    while (idx_work < num_total) {
        
        //printf("%d pixels left to compute\n", (num_total - idx_work));

        int buffer[2];              // reciever
        int pixel_idx, pixel_mag;   // reciever
        
        MPI_Recv(buffer,            // result buffer 
        2,                          // 2 items 
        MPI_INT,                    //data item is int 
        MPI_ANY_SOURCE,             // receive from any sender 
        MPI_ANY_TAG,                // receive any type of message 
        MPI_COMM_WORLD,             // always use this 
        &status);                   // info about received message 

        pixel_mag = buffer[0];
        pixel_idx = buffer[1];
        
        //printf("Computed value of %d at pixel %d\n", pixel_d, pixel_i);
        
        /* merge array */
        finalMags[pixel_idx] = pixel_mag;
        int work[4];

        worknode sender;                // assign more work
        sender = workload[idx_work];    // locate
        idx_work++;                     // point to next

        //printf("sent %f, %f\n", sender.x_val, sender.y_val);
      
        /* send more work back to slave */
        MPI_Send(&sender, 1, mpi_worknode_type, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
        
    }
    
    /*  Receive results for outstanding work requests. */
    for(rank = 1; rank < n_cpu; ++rank) {

        int buffer[2];              // reciever
        int pixel_idx, pixel_mag;   // reciever

        MPI_Recv(buffer,            // result buffer 
        2,                          // 2 items 
        MPI_INT,                    // data item is int 
        MPI_ANY_SOURCE,             // receive from any sender 
        MPI_ANY_TAG,                // receive any type of message 
        MPI_COMM_WORLD,             // always use this 
        &status);                   // info about received message 

        pixel_mag = buffer[0];
        pixel_idx = buffer[1];

        //printf("Computed mag of %d at index %d\n", pixel_d, pixel_i);
        
        /*  merge array */
        finalMags[pixel_idx] = pixel_mag;

    }
    
    /*  Tell all the slaves to exit. */
    for(rank = 1; rank < n_cpu; ++rank) {
        MPI_Send(0, 0, MPI_INT, rank, DEADTAG, MPI_COMM_WORLD);
    }

    int i, j;
    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
          printf("%d %d %d\n", j, i, finalMags[j * HEIGHT + i]);
        }
    }

    free(workload);

}

slave(complex c)

{
    MPI_Status  status;

    double x, y;

    /* REGISTER work datatype */
    int nitems = 3;

    int             blocklengths[3] = {1,1,1};
    MPI_Datatype    types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Datatype    mpi_worknode_type;
    MPI_Aint        offsets[3];

    offsets[0] = offsetof(worknode, x_val);
    offsets[1] = offsetof(worknode, y_val);
    offsets[2] = offsetof(worknode, r_idx);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_worknode_type);
    MPI_Type_commit(&mpi_worknode_type);

    int nc = NUMCOLORS;
    
    for (;;) {

        worknode inbox;
        int outbox[2];          // send 2 back

        /*  recieve work from master node */
        MPI_Recv(
            &inbox,             // buffer
            1,                  // number of itmes
            mpi_worknode_type,  // integer datatype 
            0,                  // from node 0
            MPI_ANY_TAG,        // with any tag
            MPI_COMM_WORLD,     // from around the world
            &status             // with status
        );

        if (status.MPI_TAG == DEADTAG) {
            return;             // already dead
        }

        /* match corresponding params and compute mag */
        x = inbox.x_val;
        y = inbox.y_val;

        int mag; 
        mag = calculateMag(x, y, c, nc);

        outbox[0] = mag;                // magnitude
        outbox[1] = inbox.r_idx;        // relay location

        /* send results back to master */
        MPI_Send(outbox, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

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
