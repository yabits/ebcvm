#ifndef EBCVM_H_
#define EBCVM_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCH_BYTES    8 /* EBC supports 32-bit or 64-bit */
#define MAJOR_VERSION 0x0001
#define MINOR_VERSION 0x0000

#define AUTO_MEM_SIZE 0
#define MEM_SIZE      8388608
#define STACK_BASE    0x0012d000
#define STACK_MAGIC   0x0ebc0ebc0ebc0ebc
#define RET_MAGIC     0xffffffffffffffff

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
  uint64_t regs[16];
} regs;

typedef struct mem {
  uint8_t *body;
  size_t size;
} mem;

typedef enum mem_type {
  MEM_DATA = 0,
  MEM_TEXT,
  MEM_BSS,
  MEM_EFI,
  MEM_UNKNOWN,
} mem_type;

typedef struct memmap {
  mem_type mem_type;
  uint64_t addr;
  size_t size;
} memmap;

typedef struct vm {
  regs *regs;
  mem *mem;
  memmap *memmap;
  size_t memmap_size;
  uint32_t compiler_version;
} vm;

typedef enum opcode {
  BREAK = 0x00,
  JMP,
  JMP8,
  CALL,
  CMPeq,
  CMPlte,
  CMPgte,
  CMPulte,
  CMPugte,
  CMPIeq,
  CMPIlte,
  CMPIgte,
  CMPIulte,
  CMPIugte,
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
  ASHR,
  NEG,
  NOT,
  MULU,
  DIVU,
  MODU,
  EXTNDB,
  EXTNDD,
  EXTNDW,
  MOVbw,
  MOVww,
  MOVdw,
  MOVqw,
  MOVbd,
  MOVwd,
  MOVdd,
  MOVqd,
  MOVqq,
  MOVI,
  MOVIn,
  MOVREL,
  MOVnw,
  MOVnd,
  MOVsnw,
  MOVsnd,
  POP,
  POPn,
  PUSH,
  PUSHn,
  RET,
  LOADSP,
  STORESP,
  NOP,
} opcode;

typedef struct inst {
  size_t inst_len;
  opcode opcode;
  bool op2_indirect;
  reg operand2;
  bool op1_indirect;
  reg operand1;
  union {
    /* BREAK */
    struct {
      uint8_t break_code;
    };
    /* CALL, JMP, and JMP8 */
    struct {
      bool is_jmp_imm;
      bool is_jmp64;
      bool is_cond;
      bool is_cs; /* jump if Flags.C is set/clear */
      bool is_native; /* call to EBC/native code */
      bool is_rel;
      uint64_t jmp_imm;
    };
    /* arithmetic ops, CMP, EXTND*,
     * LOADSP, STORESP, POP, POPn, PUSH, PUSHn */
    struct {
      bool is_imm;
      bool is_64op;
      uint16_t imm;
    };
    /* MOV, MOVn, and MOVsn */
    struct {
      size_t op_len; /* size of the data move */
      bool is_op1_idx;
      bool is_op2_idx;
      size_t idx_len;
      uint64_t op1_idx;
      uint64_t op2_idx;
    };
    /* MOVI, MOVIn, MOVREL, and CMPI */
    struct {
      bool is_opt_idx;
      size_t imm_len;
      size_t mov_len;
      uint16_t opt_idx;
      uint64_t imm_data;
    };
  };
} inst;

typedef enum except {
  DIV0,
  DEBUG,
  STEP,
  OPCODE,
  STACK,
  ALIGN,
  ENCODE,
  BADBREAK,
  EXIT,
  UNDEF,
} except;

typedef struct dbg {
  vm *_vm;
} dbg;

/* Debugger */
dbg *_dbg;

/* Debug mode */
bool FLAGS_debug;
/* Size of memory */
int FLAGS_mem;
/* Step exection */
bool FLAGS_step;

/* vm.c */
vm *init_vm(void);
void fini_vm(vm *);
vm *step_inst(vm *);
void exec_vm(vm *);
size_t dump_inst(vm *);
void dump_vm(vm *);
void raise_except(except, const char *);
void raise_excall(uint64_t, vm *);

/* debug.c */
dbg *init_dbg(vm *);
void fini_dbg(dbg *);
void handle_except(dbg *, except, const char *);

/* load.c */
vm *load_exe(const char *, vm *);

/* decode.c */
inst *decode_op(uint8_t *);

/* exec.c */
vm *exec_op(vm *, inst *);

/* mem.c */
mem *init_mem(void);
mem *realloc_mem(mem *, size_t);
void fini_mem(mem *);
uint8_t read_mem8(mem *, size_t);
uint16_t read_mem16(mem *, size_t);
uint32_t read_mem32(mem *, size_t);
uint64_t read_mem64(mem *, size_t);
void write_mem8(mem *, size_t, uint8_t);
void write_mem16(mem *, size_t, uint16_t);
void write_mem32(mem *, size_t, uint32_t);
void write_mem64(mem *, size_t, uint64_t);

#if ARCH_BYTES == 4
uint32_t read_memn(mem *, size_t);
void write_memn(mem *, size_t, uint32_t);
#elif ARCH_BYTES == 8
uint64_t read_memn(mem *, size_t);
void write_memn(mem *, size_t, uint64_t);
#else
#error unsupported architecture
#endif

/* efi.c */
vm *load_efi(uint64_t, vm *);
void handle_excall(uint64_t, vm *);

/* disas.c */
char *disas_inst(inst *);

/* util.c */
void error(const char *, ...);

#endif /* EBCVM_H_ */
