#pragma once

#include "bitset.h"
#include "randomutil.h"
#include "simstat.h"
#include "blockmodel.h"

// this is the lane-based block Bloom filter scenario
sim_stats_t simulate_lane(size_t HowManyKeys, model_t model, size_t howmanytests,
                     bitset_t *b) {
  bitset_resize(b, model.BlockSizeInBits * model.NumberOfBlocks, true);
  bitset_clear(b);
  size_t lane_size = model.BlockSizeInBits / model.BitsPerKeyPerBlock;

  for (size_t i = 0; i < HowManyKeys; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(lane_size);
      bitset_set(b, blockindex * model.BlockSizeInBits + j * lane_size + bi);
    }
  }
  size_t matches = 0;
  for (size_t i = 0; i < howmanytests; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    bool is_matched = true;
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(lane_size);
      is_matched =
          is_matched && bitset_get(b, blockindex * model.BlockSizeInBits +
                                          j * lane_size + bi);
    }
    if (is_matched)
      matches++;
  }
  sim_stats_t answer;
  answer.collision_rate = matches * 1.0 / howmanytests;
  answer.density = bitset_count(b) * 1.0 /  (model.BlockSizeInBits * model.NumberOfBlocks) ;
  return answer;
}

size_t findBestLaneSize(size_t HowManyKeys, size_t bitbudget,
                        size_t BlockSizeInBits, sim_stats_t *beststats,
                        bool verbose) {
  model_t m;
  m.NumberOfBlocks = bitbudget / BlockSizeInBits;
  m.BlockSizeInBits = BlockSizeInBits;
  size_t howmanytests = 100000;
  bitset_t *b = bitset_create_with_capacity(bitbudget);
  beststats->collision_rate = 1.0;
  size_t BestLanes = 0;
  size_t valid_lanes[] = {8, 16, 32, 64};
  for (size_t laneindex = 0;
       laneindex < sizeof(valid_lanes) / sizeof(valid_lanes[0]); laneindex++) {
    size_t lanesize = valid_lanes[laneindex];
    if (lanesize > m.BlockSizeInBits)
      continue;
    m.BitsPerKeyPerBlock = m.BlockSizeInBits / lanesize;
    // we take the averge of several attempts
    sim_stats_t s;
    s.collision_rate = 0;
    s.density = 0;
    size_t attempts = HowManyKeys < 10000000 ? 5 : 1;
    for (int i = 0; i < attempts; i++) {
      sim_stats_t ts = simulate_lane(HowManyKeys, m, howmanytests, b);
      s.density += ts.density;
      s.collision_rate += ts.collision_rate;

    }
    s.collision_rate /= attempts;
    s.density /= attempts;
    if (s.collision_rate * 1.01 <
        beststats->collision_rate) { // has to be at least one percent better
      beststats->collision_rate = s.collision_rate;
      beststats->density = s.density;
      BestLanes = lanesize;
    }
  }
  bitset_free(b);
  return BestLanes;
}

sim_stats_t findLaneCollisionRateAndDensity(size_t HowManyKeys, size_t bitbudget,
                             size_t BlockSizeInBits, size_t lanesize,
                             bool verbose) {
  model_t m;
  m.NumberOfBlocks = bitbudget / BlockSizeInBits;
  m.BlockSizeInBits = BlockSizeInBits;
  size_t howmanytests = 100000;
  bitset_t *b = bitset_create_with_capacity(bitbudget);
  m.BitsPerKeyPerBlock = m.BlockSizeInBits / lanesize;
  // we take the averge of several attempts
  sim_stats_t s; 
  s.collision_rate = 0;
  s.density = 0;
  size_t attempts = HowManyKeys < 10000000 ? 5 : 1;
  for (int i = 0; i < attempts; i++) {
    sim_stats_t ts = simulate_lane(HowManyKeys, m, howmanytests, b);
    s.density += ts.density;
    s.collision_rate += ts.collision_rate;
  }
  s.collision_rate /= attempts;
  s.density /= attempts;
  bitset_free(b);
  return s;
}


/***
 * We assess the lane-based block Bloom filter case.
 * 
 * Given a number of keys and a blokc size, find the smallest collision rate for
 * various bit budget.
 */
void assessBlockSizeLanes(size_t HowManyKeys, size_t BlockSizeInBits,
                          bool verbose) {
  for (size_t bitsperkey = 8; bitsperkey <= 16; bitsperkey += 1) {
    size_t bitbudget = bitsperkey * HowManyKeys;
    sim_stats_t beststats;
    size_t bestlane = findBestLaneSize(HowManyKeys, bitbudget, BlockSizeInBits,
                                       &beststats, verbose);
    double colrate8 = findLaneCollisionRateAndDensity(HowManyKeys, bitbudget,
                                            BlockSizeInBits, 8, verbose).collision_rate;
    // for each key, we set BlockSizeInBits/bestlane bits to one
    double density = ( BlockSizeInBits / bestlane ) * 1.0 / bitsperkey;

    double bloombestbitsperbucket = bitsperkey * log(2);
    double bloombestcollisionrate = pow(2, -bloombestbitsperbucket);

    double theoretical_best_bits = log(1.0/beststats.collision_rate) / log(2.0);
    double inefficiency = (bitsperkey/theoretical_best_bits - 1.0) * 100.0;
    printf("bits per key %3zu best lane size = %zu bits, max density = %0.0f %%, density = %0.0f %%, collision rate = "
           "%0.3f %% (x %.2f worse than Bloom), inefficiency = %0.1f %%  // collision rate at 8 bits is "
           "%0.3f %% \n",
           bitsperkey, bestlane, density * 100, beststats.density * 100, beststats.collision_rate * 100,
           beststats.collision_rate / bloombestcollisionrate,  inefficiency, colrate8 * 100);
  }
}