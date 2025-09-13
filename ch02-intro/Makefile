# ostep-code/intro/Makefile

.PHONY  : all clean

CC      = gcc
CFLAGS := -Wall -Werror

HEADERS = common.h common_threads.h
SOURCES = cpu.c mem.c io.c threads.v0.c threads.v1.c
BIN_1_3 = $(subst .c,.out,$(wordlist 1,3,$(SOURCES)))
BIN_4_5 = $(subst .c,.out,$(wordlist 4,5,$(SOURCES)))

all: ${BIN_1_3} ${BIN_4_5}

$(BIN_1_3): %.out: %.c $(word 1, $(HEADERS))
	$(CC) $(CFLAGS) $< -o $@

$(BIN_4_5): %.out: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fv *.out ./tmp/*
