#include <math.h>
#include <iostream>
#include <string.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    MPI_Comm comm;
    int processId = 0;
    int numTasks = 0;
    int numWorker = 0;
    
    int numWorkers = 5;
    int numComputations = 10000000;
    int jobSize = 100;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    double startTime = MPI_Wtime();
    
    if (argc != 4) {
        printf("usage: %s <number of workers> <number of computations> <job size>\n", argv[0]);
    }
    else {
        numWorkers = atoi(argv[1]);
        numComputations = atoi(argv[2]);
        jobSize = atoi(argv[3]);
    }
    printf("Spawning worker processes...\n");
    
    MPI_Comm_spawn("Worker", MPI_ARGV_NULL, numWorkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm, MPI_ERRCODES_IGNORE); // <- Doesn't create workers?
    MPI_Comm_size(comm, &numWorker); //Updates task count after Comm_spawn

    int computationsRemaining = numComputations;
    int computationsRecieved = 0;

    if (numWorkers > 0) {
        printf("Assigning initial jobs...\n");

        for (int i = 0; i < numWorkers; i++) {
            int buff[1];
            buff[0] = std::min(jobSize, computationsRemaining);
            MPI_Send(buff, 1, MPI_INT, i, 1, comm);
            computationsRemaining -= jobSize;
            if (computationsRemaining <= 0) {
                break;
            }
        }

        printf("Handling jobs for %d computations...\n", numComputations);

        int sum = 0;
        while (computationsRecieved < numComputations) {
            MPI_Status status;
            int buff[1];

            MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);
            //printf("     Recieving data from worker %d", status.MPI_SOURCE);

            computationsRecieved += jobSize;
            sum += buff[0];

            buff[0] = std::min(jobSize, computationsRemaining);
            if (computationsRemaining > 0) {
                computationsRemaining -= jobSize;
            }

            MPI_Send(buff, 1, MPI_INT, status.MPI_SOURCE, 1, comm);
        }

        printf("Closing workers...\n");

        for (int i = 0; i < numWorkers; i++) {
            int buff[1];
            buff[0] = 0;
            MPI_Send(buff, 1, MPI_INT, i, 1, comm);
        }

        printf("Estimating PI...\n");

        double pi = 4.0*((double)sum / (double)numComputations);
        double elapsedTime = MPI_Wtime() - startTime;

        std::cout << "\nEstimated value of PI: " << pi << "\n";
        std::cout << "Elapsed time: " << elapsedTime << " seconds\n\n";
    }
    else {
        printf("There are no worker processes. Halting execution...\n\n");
    }

    MPI_Finalize();
    return 0;
}