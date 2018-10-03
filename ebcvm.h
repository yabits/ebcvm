#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 1024

typedef enum reg {
  IP = 0,
  FLAGS,
  R0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
} reg;

typedef struct regs {
  uint64_t regs[10];
} regs;

typedef struct mem {
  uint8_t *body;
  size_t size;
} mem;

typedef struct vm {
  regs *regs;
  mem *mem;
} vm;

typedef enum opcode {
  NOP = 0,
  ADD,
} opcode;

typedef struct inst {
  opcode opcode;
  bool is_imm;
  bool is_64op;
  union {
    bool op2_indirect;
    reg operand2;
    bool op1_indirect;
    reg operand1;
  };
  uint16_t imm;
} inst;

/* mem.c */
void init_mem(mem *);
uint8_t read_mem(mem *, size_t);
void write_mem(mem *, size_t, uint8_t);

/* vm.c */
vm *init_vm(void);

/* util.c */
void error(const char *, ...);
