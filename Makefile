CFLAGS=-Wall -std=c11 -g -O0 -Iinclude
HDRS=$(wildcard include/*.h)
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TESTS=$(wildcard test/*.c)
TOBJS=$(TESTS:.c=.o)
TOOLS=$(wildcard tools/*.c)
TOOLSOBJS=$(TOOLS:.c=.o)
TARGET=ebcvm

all: $(TARGET) tools

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(HDRS)

tools: $(OBJS) $(TOOLSOBJS)
	$(CC) -o tools/ebcdisas $(filter-out main.o, $(OBJS)) tools/ebcdisas.o
	$(CC) -o tools/fnv1 tools/fnv1.o

test: $(OBJS) $(TOBJS)
	./test.sh

clean:
	rm -f $(TARGET) ebcdisas fnv1 tools/*.o *.o test/*.o test/*.exe

.PHONY: test clean
