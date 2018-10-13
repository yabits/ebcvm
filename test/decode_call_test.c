#include <assert.h>
#include "ebcvm.h"

static void is_jmp_imm_test(uint8_t *op) {
  inst *_inst;
  /* imm absent */
  op[0] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->is_jmp_imm == false);
  free(_inst);
  /* imm present */
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->is_jmp_imm == true);
  free(_inst);
}

static void is_jmp64_test(uint8_t *op) {
  inst *_inst;
  /* 32-bit op */
  op[0] &= ~0x40;
  _inst = decode_op(op);
  assert(_inst->is_jmp64 == false);
  free(_inst);
  /* 64-bit op */
  op[0] |= 0x40;
  _inst = decode_op(op);
  assert(_inst->is_jmp64 == true);
  free(_inst);
}

static void jmp_imm_test(uint8_t *op) {
  inst *_inst;
  /* call32 with imm absent */
  op[0] &= ~0x40;
  op[0] &= ~0x80;
  _inst = decode_op(op);
  /* no assert */
  /* call32 with imm present */
  op[0] &= ~0x40;
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->jmp_imm == 0x89abcdef);
  /* call64 with imm present */
  op[0] |= 0x40;
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->jmp_imm == 0x0123456789abcdef);
}

static void is_native_test(uint8_t *op) {
  inst *_inst;
  /* call to EBC */
  op[1] &= ~0x20;
  _inst = decode_op(op);
  assert(_inst->is_native == false);
  free(_inst);
  /* call to native code */
  op[1] |= 0x20;
  _inst = decode_op(op);
  assert(_inst->is_native == true);
  free(_inst);
}

static void is_rel_test(uint8_t *op) {
  inst *_inst;
  /* absolute address */
  op[1] &= ~0x10;
  _inst = decode_op(op);
  assert(_inst->is_rel == false);
  free(_inst);
  /* relative address */
  op[1] |= 0x10;
  _inst = decode_op(op);
  assert(_inst->is_rel == true);
  free(_inst);
}

static void op1_indirect_test(uint8_t *op) {
  inst *_inst;
  /* operand1 direct */
  op[1] &= ~0x08;
  _inst = decode_op(op);
  assert(_inst->op1_indirect == false);
  free(_inst);
  /* operand1 indirect */
  op[1] |= 0x08;
  _inst = decode_op(op);
  assert(_inst->op1_indirect == true);
  free(_inst);
}

static void op1_test(uint8_t *op) {
  const reg ops[] = { R0, R1, R2, R3, R4, R5, R6, R7 };
  inst *_inst;
  for (uint8_t op1 = 0x0; op1 < 0x8; op1++) {
    op[1] &= ~(0x7 << 0);
    op[1] |= op1 << 0;
    _inst = decode_op(op);
    assert(_inst->operand1 == ops[op1]);
    free(_inst);
  }
}

static void opecode_test(uint8_t *op, opcode _opcode) {
  inst *_inst = decode_op(op);
  assert(_inst->opcode == _opcode);
  free(_inst);
}

static void call_test() {
  uint8_t *op = malloc(sizeof(uint8_t) * 10);
  op[0] = 0x03;
  op[1] = 0x00;
  op[2] = 0xef;
  op[3] = 0xcd;
  op[4] = 0xab;
  op[5] = 0x89;
  op[6] = 0x67;
  op[7] = 0x45;
  op[8] = 0x23;
  op[9] = 0x01;

  opecode_test(op, CALL);
  is_jmp64_test(op);
  is_jmp_imm_test(op);
  jmp_imm_test(op);
  is_native_test(op);
  is_rel_test(op);
  op1_indirect_test(op);
  op1_test(op);
  free(op);
}

int main() {
  call_test();

  return 0;
}
