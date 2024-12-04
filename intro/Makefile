BINR := ./bin
CFLAGS := -Wall
.PHONY := all clean

all: ${BINR}/cpu ${BINR}/mem ${BINR}/io ${BINR}/threads

clean:
	rm -frv ${BINR}/*

${BINR}/cpu: cpu.c common.h
	gcc -o $@ $< $(CFLAGS)

${BINR}/mem: mem.c common.h
	gcc -o $@ $< $(CFLAGS)

${BINR}/io: io.c common.h
	gcc -o $@ $< $(CFLAGS)

${BINR}/threads: threads.c common.h common_threads.h
	gcc -o $@ $< $(CFLAGS)
