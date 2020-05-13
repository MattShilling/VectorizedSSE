#include "test_rig.h"

#include <algorithm>
#include <cmath>
#include <omp.h>
#include <stdio.h>

void TestRig::Init(int num_threads) {
  if (mp_good_) {
    // Set the number of threads we want to use.
    // omp_set_num_threads(num_threads);
    // fprintf(stderr, "Using %d threads\n", num_threads);
    // We aren't using OMP multithreading for this..yet.

    // Initialize test memory.
    test_init_(mem_);

    // Clear our records.
    perf_.clear();
  } else {
    printf("Init error: OpenMP not supported!\n");
  }
}

void TestRig::Run(double ops) {
  if (mp_good_) {
    // Get the starting time for our test.
    double start = omp_get_wtime();
    // Run. That. Test!
    test_run_(mem_);
    // Get the ending time for our test.
    double stop = omp_get_wtime();
    // Calculate the multiplications per second we accomplished.
    double time_per_op = ops / (stop - start);
    // Convert into megamults.
    double perf = time_per_op / 1000000.00;
    // Add results to our records.
    perf_.push_back(perf);
  } else {
    printf("Run error: OpenMP not supported!\n");
  }
}

double TestRig::MaxPerf() {
  return *std::max_element(perf_.begin(), perf_.end());
}

double TestRig::MinPerf() {
  return *std::min_element(perf_.begin(), perf_.end());
}

bool TestRig::CheckOpenMP() {
#ifndef _OPENMP
  fprintf(stderr, "OpenMP is not supported!\n");
  return false;
#endif
  return true;
}