#pragma once


#include <math.h>
#include "bitset.h"
#include "randomutil.h"
#include "simstat.h"

struct windowed_model_s {
  size_t BitsPerKeyPerBlock;
  size_t TotalBits;
  size_t BlockSizeInBits;
  size_t WindowSizeInBits;

};

typedef struct windowed_model_s windowed_model_t;


size_t NumberOfBlocks(const windowed_model_t * model) {
  return (model->TotalBits - model->BlockSizeInBits) / model->WindowSizeInBits + 1;
}

// this is the pure block Bloom filter scenario
sim_stats_t simulateWindow(size_t HowManyKeys, windowed_model_t model, size_t howmanytests,
                bitset_t *b) {
  bitset_resize(b, model.TotalBits, true);
  bitset_clear(b);
  for (size_t i = 0; i < HowManyKeys; i++) {
    size_t blockindex = random_integer(NumberOfBlocks(&model));
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(model.BlockSizeInBits);
      bitset_set(b, blockindex * model.WindowSizeInBits + bi);
    }
  }
  size_t matches = 0;
  for (size_t i = 0; i < howmanytests; i++) {
    size_t blockindex = random_integer(NumberOfBlocks(&model));
    bool is_matched = true;
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(model.BlockSizeInBits);
      is_matched =
          is_matched && bitset_get(b, blockindex * model.WindowSizeInBits + bi);
    }
    if (is_matched)
      matches++;
  }
  sim_stats_t answer;
  answer.collision_rate = matches * 1.0 / howmanytests;
  answer.density = bitset_count(b) * 1.0 / model.TotalBits;
  return answer;
}

/// What we want to do is this: given a number of keys and a storage budget,
/// what is the best we can do?
size_t findBestBitsPerKeyPerBlockWindow(size_t HowManyKeys, size_t bitbudget,
                                  size_t BlockSizeInBits, size_t WindowSizeInBits,
                                  sim_stats_t *beststats, bool verbose) {
  windowed_model_t m;
  m.TotalBits = bitbudget;
  m.BlockSizeInBits = BlockSizeInBits;
  m.WindowSizeInBits = WindowSizeInBits;
  size_t howmanytests = 50000;
  bitset_t *b = bitset_create_with_capacity(bitbudget);
  beststats->collision_rate = 1.0;
  size_t BestBitsPerKeyPerBlock = 0;
  for (size_t BitsPerKeyPerBlock = 1;
       (BitsPerKeyPerBlock <= 16) ||
       (BestBitsPerKeyPerBlock == BitsPerKeyPerBlock);
       BitsPerKeyPerBlock++) {
    m.BitsPerKeyPerBlock = BitsPerKeyPerBlock;
    // we take the average of several attempts
    //double collisionrate = 0;
    sim_stats_t s;
    s.collision_rate = 0;
    s.density = 0;
    size_t attempts = HowManyKeys < 10000000 ? 5 : 1;
    for (int i = 0; i < attempts; i++) {
      sim_stats_t ts = simulateWindow(HowManyKeys, m, howmanytests, b);
      s.density += ts.density;
      s.collision_rate += ts.collision_rate;
    }
    s.collision_rate /= attempts;
    s.density /= attempts;

    if (s.collision_rate * 1.01 <
        beststats->collision_rate) { // has to be at least one percent better
      beststats->collision_rate = s.collision_rate;
      beststats->density = s.density; 
      BestBitsPerKeyPerBlock = BitsPerKeyPerBlock;
    }
  }
  bitset_free(b);
  return BestBitsPerKeyPerBlock;
}


/***
 * We assess the windowed block Bloom filter case.
 * 
 * Given a number of keys and a block size, find the smallest collision rate for
 * various bit budget.
 */
void assessWindowedBlockSize(size_t HowManyKeys, size_t BlockSizeInBits, size_t WindowSizeInBits, bool verbose) {
  for (size_t bitsperkey = 8; bitsperkey <= 16; bitsperkey += 4) {
    size_t bitbudget = bitsperkey * HowManyKeys;
    sim_stats_t beststats;
    size_t bestbitsperbucket = findBestBitsPerKeyPerBlockWindow(
        HowManyKeys, bitbudget, BlockSizeInBits, WindowSizeInBits, &beststats, verbose);
    double density = bestbitsperbucket * 1.0 / bitsperkey;

    double theoretical_best_bits = log(1.0/beststats.collision_rate) / log(2.0);
    double inefficiency = (bitsperkey/theoretical_best_bits - 1.0) * 100.0;
    

    printf("bits per key %3zu WindowSizeInBits %3zu best bits per bucket = %zu, max density = %0.0f %%, density = %0.0f %%, collision rate = "
           "%0.3f %%, inefficiency = %0.1f %% \n",
           bitsperkey, WindowSizeInBits, bestbitsperbucket, density * 100,   beststats.density * 100, beststats.collision_rate * 100, inefficiency);
  }
}