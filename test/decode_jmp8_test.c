#include <assert.h>
#include "ebcvm.h"

static void is_cond_test(uint8_t *op) {
  inst *_inst;
  /* unconditional jmp */
  op[0] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->is_cond == false);
  free(_inst);
  /* conditional jmp */
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->is_cond == true);
  free(_inst);
}

static void is_cs_test(uint8_t *op) {
  inst *_inst;
  /* is Flags.C is clear */
  op[0] &= ~0x40;
  _inst = decode_op(op);
  assert(_inst->is_cs == false);
  free(_inst);
  /* is Flags.C is set */
  op[0] |= 0x40;
  _inst = decode_op(op);
  assert(_inst->is_cs == true);
  free(_inst);
}

static void jmp_imm_test(uint8_t *op) {
  inst *_inst;
  _inst = decode_op(op);
  assert(_inst->jmp_imm = 0xab);
  free(_inst);
}

static void opecode_test(uint8_t *op, opcode _opcode) {
  inst *_inst = decode_op(op);
  assert(_inst->opcode == _opcode);
  free(_inst);
}

static void jmp8_test() {
  uint8_t *op = malloc(sizeof(uint8_t) * 10);
  op[0] = 0x02;
  op[1] = 0xab;

  opecode_test(op, JMP8);
  is_cond_test(op);
  is_cs_test(op);
  jmp_imm_test(op);
  free(op);
}

int main() {
  jmp8_test();

  return 0;
}
