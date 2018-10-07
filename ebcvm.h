#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 1024

typedef enum reg {
  IP = 0,
  FLAGS,
  RV2,
  RV3,
  RV4,
  RV5,
  RV6,
  RV7,
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
  uint64_t regs[14];
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
  NOP = 0x00,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  AND,
  OR,
  XOR,
  SHL,
  SHR,
  NEG,
  NOT,
  MULU = 0x10,
  DIVU,
  MODU,
  RET,
  STORESP,
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

/* decode.c */
inst *decode_op(uint64_t);

/* exec.c */
vm *exec_op(vm *, inst *);

/* mem.c */
void init_mem(mem *);
uint8_t read_mem8(mem *, size_t);
uint16_t read_mem16(mem *, size_t);
uint32_t read_mem32(mem *, size_t);
uint64_t read_mem64(mem *, size_t);
void write_mem8(mem *, size_t, uint8_t);
void write_mem16(mem *, size_t, uint16_t);
void write_mem32(mem *, size_t, uint32_t);
void write_mem64(mem *, size_t, uint64_t);

/* vm.c */
vm *init_vm(void);
vm *step_inst(vm *);

/* util.c */
void error(const char *, ...);
