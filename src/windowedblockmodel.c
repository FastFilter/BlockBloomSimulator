#include "windowedblockmodel.h"

#include "blockmodel.h"




void compute(size_t HowManyKeys) {
  bool verbose = false;
 /* printf("Number of keys %zu \n", HowManyKeys);
  printf(" cache line (512-bits) \n");
  assessBlockSize(HowManyKeys, 512, verbose);

  for(size_t window = 8; window <= 512; window*=2) {
    assessWindowedBlockSize(HowManyKeys, 512, window, verbose);
  }
  printf("\n");*/

  printf("quarter cache line (128-bits) \n");
  assessBlockSize(HowManyKeys, 128, verbose);

  for(size_t window = 8; window <= 128; window*=2) {
    assessWindowedBlockSize(HowManyKeys, 128, window, verbose);
  }  

  printf("\n");
}

int main() {
  printf("bits per key indicates storage (bits used per key): smaller is "
         "better\n");
  printf("best bits per bucket indicates the number of bits to check per key: "
         "smaller is better\n");
  printf("collision rate is the false negative rate: smaller is better\n");
  printf("In the 'lane' model you have one bit set per lane.\n");
  printf("\n");


  compute(10000000);
  printf("==========\n\n");

  return EXIT_SUCCESS;
}
