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

Number of keys 1000
Bloom
bits per key   8 best bits per bucket =   6, collision rate = 2.167 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   8, collision rate = 0.320 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  12, collision rate = 0.042 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.696 %
bits per key  12 best bits per bucket = 7, collision rate = 0.433 %
bits per key  16 best bits per bucket = 11, collision rate = 0.083 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.615 %
bits per key  12 best bits per bucket = 8, collision rate = 0.418 %
bits per key  16 best bits per bucket = 10, collision rate = 0.083 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.483 %
bits per key  12 best bits per bucket = 8, collision rate = 0.508 %
bits per key  16 best bits per bucket = 10, collision rate = 0.115 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.769 %
bits per key  12 best bits per bucket = 7, collision rate = 0.694 %
bits per key  16 best bits per bucket = 8, collision rate = 0.206 %

single word (64-bits)
bits per key   8 best bits per bucket = 4, collision rate = 3.355 %
bits per key  12 best bits per bucket = 5, collision rate = 1.043 %
bits per key  16 best bits per bucket = 8, collision rate = 0.417 %

==========

Number of keys 1000000
Bloom
bits per key   8 best bits per bucket =   5, collision rate = 2.166 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   9, collision rate = 0.312 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  10, collision rate = 0.044 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.294 %
bits per key  12 best bits per bucket = 7, collision rate = 0.413 %
bits per key  16 best bits per bucket = 9, collision rate = 0.080 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.329 %
bits per key  12 best bits per bucket = 7, collision rate = 0.410 %
bits per key  16 best bits per bucket = 10, collision rate = 0.086 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.478 %
bits per key  12 best bits per bucket = 7, collision rate = 0.523 %
bits per key  16 best bits per bucket = 9, collision rate = 0.121 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.773 %
bits per key  12 best bits per bucket = 7, collision rate = 0.696 %
bits per key  16 best bits per bucket = 7, collision rate = 0.218 %

single word (64-bits)
bits per key   8 best bits per bucket = 4, collision rate = 3.319 %
bits per key  12 best bits per bucket = 6, collision rate = 1.017 %
bits per key  16 best bits per bucket = 6, collision rate = 0.396 %

==========

Number of keys 10000000
Bloom
bits per key   8 best bits per bucket =   6, collision rate = 2.127 % [measured]
bits per key   8 best bits per bucket = 5.5, collision rate = 2.142 % [theory]
bits per key  12 best bits per bucket =   7, collision rate = 0.305 % [measured]
bits per key  12 best bits per bucket = 8.3, collision rate = 0.313 % [theory]
bits per key  16 best bits per bucket =  10, collision rate = 0.047 % [measured]
bits per key  16 best bits per bucket = 11.1, collision rate = 0.046 % [theory]

Two full cache lines (1024-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.308 %
bits per key  12 best bits per bucket = 9, collision rate = 0.405 %
bits per key  16 best bits per bucket = 8, collision rate = 0.071 %

Full cache line (512-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.295 %
bits per key  12 best bits per bucket = 7, collision rate = 0.403 %
bits per key  16 best bits per bucket = 11, collision rate = 0.071 %

half cache line  (256-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.472 %
bits per key  12 best bits per bucket = 7, collision rate = 0.497 %
bits per key  16 best bits per bucket = 10, collision rate = 0.122 %

quarter cache line  (128-bits)
bits per key   8 best bits per bucket = 5, collision rate = 2.823 %
bits per key  12 best bits per bucket = 8, collision rate = 0.692 %
bits per key  16 best bits per bucket = 7, collision rate = 0.214 %

single word (64-bits)
bits per key   8 best bits per bucket = 4, collision rate = 3.297 %
bits per key  12 best bits per bucket = 5, collision rate = 1.008 %
bits per key  16 best bits per bucket = 6, collision rate = 0.403 %

==========
```
