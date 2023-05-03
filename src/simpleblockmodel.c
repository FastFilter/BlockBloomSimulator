#include "blockmodel.h"
#include "bloommodel.h"
#include "laneblockmodel.h"
#include "softblockmodel.h"





void compute(size_t HowManyKeys) {
  bool verbose = false;
  printf("Number of keys %zu \n", HowManyKeys);
  printf("Full cache line (512-bits) \n");
  assessBlockSize(HowManyKeys, 512, verbose);
}

int main() {
  printf("bits per key indicates storage (bits used per key): smaller is "
         "better\n");
  printf("best bits per bucket indicates the number of bits to check per key: "
         "smaller is better\n");
  printf("collision rate is the false negative rate: smaller is better\n");
  printf("In the 'lane' model you have one bit set per lane.\n");
  printf("\n");
  for(size_t i = 100; i < 1000000000; i*=10) {
    compute(i);
  }

  return EXIT_SUCCESS;
}
