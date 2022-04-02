#include <math.h>
#include <iostream>
#include <string.h>
#include <random>
#include <time.h>
#include <stdlib.h>

int main()
{
    std::cout << "Num points to compute: ";

    int npoints = 100000;
    std::cin >> npoints;

    clock_t tStart = clock();

    int circle_count = 0;

    for (int i = 0; i < npoints; i++) {
        double xcord = ((double)rand() / (double)RAND_MAX);
        double ycord = ((double)rand() / (double)RAND_MAX);

        if (sqrt(xcord*xcord + ycord*ycord) <= 1) {
            circle_count++;
        }
    }

    double PI = (4.0 * ((double)circle_count / (double)npoints));
    std::cout << "\nEstimated value of Pi: " << PI << "\n";

    double executionTime = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    std::cout << "Execution Time: " << executionTime << " seconds.\n";
}
