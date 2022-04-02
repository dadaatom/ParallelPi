#include <math.h>
#include <iostream>
#include <string.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int npoints = 10000000;
clock_t tStart;

/*  -------------------------------------------------------------------------
 *  Master recieves pi counts from worker threads.
 *  -------------------------------------------------------------------------*/

void master(int circle_count) {
    int i, j, source, start, npts, buffer[2];
    MPI_Status status;

    int sum = circle_count;
    int numtasks;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    /* Store worker's results in results array */
    for (i = 1; i < numtasks; i++) {
        /* Receive first point, number of points and results */
        MPI_Recv(buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        sum += buffer[0];
    }

    double PI = (4.0 * ((double)sum / (double)npoints));
    std::cout << "\nEstimated value of Pi: " << PI << "\n";

    double executionTime = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    std::cout << "Execution Time: " << executionTime << " seconds.\n";
}

/*  -------------------------------------------------------------------------
 *  Workers send the updated values to the master
 *  -------------------------------------------------------------------------*/

void workers(int circle_count) {
    int buffer[1];
    buffer[0] = circle_count;
    MPI_Status status;

    MPI_Send(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    //MPI_Send(&values[1], npoints, MPI_DOUBLE, MASTER, OUT2, MPI_COMM_WORLD);
}


/*  ------------------------------------------------------------------------
 *  Main program
 *  ------------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
    tStart = clock();

    int taskid;
    int numtasks;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    int buf[1];
    if (taskid == 0) {

        if (argc != 1) {
            printf("usage: <number of computations>\n");
        }
        else {
            npoints = atoi(argv[0]);
        }

        buf[0] = npoints;
        MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        buf[0] = 10000;
        MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
        npoints = buf[0];
    }

    if (buf[0] > 0) {
        srand(time(NULL) * taskid);

        int circle_count = 0;
        for (int i = 0; i < npoints / numtasks; i++) {
            double x = ((double)rand() / (double)RAND_MAX);
            double y = ((double)rand() / (double)RAND_MAX);

            if (sqrt(x * x + y * y) <= 1) {
                circle_count++;
            }
        }

        /* Get program parameters and initialize wave values */
        if (taskid == 0) {
            printf("Starting mpi_wave using %d tasks.\n", numtasks);

            master(circle_count);
        }
        else {
            workers(circle_count);
        }
    }

    MPI_Finalize();
    return 0;
}