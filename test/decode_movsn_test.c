#include "ebcvm.h"
#include <assert.h>

static void op_len_test(uint8_t *op) {
  inst *_inst;
  _inst = decode_op(op);
  switch (_inst->opcode) {
    case MOVsnw:
      assert(_inst->op_len == 2);
      break;
    case MOVsnd:
      assert(_inst->op_len == 4);
      break;
    default:
      assert(false);
  }
  free(_inst);
}

static void is_op1_idx_test(uint8_t *op) {
  inst *_inst;
  /* operand1 index absent */
  op[0] &= ~0x80;
  _inst = decode_op(op);
  assert(_inst->is_op1_idx == false);
  free(_inst);
  /* operand1 index present */
  op[0] |= 0x80;
  _inst = decode_op(op);
  assert(_inst->is_op1_idx == true);
  free(_inst);
}

static void is_op2_idx_test(uint8_t *op) {
  inst *_inst;
  /* operand2 index absent */
  op[0] &= ~0x40;
  _inst = decode_op(op);
  assert(_inst->is_op2_idx == false);
  free(_inst);
  /* operand2 index present */
  op[0] |= 0x40;
  _inst = decode_op(op);
  assert(_inst->is_op2_idx == true);
  free(_inst);
}

static void idx_len_test(uint8_t *op) {
  inst *_inst;
  _inst = decode_op(op);
  switch (_inst->opcode) {
    case MOVsnw:
      assert(_inst->idx_len == 2);
      break;
    case MOVsnd:
      assert(_inst->idx_len == 4);
      break;
    default:
      assert(false);
  }
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

static void op_idx_test(uint8_t *op) {
  inst *_inst;
  /* operand1 absent, operand2 absent */
  op[0] &= ~0x80;
  op[0] &= ~0x40;
  _inst = decode_op(op);
  /* no assert */
  free(_inst);
  /* operand1 present, operand2 absent */
  op[0] |= 0x80;
  op[0] &= ~0x40;
  _inst = decode_op(op);
  switch (_inst->idx_len) {
    case 2:
      assert(_inst->op1_idx == 0xcdef);
      break;
    case 4:
      assert(_inst->op1_idx == 0x89abcdef);
      break;
    default:
      assert(false);
  }
  free(_inst);
  /* operand1 absent, operand2 present */
  op[0] &= ~0x80;
  op[0] |= 0x40;
  _inst = decode_op(op);
  switch (_inst->idx_len) {
    case 2:
      assert(_inst->op2_idx == 0xcdef);
      break;
    case 4:
      assert(_inst->op2_idx == 0x89abcdef);
      break;
    default:
      assert(false);
  }
  free(_inst);
  /* operand1 present, operand2 present */
  op[0] |= 0x80;
  op[0] |= 0x40;
  _inst = decode_op(op);
  switch (_inst->idx_len) {
    case 2:
      assert(_inst->op1_idx == 0xcdef);
      assert(_inst->op2_idx == 0x89ab);
      break;
    case 4:
      assert(_inst->op1_idx == 0x89abcdef);
      assert(_inst->op2_idx == 0x01234567);
      break;
    default:
      assert(false);
  }
  free(_inst);
}

static void opecode_test(uint8_t *op, opcode _opcode) {
  inst *_inst = decode_op(op);
  assert(_inst->opcode == _opcode);
  free(_inst);
}

static void movsn_test(uint8_t _op, opcode _opcode) {
  uint8_t *op = malloc(sizeof(uint8_t) * 10);
  op[0] = _op;
  op[1] = 0x00;
  op[2] = 0xef;
  op[3] = 0xcd;
  op[4] = 0xab;
  op[5] = 0x89;
  op[6] = 0x67;
  op[7] = 0x45;
  op[8] = 0x23;
  op[9] = 0x01;

  opecode_test(op, _opcode);
  op_len_test(op);
  is_op1_idx_test(op);
  is_op2_idx_test(op);
  idx_len_test(op);
  op2_indirect_test(op);
  op2_test(op);
  op1_indirect_test(op);
  op1_test(op);
  op_idx_test(op);
}

int main() {
  movsn_test(0x25, MOVsnw);
  movsn_test(0x26, MOVsnd);

  return 0;
}
