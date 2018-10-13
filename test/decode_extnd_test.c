#include <assert.h>
#include "ebcvm.h"

static void is_imm_test(uint8_t *op) {
  inst *_inst;
  /* imm absent */
  op[0] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->is_imm == false);
  free(_inst);
  /* imm present */
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->is_imm == true);
  assert(_inst->imm == 0x0123);
  free(_inst);
}

static void is_64op_test(uint8_t *op) {
  inst *_inst;
  /* 32-bit op */
  op[0] &= ~0x40;
  _inst = decode_op(op);
  assert(_inst->is_64op == false);
  free(_inst);
  /* 64-bit op */
  op[0] |= 0x40;
  _inst = decode_op(op);
  assert(_inst->is_64op == true);
  free(_inst);
}

static void op2_indirect_test(uint8_t *op) {
  inst *_inst;
  /* operand2 direct */
  op[1] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->op2_indirect == false);
  free(_inst);
  /* operand2 indirect */
  op[1] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->op2_indirect == true);
  free(_inst);
}

static void op2_test(uint8_t *op) {
  const reg ops[] = { R0, R1, R2, R3, R4, R5, R6, R7 };
  inst *_inst;
  for (uint8_t op2 = 0x0; op2 < 0x8; op2++) {
    op[1] &= ~(0x7 << 4);
    op[1] |= op2 << 4;
    _inst = decode_op(op);
    assert(_inst->operand2 == ops[op2]);
    free(_inst);
  }
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

static void extnd_test(uint8_t _op, opcode _opcode) {
  uint8_t *op = malloc(sizeof(uint8_t) * 4);
  op[0] = _op;
  op[1] = 0x00;
  op[2] = 0x23;
  op[3] = 0x01;

  opecode_test(op, _opcode);
  is_imm_test(op);
  is_64op_test(op);
  op2_indirect_test(op);
  op2_test(op);
  op1_indirect_test(op);
  op1_test(op);
  free(op);
}

int main() {
  extnd_test(0x1a, EXTNDB);
  extnd_test(0x1b, EXTNDW);
  extnd_test(0x1c, EXTNDD);

  return 0;
}
