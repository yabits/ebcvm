CFLAGS=-Wall -std=c11 -I. -g -O0
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TESTS=$(wildcard test/*.c)
TOBJS=$(TESTS:.c=.o)
TARGET=ebcvm

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(TARGET).h

test: $(OBJS) $(TOBJS)
	./test.sh

clean:
	rm -f $(TARGET) *.o test/*.{o,exe}

.PHONY: test clean
