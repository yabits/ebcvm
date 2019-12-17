/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"

mem *init_mem() {
  int mem_size = FLAGS_mem;
  if (!FLAGS_mem)
    mem_size = MEM_SIZE;
  mem *_mem = (mem *)malloc(sizeof(mem));
  if (!_mem)
    error("malloc failed");
  _mem->body = (uint8_t *)malloc(sizeof(uint8_t) * mem_size);
  if (!_mem->body)
    error("malloc failed");
  _mem->size = mem_size;

  return _mem;
}

mem *realloc_mem(mem *_mem, size_t size) {
  _mem->body = (uint8_t *)realloc(_mem->body, sizeof(uint8_t) * size);
  if (!_mem->body)
    error("realloc failed");
  _mem->size = size;

  return _mem;
}

void fini_mem(mem *_mem) {
  free(_mem->body);
  free(_mem);
}

uint8_t read_mem8(mem *_mem, size_t idx) {
  if (idx > _mem->size)
    raise_except(MEMORY, "out of memory", __FILE__, __LINE__);
  return _mem->body[idx];
}

uint16_t read_mem16(mem *_mem, size_t idx) {
  uint16_t res = 0;
  res += (uint16_t)read_mem8(_mem, idx + sizeof(uint8_t) * 0) << 0;
  res += (uint16_t)read_mem8(_mem, idx + sizeof(uint8_t) * 1) << 8;
  return res;
}

uint32_t read_mem32(mem *_mem, size_t idx) {
  uint32_t res = 0;
  res += (uint32_t)read_mem16(_mem, idx + sizeof(uint16_t) * 0) << 0;
  res += (uint32_t)read_mem16(_mem, idx + sizeof(uint16_t) * 1) << 16;
  return res;
}

uint64_t read_mem64(mem *_mem, size_t idx) {
  uint64_t res = 0;
  res += (uint64_t)read_mem32(_mem, idx + sizeof(uint32_t) * 0) << 0;
  res += (uint64_t)read_mem32(_mem, idx + sizeof(uint32_t) * 1) << 32;
  return res;
}

void write_mem8(mem *_mem, size_t idx, uint8_t val) {
  if (idx > _mem->size)
    raise_except(MEMORY, "out of memory", __FILE__, __LINE__);
  _mem->body[idx] = val;
}

void write_mem16(mem *_mem, size_t idx, uint16_t val) {
  write_mem8(_mem, idx + sizeof(uint8_t) * 0, (uint8_t)(val >> 0));
  write_mem8(_mem, idx + sizeof(uint8_t) * 1, (uint8_t)(val >> 8));
}

void write_mem32(mem *_mem, size_t idx, uint32_t val) {
  write_mem16(_mem, idx + sizeof(uint16_t) * 0, (uint16_t)(val >> 0));
  write_mem16(_mem, idx + sizeof(uint16_t) * 1, (uint16_t)(val >> 16));
}

void write_mem64(mem *_mem, size_t idx, uint64_t val) {
  write_mem32(_mem, idx + sizeof(uint32_t) * 0, (uint32_t)(val >> 0));
  write_mem32(_mem, idx + sizeof(uint32_t) * 1, (uint32_t)(val >> 32));
}

#if ARCH_BYTES == 4
uint32_t read_memn(mem *_mem, size_t idx) {
  return read_mem32(_mem, idx);
}
void write_memn(mem *_mem, size_t idx, uint32_t val) {
  write_mem32(_mem, idx, val);
}
#elif ARCH_BYTES == 8
uint64_t read_memn(mem *_mem, size_t idx) {
  return read_mem64(_mem, idx);
}
void write_memn(mem *_mem, size_t idx, uint64_t val) {
  write_mem64(_mem, idx, val);
}
#else
#error unsupported architecture
#endif
