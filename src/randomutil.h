#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

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

uint32_t random_word64() {
  return ((uint64_t) pcg32_random_r(&gseed) << 32) | pcg32_random_r(&gseed);
}

size_t random_integer(size_t range) {
  if (range > 0xFFFFFFFF)
    abort();
  return (((uint64_t)pcg32_random_r(&gseed)) * range) >> 32;
}

