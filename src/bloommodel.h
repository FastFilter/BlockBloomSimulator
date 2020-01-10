#pragma once

#include <math.h>
#include "blockmodel.h"
#include "randomutil.h"
#include "simstat.h"


void describeBloom(size_t bitsperkey) {
  double bestbitsperbucket = bitsperkey * log(2);
  double density = bestbitsperbucket * 1.0 / bitsperkey;

  double bestcollisionrate = pow(2, -bestbitsperbucket);
  printf("bits per key %3zu best bits per bucket = %4.1f, best density = %0.0f %%, collision rate = "
         "%0.3f %% [theory]\n",
         bitsperkey, bestbitsperbucket, density * 100, bestcollisionrate * 100);
}


/***
 * We assess the conventional Bloom filter case (for comparison).
 * 
 * Given a number of keys, find the smallest collision rate for
 * various bit budget.
 */
void assessBloomCase(size_t HowManyKeys, bool verbose) {
  for (size_t bitsperkey = 8; bitsperkey <= 16; bitsperkey += 1) {
    size_t bitbudget = bitsperkey * HowManyKeys;
    sim_stats_t beststats;
    size_t bestbitsperbucket = findBestBitsPerKeyPerBlock(
        HowManyKeys, bitbudget, bitbudget, &beststats, verbose);
    double density = bestbitsperbucket * 1.0 / bitsperkey;

    printf("bits per key %3zu best bits per bucket = %4.1f, max density = %0.0f %%, density = %0.0f %%, collision rate = "
           "%0.3f %% [measured]\n",
           bitsperkey, (float)bestbitsperbucket, density * 100, beststats.density * 100, beststats.collision_rate * 100);
    describeBloom(bitsperkey);
  }
}
