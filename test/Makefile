
CC = gcc
CCDIR = coverage
ONAME = libhlsparse
CCOBJDIR = $(CCDIR)/obj
CFLAGS = -I../bin -L../bin
LIBS = -lhlsparse -lcunit

.SECONDEXPANSION:
OBJ_SRC := $(patsubst %.c, %.o, $(filter-out tests.c, $(wildcard *.c)))

DEBUG ?= 0
COVERAGE ?= 0
PROFILING ?= 0

ifeq ($(COVERAGE), 1)
	CFLAGS += -fprofile-arcs -ftest-coverage -fprofile-dir=$(CCOBJDIR)
	DEBUG = 1
endif

ifeq ($(PROFILING), 1)
	CFLAGS += -pg
	DEBUG = 1
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
else
	CFLAGS += -O2
endif

all: tests

.PHONY: tests check clean

%.o: %.c
	$(CC) -o $@ $< tests.c $(CFLAGS) $(LIBS)

tests: $(OBJ_SRC)

check: tests
	./test-runner.sh

clean:
	rm -r -f test/*.o.dSYM
	find . -type f -name '*.o' -exec rm {} \;
	find . -type f -name '*.o.dSYM' -exec rm {} \;
	find . -type f -name '*.o.gcno' -exec rm {} \;
	find . -type f -name '*.o.gcda' -exec rm {} \;
	find . -type f -name 'gmon.out' -exec rm {} \;
