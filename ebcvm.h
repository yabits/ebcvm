#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 1024

typedef struct regs {
  uint64_t r0;
  uint64_t r1;
  uint64_t r2;
  uint64_t r3;
  uint64_t r4;
  uint64_t r5;
  uint64_t r6;
  uint64_t r7;
  uint64_t ip;
  uint64_t flags;
} regs;

typedef struct mem {
  uint8_t *body;
  size_t size;
} mem;

typedef struct vm {
  regs *regs;
  mem *mem;
} vm;

/* mem.c */
void init_mem(mem *);
uint8_t read_mem(mem *, size_t);
void write_mem(mem *, size_t, uint8_t);

/* vm.c */
vm *init_vm(void);

/* util.c */
void error(const char *, ...);
