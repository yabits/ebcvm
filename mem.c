#include "ebcvm.h"

void init_mem(mem *_mem) {
  _mem = malloc(sizeof(mem));
  _mem->body = malloc(sizeof(uint8_t) * MEM_SIZE);
  _mem->size = MEM_SIZE;
}

uint8_t read_mem(mem *_mem, size_t idx) {
  if (idx > _mem->size)
    error("out of memory");
  return _mem->body[idx];
}

void write_mem(mem *_mem, size_t idx, uint8_t val) {
  if (idx > _mem->size)
    error("out of memory");
  _mem->body[idx] = val;
}
