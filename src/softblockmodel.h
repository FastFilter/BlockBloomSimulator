#pragma once

#include "bitset.h"
#include "randomutil.h"
#include "simstat.h"
#include "blockmodel.h"

struct softmodel_s {
  size_t FoldCount;// 0= 50%, 1 = 25%, 3 = ... and so forth
  size_t BlockSizeInBits;
  size_t NumberOfBlocks;
};

typedef struct softmodel_s softmodel_t;

sim_stats_t simulate_soft(size_t HowManyKeys, softmodel_t model, size_t howmanytests,
                     bitset_t *b) {
  if((model.BlockSizeInBits % 64)!=0) {
      printf("block size should be divisible by 64 bits");
      abort();
  }
  size_t wordsperblock = model.BlockSizeInBits / 64;
  bitset_resize(b, model.BlockSizeInBits * model.NumberOfBlocks, true);
  bitset_clear(b);
  for (size_t i = 0; i < HowManyKeys; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    for(size_t j = 0; j < wordsperblock; j++) {
        uint64_t w = random_word64();
        for(size_t k = 0; k < model.FoldCount; k++) {
            w &= random_word64();
        }
        w = w | bitset_get_word(b, blockindex * wordsperblock + j);
        bitset_set_word(b, w, blockindex * wordsperblock + j);
    }
  }
  size_t matches = 0;
  for (size_t i = 0; i < howmanytests; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    bool is_matched = true;
    for(size_t j = 0; j < wordsperblock; j++) {
        uint64_t w = random_word64();
        for(size_t k = 0; k < model.FoldCount; k++) {
            w &= random_word64();;
        }
        uint64_t tw = bitset_get_word(b, blockindex * wordsperblock + j);
        if( (tw & w) != w) {
            is_matched = false; 
        }
    }
    if (is_matched)
      matches++;
  }
  sim_stats_t answer;
  answer.collision_rate = matches * 1.0 / howmanytests;
  answer.density = bitset_count(b) * 1.0 /  (model.BlockSizeInBits * model.NumberOfBlocks) ;
  return answer;
}

/// What we want to do is this: given a number of keys and a storage budget,
/// what is the best we can do?
size_t findBestFold(size_t HowManyKeys, size_t bitbudget,
                                  size_t BlockSizeInBits,
                                  sim_stats_t *beststats, bool verbose) {
  softmodel_t m;
  m.NumberOfBlocks = bitbudget / BlockSizeInBits;
  m.BlockSizeInBits = BlockSizeInBits;
  size_t howmanytests = 100000;
  bitset_t *b = bitset_create_with_capacity(bitbudget);
  beststats->collision_rate = 1.0;
  size_t BestFold = 0;
  for (size_t fold = 0; fold <= 16; fold++) {
    m.FoldCount = fold;
    // we take the average of several attempts
    //double collisionrate = 0;
    sim_stats_t s;
    s.collision_rate = 0;
    s.density = 0;
    size_t attempts = HowManyKeys < 10000000 ? 5 : 1;
    for (int i = 0; i < attempts; i++) {
      sim_stats_t ts = simulate_soft(HowManyKeys, m, howmanytests, b);
      s.density += ts.density;
      s.collision_rate += ts.collision_rate;
    }
    s.collision_rate /= attempts;
    s.density /= attempts;

    if (s.collision_rate * 1.01 <
        beststats->collision_rate) { // has to be at least one percent better
      beststats->collision_rate = s.collision_rate;
      beststats->density = s.density; 
      BestFold = fold;
    }
  }
  bitset_free(b);
  return BestFold;
}

sim_stats_t findSoftCollisionRateAndDensity(size_t HowManyKeys, size_t bitbudget,
                             size_t BlockSizeInBits, size_t fold,
                             bool verbose) {
  softmodel_t m;
  m.NumberOfBlocks = bitbudget / BlockSizeInBits;
  m.BlockSizeInBits = BlockSizeInBits;
  size_t howmanytests = 100000;
  bitset_t *b = bitset_create_with_capacity(bitbudget);
  m.FoldCount = fold;
  // we take the averge of several attempts
  sim_stats_t s; 
  s.collision_rate = 0;
  s.density = 0;
  size_t attempts = HowManyKeys < 10000000 ? 5 : 1;
  for (int i = 0; i < attempts; i++) {
    sim_stats_t ts = simulate_soft(HowManyKeys, m, howmanytests, b);
    s.density += ts.density;
    s.collision_rate += ts.collision_rate;
  }
  s.collision_rate /= attempts;
  s.density /= attempts;
  bitset_free(b);
  return s;
}

void assessFold(size_t HowManyKeys, size_t BlockSizeInBits,
                          bool verbose) {
  for (size_t bitsperkey = 8; bitsperkey <= 10; bitsperkey += 1) {
    size_t bitbudget = bitsperkey * HowManyKeys;
    sim_stats_t beststats;
    size_t bestfold = findBestFold(HowManyKeys, bitbudget, BlockSizeInBits,
                                       &beststats, verbose);
    printf("bits per key %3zu best fold = %zu,  density = %0.0f %%, collision rate = "
           "%0.3f %% \n",
           bitsperkey, bestfold, beststats.density * 100, beststats.collision_rate * 100);
  }
}