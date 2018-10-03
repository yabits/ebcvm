CFLAGS=-Wall -std=c11 -g -O0
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TARGET=ebcvm

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(TARGET).h

clean:
	rm -f $(TARGET) *.o

.PHONY: clean
