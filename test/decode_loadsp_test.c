#include <assert.h>
#include "ebcvm.h"

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

static void op1_test(uint8_t *op) {
  const reg ops[] = { IP, FLAGS, RV2, RV3, RV4, RV5, RV6, RV7 };
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

static void loadsp_test() {
  uint8_t *op = malloc(sizeof(uint8_t) * 2);
  op[0] = 0x29;
  op[1] = 0x00;

  opecode_test(op, LOADSP);
  op2_test(op);
  op1_test(op);
  free(op);
}

int main() {
  loadsp_test();

  return 0;
}
