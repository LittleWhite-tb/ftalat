CC=gcc
CFLAGS=-O3 -g -march=native -Wall -Wextra -std=gnu89
LDFLAGS=

.PHONY: all trace doc clean

all:
	$(CC) $(CFLAGS) $(LDFLAGS) main.c loop.c CoreRelation.c FreqGetter.c FreqSetter.c measure.c utils.c ConfInterval.c -o ftalat -lm -pthread

trace:
	$(CC) $(CFLAGS) $(LDFLAGS) -D_DUMP main.c loop.c dumpResults.c CoreRelation.c FreqGetter.c FreqSetter.c measure.c utils.c ConfInterval.c -o ftalat -lm -pthread

doc:
	doxygen ./ftalat.doxy

clean:
	rm -f ./ftalat
