#include "blockmodel.h"
#include "bloommodel.h"
#include "laneblockmodel.h"
#include "softblockmodel.h"





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
  printf("soft:\n");
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
