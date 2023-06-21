all: blockmodel simpleblockmodel windowedblockmodel

test: blockmodel
	./blockmodel

bitset.o: src/bitset.c src/bitset.h  src/portability.h
	cc -O3 -c src/bitset.c -Wall -Isrc

blockmodel: bitset.o src/bitset.h src/blockmodel.c  src/bloommodel.h     src/portability.h    src/softblockmodel.h  src/simstat.h src/blockmodel.h     src/laneblockmodel.h src/randomutil.h
	cc -O3 -o blockmodel src/blockmodel.c bitset.o -Wall -Isrc -lm


windowedblockmodel: bitset.o src/bitset.h src/windowedblockmodel.c  src/windowedblockmodel.h     src/portability.h    src/randomutil.h
	cc -O3 -o windowedblockmodel src/windowedblockmodel.c bitset.o -Wall -Isrc -lm


simpleblockmodel: bitset.o src/bitset.h src/simpleblockmodel.c  src/bloommodel.h     src/portability.h    src/softblockmodel.h  src/simstat.h src/blockmodel.h     src/laneblockmodel.h src/randomutil.h
	cc -O3 -o simpleblockmodel src/simpleblockmodel.c bitset.o -Wall -Isrc -lm

clean:
	rm -f blockmodel bitset.o simpleblockmodel
