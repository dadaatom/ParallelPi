#include <math.h>
#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	MPI_Comm comm;
	int processID;
	int numChildren;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numChildren);
	MPI_Comm_rank(MPI_COMM_WORLD, &processID);
	MPI_Comm_get_parent(&comm);

	printf("     Worker %d created!\n", processID);

	srand(time(NULL)*processID);
	while (true) {
		MPI_Status status;
		int buff[1];

		MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, 1, comm, &status);

		int num = buff[0];

		if (num <= 0) {
			break;
		}

		int count = 0;
		for (int i = 0; i < num; i++) {
			double x = ((double)rand() / (double)RAND_MAX);
			double y = ((double)rand() / (double)RAND_MAX);

			if (sqrt(x * x + y * y) <= 1) {
				count++;
			}
		}

		buff[0] = count;
		MPI_Send(buff, 1, MPI_INT, status.MPI_SOURCE, 0, comm);
	}

	printf("     Worker %d finalizing...\n", processID);

	MPI_Finalize();
	return 0;
}