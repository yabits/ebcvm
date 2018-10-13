#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAJOR_VERSION 0x0001
#define MINOR_VERSION 0x0000
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
  uint64_t regs[16];
} regs;

typedef struct mem {
  uint8_t *body;
  size_t size;
} mem;

typedef struct vm {
  regs *regs;
  mem *mem;
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
      size_t op_len;
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

/* decode.c */
inst *decode_op(uint8_t *);

/* exec.c */
vm *exec_op(vm *, inst *);

/* mem.c */
mem *init_mem(void);
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
