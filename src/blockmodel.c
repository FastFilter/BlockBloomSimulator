
#include <bitset.h>
#include <math.h>

struct model_s {
  size_t BitsPerKeyPerBlock;
  size_t BlockSizeInBits;
  size_t NumberOfBlocks;
};

typedef struct model_s model_t;

struct keymap_s {
  size_t blockindex;
  size_t *bitindexes;
};

typedef struct keymap_s keymap_t;

typedef struct pcg_state_setseq_64 { // Internals are *Private*.
  uint64_t state;                    // RNG state.  All values are possible.
  uint64_t inc;                      // Controls which RNG sequence (stream) is
                                     // selected. Must *always* be odd.
} pcg32_random_t;

pcg32_random_t gseed = {.state = 131232, .inc = 3333};

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

size_t random_integer(size_t range) {
  if (range > 0xFFFFFFFF)
    abort();
  return (((uint64_t)pcg32_random_r(&gseed)) * range) >> 32;
}

typedef struct  {
  double collision_rate;
  double density;
} sim_stats_t;

sim_stats_t simulate(size_t HowManyKeys, model_t model, size_t howmanytests,
                bitset_t *b) {
  bitset_resize(b, model.BlockSizeInBits * model.NumberOfBlocks, true);
  bitset_clear(b);

  for (size_t i = 0; i < HowManyKeys; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(model.BlockSizeInBits);
      bitset_set(b, blockindex * model.BlockSizeInBits + bi);
    }
  }
  size_t matches = 0;
  for (size_t i = 0; i < howmanytests; i++) {
    size_t blockindex = random_integer(model.NumberOfBlocks);
    bool is_matched = true;
    for (size_t j = 0; j < model.BitsPerKeyPerBlock; j++) {
      size_t bi = random_integer(model.BlockSizeInBits);
      is_matched =
          is_matched && bitset_get(b, blockindex * model.BlockSizeInBits + bi);
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
size_t findBestBitsPerKeyPerBlock(size_t HowManyKeys, size_t bitbudget,
                                  size_t BlockSizeInBits,
                                  sim_stats_t *beststats, bool verbose) {
  model_t m;
  m.NumberOfBlocks = bitbudget / BlockSizeInBits;
  m.BlockSizeInBits = BlockSizeInBits;
  size_t howmanytests = 100000;
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
      sim_stats_t ts = simulate(HowManyKeys, m, howmanytests, b);
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


/***
 * We assess the conventional block Bloom filter case (for comparison).
 * 
 * Given a number of keys and a blokc size, find the smallest collision rate for
 * various bit budget.
 */
void assessBlockSize(size_t HowManyKeys, size_t BlockSizeInBits, bool verbose) {
  for (size_t bitsperkey = 8; bitsperkey <= 16; bitsperkey += 4) {
    size_t bitbudget = bitsperkey * HowManyKeys;
    sim_stats_t beststats;
    size_t bestbitsperbucket = findBestBitsPerKeyPerBlock(
        HowManyKeys, bitbudget, BlockSizeInBits, &beststats, verbose);
    double density = bestbitsperbucket * 1.0 / bitsperkey;

    printf("bits per key %3zu best bits per bucket = %zu, max density = %0.0f %%, density = %0.0f %%, collision rate = "
           "%0.3f %%\n",
           bitsperkey, bestbitsperbucket,density * 100,   beststats.density * 100, beststats.collision_rate * 100);
  }
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
    printf("bits per key %3zu best lane size = %zu bits, max density = %0.0f %%, density = %0.0f %%, collision rate = "
           "%0.3f %% (x %.1f worse than Bloom)  // collision rate at 8 bits is "
           "%0.3f %% \n",
           bitsperkey, bestlane, density * 100, beststats.density * 100, beststats.collision_rate * 100,
           beststats.collision_rate / bloombestcollisionrate, colrate8 * 100);
  }
}

void compute(size_t HowManyKeys) {
  bool verbose = false;
  printf("Number of keys %zu \n", HowManyKeys);
  printf("Bloom \n");
  assessBloomCase(HowManyKeys, verbose);
  printf("\n");
  printf("Two full cache lines (1024-bits) \n");
  assessBlockSize(HowManyKeys, 1024, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 1024, verbose);
  printf("\n");
  printf("Full cache line (512-bits) \n");
  assessBlockSize(HowManyKeys, 512, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 512, verbose);
  printf("\n");
  printf("half cache line  (256-bits) \n");
  assessBlockSize(HowManyKeys, 256, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 256, verbose);

  printf("\n");
  printf("quarter cache line  (128-bits) \n");
  assessBlockSize(HowManyKeys, 128, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 128, verbose);
  printf("\n");
  printf("single word (64-bits) \n");
  assessBlockSize(HowManyKeys, 64, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 64, verbose);
  printf("\n");
  printf("single word (32-bits) \n");
  assessBlockSize(HowManyKeys, 32, verbose);
  printf("lanes:\n");
  assessBlockSizeLanes(HowManyKeys, 32, verbose);
}

int main() {
  printf("bits per key indicates storage (bits used per key): smaller is "
         "better\n");
  printf("best bits per bucket indicates the number of bits to check per key: "
         "smaller is better\n");
  printf("collision rate is the false negative rate: smaller is better\n");
  printf("In the 'lane' model you have one bit set per lane.\n");
  printf("\n");

  compute(1000);
  printf("==========\n\n");

  compute(1000000);
  printf("==========\n\n");

  compute(10000000);
  printf("==========\n\n");

  return EXIT_SUCCESS;
}
