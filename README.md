# ParallelPi

Several implementation to estimate the value of pi. Two of which use MPI to parallelise their solutions. A sample size can be set to determine the runtime and accuracy of the estimated pi value.

Solutions:

  - Serial_Pi: Iteratively solves for pi, but can be quite slow with larger sample sizes.
  - Simple_Parallel_Pi: Tasks every thread with with computing a portion of the sample whilst the initial task collects and computes the final result.
  - Distributed_Parallel_Pi: Master thread initiallizes Child processes which will compute a set portion of the sample size. A job size can be determined where Child processes will send and request a new job after a specified number of computations. Master thread then compiles, computes, and returns all final results.
