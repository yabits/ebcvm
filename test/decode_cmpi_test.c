/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void imm_len_test(uint8_t *op) {
  inst *_inst;
  /* immediate data is 16 bits */
  op[0] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->imm_len == 2);
  free(_inst);
  /* immediate data is 32 bits */
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->imm_len == 4);
  free(_inst);
}

static void mov_len_test(uint8_t *op) {
  inst *_inst;
  /* 32 bit comparison */
  op[0] &= ~0x40;
  _inst = decode_op(op);
  assert(_inst->mov_len == 4);
  free(_inst);
  /* 64 bit comparison */
  op[0] |= 0x40;
  _inst = decode_op(op);
  assert(_inst->mov_len == 8);
  free(_inst);
}

static void is_opt_idx_test(uint8_t *op) {
  inst *_inst;
  /* operand1 index absent */
  op[1] &= ~0x10;
  _inst = decode_op(op);
  assert(_inst->is_opt_idx == false);
  free(_inst);
  /* operand1 index present */
  op[1] |= 0x10;
  _inst = decode_op(op);
  assert(_inst->is_opt_idx == true);
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

static void opt_idx_test(uint8_t *op) {
  inst *_inst;
  /* operand1 index absent */
  op[1] &= ~0x10;
  _inst = decode_op(op);
  /* no assert */
  free(_inst);
  /* operand1 index present */
  op[1] |= 0x10;
  _inst = decode_op(op);
  assert(_inst->opt_idx == 0xcdef);
  free(_inst);
}

static void imm_data_test(uint8_t *op) {
  inst *_inst;
  /* operand1 index absent */
  op[1] &= ~0x10;
  for (int i = 0; i < 1; i++) {
    op[0] &= ~0xc0;
    op[0] |= i << 6;
    _inst = decode_op(op);
    switch (_inst->imm_len) {
      case 2:
        assert(_inst->imm_data == 0xcdef);
        break;
      case 4:
        assert(_inst->imm_data == 0x89abcdef);
        break;
      default:
        assert(false);
    }
    free(_inst);
  }
  /* operand1 index present */
  op[1] |= 0x10;
  _inst = decode_op(op);
  for (int i = 0; i < 1; i++) {
    op[0] &= ~0xc0;
    op[0] |= i << 6;
    _inst = decode_op(op);
    switch (_inst->imm_len) {
      case 2:
        assert(_inst->imm_data == 0x89ab);
        break;
      case 4:
        assert(_inst->imm_data == 0x456789ab);
        break;
      default:
        assert(false);
    }
    free(_inst);
  }
}

static void opecode_test(uint8_t *op, opcode _opcode) {
  inst *_inst = decode_op(op);
  assert(_inst->opcode == _opcode);
  free(_inst);
}

static void cmpi_test(uint8_t _op, opcode _opcode) {
  uint8_t *op = malloc(sizeof(uint8_t) * 8);
  op[0] = _op;
  op[1] = 0x00;
  op[2] = 0xef;
  op[3] = 0xcd;
  op[4] = 0xab;
  op[5] = 0x89;
  op[6] = 0x67;
  op[7] = 0x45;

  opecode_test(op, _opcode);
  imm_len_test(op);
  is_opt_idx_test(op);
  mov_len_test(op);
  op1_indirect_test(op);
  op1_test(op);
  opt_idx_test(op);
  imm_data_test(op);
  free(op);
}

int main() {
  cmpi_test(0x2d, CMPIeq);
  cmpi_test(0x2e, CMPIlte);
  cmpi_test(0x2f, CMPIgte);
  cmpi_test(0x30, CMPIulte);
  cmpi_test(0x31, CMPIugte);

  return 0;
}
