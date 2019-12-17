# SPDX-License-Identifier: MIT

CFLAGS := -Wall -std=c11 -g -O0 -Iinclude

HDRS := \
  include/ebcvm.h \
  include/efi.h \
  include/pe.h \

OBJS := \
  src/debug.o \
  src/decode.o \
  src/disas.o \
  src/efi.o \
  src/exec.o \
  src/load.o \
  src/mem.o \
  src/util.o \
  src/vm.o \

TOOLS := \
  tools/ebcvm \
  tools/disas \
  tools/fnv1 \

TEST_OBJS := $(patsubst %.c,%.o,$(wildcard test/*.c))
TESTS := $(TEST_OBJS:.o=.exe)

all: $(TOOLS)
	@cp tools/ebcvm ebcvm

$(TOOLS): libebcvm.a

$(TESTS): test/%.exe: test/%.o libebcvm.a
	$(CC) $(CFLAGS) -o $@ $^

libebcvm.a: $(OBJS)
	$(AR) -rc $@ $^

$(OBJS): $(HDRS)

test: $(TESTS)
	@for test in $(TESTS) ; do \
	  ./$${test} ; \
	  out=$$? ; \
	  if [ "$${out}" != 0 ]; then \
	  	echo "$${test}: NG" ; \
		exit $${out} ; \
	  fi \
	done

clean:
	@rm -f ebcvm libebcvm.a $(TOOLS) $(TESTS) $(OBJS) $(TEST_OBJS)

.PHONY: test clean
