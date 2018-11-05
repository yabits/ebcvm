CFLAGS=-Wall -std=c11 -Iinclude -g -O0
HDRS=$(wildcard include/*.h)
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TESTS=$(wildcard test/*.c)
TOBJS=$(TESTS:.c=.o)
TARGET=ebcvm

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(HDRS)

test: $(OBJS) $(TOBJS)
	./test.sh

clean:
	rm -f $(TARGET) *.o test/*.o test/*.exe

.PHONY: test clean
