# cpu-api/Makefile

.PHONY: all clean

CC     := gcc
STD    := -std=gnu17
WAR    := -Wall
OPT    := -O3 -march=native -fdata-sections -ffunction-sections
CFLAGS := ${WAR}

SOURCES  = $(wildcard *.c)
BINARIES = $(subst .c,.out,$(SOURCES))

all: $(BINARIES)

$(BINARIES): %.out: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fv *.out p4_output.txt
