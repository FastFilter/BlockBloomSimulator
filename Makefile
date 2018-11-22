all: blockmodel

test: blockmodel
	./blockmodel

bitset.o: src/bitset.c src/bitset.h  src/portability.h
	cc -O3 -c src/bitset.c -Wall -Isrc

blockmodel: bitset.o src/bitset.h src/blockmodel.c 
	cc -O3 -o blockmodel src/blockmodel.c bitset.o -Wall -Isrc


clean:
	rm -f blockmodel bitset.o