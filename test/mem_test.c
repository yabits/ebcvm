/* SPDX-License-Identifier: MIT */

#include <assert.h>
#include "ebcvm.h"

void init_mem_test(void);
void write_mem_test(void);
void read_mem_test(void);

void init_mem_test() {
  mem *_mem = init_mem();
  assert(_mem);
  assert(_mem->body);
}

void write_mem_test() {
  mem *_mem = init_mem();

  write_mem8(_mem, 0x00, 0x01);
  assert(_mem->body[0x00] == 0x01);

  write_mem16(_mem, 0x02, 0x1234);
  assert(_mem->body[0x02] == 0x34);
  assert(_mem->body[0x03] == 0x12);

  write_mem32(_mem, 0x04, 0x98765432);
  assert(_mem->body[0x04] == 0x32);
  assert(_mem->body[0x05] == 0x54);
  assert(_mem->body[0x06] == 0x76);
  assert(_mem->body[0x07] == 0x98);

  write_mem64(_mem, 0x08, 0x0123456789abcdef);
  assert(_mem->body[0x08] == 0xef);
  assert(_mem->body[0x09] == 0xcd);
  assert(_mem->body[0x0a] == 0xab);
  assert(_mem->body[0x0b] == 0x89);
  assert(_mem->body[0x0c] == 0x67);
  assert(_mem->body[0x0d] == 0x45);
  assert(_mem->body[0x0e] == 0x23);
  assert(_mem->body[0x0f] == 0x01);
}

void read_mem_test() {
  mem *_mem = init_mem();

  write_mem8(_mem, 0x00, 0x01);
  assert(read_mem8(_mem, 0x00) == 0x01);

  write_mem16(_mem, 0x02, 0x1234);
  assert(read_mem16(_mem, 0x02) == 0x1234);

  write_mem32(_mem, 0x04, 0x98765432);
  assert(read_mem32(_mem, 0x04) == 0x98765432);

  write_mem64(_mem, 0x08, 0x0123456789abcdef);
  assert(read_mem64(_mem, 0x08) == 0x0123456789abcdef);
}

int main() {
  init_mem_test();
  write_mem_test();
  read_mem_test();

  return 0;
}
