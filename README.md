# BlockBloomSimulator
experiments with block Bloom filters (*not* useful code)


## Usage

```
make test
```

## Sample output

```
bits per key indicates storage (bits used per key): smaller is better
best bits per bucket indicates the number of bits to check per key: smaller is better
collision rate is the false negative rate: smaller is better
In the 'lane' model you have one bit set per lane.

Number of keys 1000
Bloom
bits per key   8 best bits per bucket =   5, collision rate = 2.170 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   8, collision rate = 0.320 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  12, collision rate = 0.042 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 5, collision rate = 3.365 %
bits per key  12 best bits per bucket = 7, collision rate = 0.484 %
bits per key  16 best bits per bucket = 10, collision rate = 0.080 %
bits per key   8 best lane size = 64 bits, collision rate = 16.356 %
bits per key  10 best lane size = 64 bits, collision rate = 5.198 %
bits per key  12 best lane size = 64 bits, collision rate = 1.622 %
bits per key  14 best lane size = 64 bits, collision rate = 0.480 %
bits per key  16 best lane size = 64 bits, collision rate = 0.163 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.625 %
bits per key  12 best bits per bucket = 7, collision rate = 0.445 %
bits per key  16 best bits per bucket = 9, collision rate = 0.083 %
bits per key   8 best lane size = 64 bits, collision rate = 3.366 %
bits per key  10 best lane size = 64 bits, collision rate = 1.211 %
bits per key  12 best lane size = 64 bits, collision rate = 0.467 %
bits per key  14 best lane size = 64 bits, collision rate = 0.195 %
bits per key  16 best lane size = 64 bits, collision rate = 0.097 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.574 %
bits per key  12 best bits per bucket = 6, collision rate = 0.544 %
bits per key  16 best bits per bucket = 8, collision rate = 0.133 %
bits per key   8 best lane size = 64 bits, collision rate = 2.724 %
bits per key  10 best lane size = 32 bits, collision rate = 1.226 %
bits per key  12 best lane size = 32 bits, collision rate = 0.582 %
bits per key  14 best lane size = 32 bits, collision rate = 0.279 %
bits per key  16 best lane size = 32 bits, collision rate = 0.127 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.838 %
bits per key  12 best bits per bucket = 6, collision rate = 0.674 %
bits per key  16 best bits per bucket = 8, collision rate = 0.202 %
bits per key   8 best lane size = 32 bits, collision rate = 2.993 %
bits per key  10 best lane size = 32 bits, collision rate = 1.573 %
bits per key  12 best lane size = 16 bits, collision rate = 0.782 %
bits per key  14 best lane size = 16 bits, collision rate = 0.409 %
bits per key  16 best lane size = 16 bits, collision rate = 0.227 %

single word (64-bits)
bits per key   8 best bits per bucket = 5, collision rate = 3.329 %
bits per key  12 best bits per bucket = 6, collision rate = 1.007 %
bits per key  16 best bits per bucket = 6, collision rate = 0.408 %

==========

Number of keys 1000000
Bloom
bits per key   8 best bits per bucket =   6, collision rate = 2.143 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   9, collision rate = 0.311 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  11, collision rate = 0.039 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 6, collision rate = 2.251 %
bits per key  12 best bits per bucket = 8, collision rate = 0.347 %
bits per key  16 best bits per bucket = 12, collision rate = 0.063 %
bits per key   8 best lane size = 64 bits, collision rate = 10.689 %
bits per key  10 best lane size = 64 bits, collision rate = 3.237 %
bits per key  12 best lane size = 64 bits, collision rate = 1.013 %
bits per key  14 best lane size = 64 bits, collision rate = 0.335 %
bits per key  16 best lane size = 64 bits, collision rate = 0.115 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.334 %
bits per key  12 best bits per bucket = 7, collision rate = 0.421 %
bits per key  16 best bits per bucket = 10, collision rate = 0.087 %
bits per key   8 best lane size = 64 bits, collision rate = 2.918 %
bits per key  10 best lane size = 64 bits, collision rate = 1.057 %
bits per key  12 best lane size = 64 bits, collision rate = 0.419 %
bits per key  14 best lane size = 64 bits, collision rate = 0.188 %
bits per key  16 best lane size = 64 bits, collision rate = 0.085 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.467 %
bits per key  12 best bits per bucket = 6, collision rate = 0.514 %
bits per key  16 best bits per bucket = 8, collision rate = 0.130 %
bits per key   8 best lane size = 64 bits, collision rate = 2.671 %
bits per key  10 best lane size = 32 bits, collision rate = 1.239 %
bits per key  12 best lane size = 32 bits, collision rate = 0.526 %
bits per key  14 best lane size = 32 bits, collision rate = 0.250 %
bits per key  16 best lane size = 32 bits, collision rate = 0.124 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.798 %
bits per key  12 best bits per bucket = 6, collision rate = 0.692 %
bits per key  16 best bits per bucket = 7, collision rate = 0.219 %
bits per key   8 best lane size = 32 bits, collision rate = 2.937 %
bits per key  10 best lane size = 32 bits, collision rate = 1.566 %
bits per key  12 best lane size = 16 bits, collision rate = 0.800 %
bits per key  14 best lane size = 16 bits, collision rate = 0.417 %
bits per key  16 best lane size = 16 bits, collision rate = 0.228 %

single word (64-bits)
bits per key   8 best bits per bucket = 4, collision rate = 3.372 %
bits per key  12 best bits per bucket = 6, collision rate = 1.022 %
bits per key  16 best bits per bucket = 6, collision rate = 0.400 %

==========

Number of keys 10000000
Bloom
bits per key   8 best bits per bucket =   5, collision rate = 2.077 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   9, collision rate = 0.297 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  10, collision rate = 0.039 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 6, collision rate = 2.160 %
bits per key  12 best bits per bucket = 8, collision rate = 0.322 %
bits per key  16 best bits per bucket = 11, collision rate = 0.054 %
bits per key   8 best lane size = 64 bits, collision rate = 10.664 %
bits per key  10 best lane size = 64 bits, collision rate = 3.207 %
bits per key  12 best lane size = 64 bits, collision rate = 0.993 %
bits per key  14 best lane size = 64 bits, collision rate = 0.341 %
bits per key  16 best lane size = 64 bits, collision rate = 0.114 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.268 %
bits per key  12 best bits per bucket = 8, collision rate = 0.387 %
bits per key  16 best bits per bucket = 10, collision rate = 0.079 %
bits per key   8 best lane size = 64 bits, collision rate = 2.999 %
bits per key  10 best lane size = 64 bits, collision rate = 1.098 %
bits per key  12 best lane size = 64 bits, collision rate = 0.439 %
bits per key  14 best lane size = 64 bits, collision rate = 0.190 %
bits per key  16 best lane size = 64 bits, collision rate = 0.095 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.502 %
bits per key  12 best bits per bucket = 7, collision rate = 0.499 %
bits per key  16 best bits per bucket = 9, collision rate = 0.117 %
bits per key   8 best lane size = 64 bits, collision rate = 2.649 %
bits per key  10 best lane size = 32 bits, collision rate = 1.256 %
bits per key  12 best lane size = 32 bits, collision rate = 0.511 %
bits per key  14 best lane size = 32 bits, collision rate = 0.272 %
bits per key  16 best lane size = 32 bits, collision rate = 0.140 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.773 %
bits per key  12 best bits per bucket = 6, collision rate = 0.658 %
bits per key  16 best bits per bucket = 7, collision rate = 0.194 %
bits per key   8 best lane size = 32 bits, collision rate = 2.968 %
bits per key  10 best lane size = 32 bits, collision rate = 1.599 %
bits per key  12 best lane size = 16 bits, collision rate = 0.783 %
bits per key  14 best lane size = 16 bits, collision rate = 0.413 %
bits per key  16 best lane size = 16 bits, collision rate = 0.247 %

single word (64-bits)
bits per key   8 best bits per bucket = 4, collision rate = 3.235 %
bits per key  12 best bits per bucket = 5, collision rate = 1.011 %
bits per key  16 best bits per bucket = 6, collision rate = 0.399 %

==========
```
