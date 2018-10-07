#include "ebcvm.h"

static opcode decode_opcode(uint8_t);
static reg decode_gp_reg(uint8_t);
static reg decode_dd_reg(uint8_t);
static inst *decode_mov(inst *, uint8_t *);

opcode ops[] = {
  NOP, /* 0x00 */
  NOP, /* 0x01 */
  NOP, /* 0x02 */
  NOP, /* 0x03 */
  RET, /* 0x04 */
  NOP, /* 0x05 */
  NOP, /* 0x06 */
  NOP, /* 0x07 */
  NOP, /* 0x08 */
  NOP, /* 0x09 */
  NOT, /* 0x0a */
  NEG, /* 0x0b */
  ADD, /* 0x0c */
  SUB, /* 0x0d */
  MUL, /* 0x0e */
  MULU,/* 0x0f */
  DIV, /* 0x10 */
  DIVU,/* 0x11 */
  MOD, /* 0x12 */
  MODU,/* 0x13 */
  AND, /* 0x14 */
  OR,  /* 0x15 */
  XOR, /* 0x16 */
  SHL, /* 0x17 */
  SHR, /* 0x18 */
  NOP, /* 0x19 */
  NOP, /* 0x1a */
  NOP, /* 0x1b */
  NOP, /* 0x1c */
  MOVbw, /* 0x1d */
  MOVww, /* 0x1e */
  MOVdw, /* 0x1f */
  MOVqw, /* 0x20 */
  MOVbd, /* 0x21 */
  MOVwd, /* 0x22 */
  MOVdd, /* 0x23 */
  MOVqd, /* 0x24 */
  NOP, /* 0x25 */
  NOP, /* 0x26 */
  NOP, /* 0x27 */
  MOVqq, /* 0x28 */
  NOP, /* 0x29 */
  STORESP, /* 0x2a */
  PUSH,/* 0x2b */
  POP, /* 0x2c */
  NOP, /* 0x2d */
  NOP, /* 0x2e */
  NOP, /* 0x2f */
  NOP, /* 0x30 */
  NOP, /* 0x31 */
  NOP, /* 0x32 */
  NOP, /* 0x33 */
  NOP, /* 0x34 */
  PUSHn, /* 0x35 */
  POPn,/* 0x36 */
  NOP, /* 0x37 */
  NOP, /* 0x38 */
  NOP, /* 0x39 */
  NOP, /* 0x3a */
  NOP, /* 0x3b */
  NOP, /* 0x3c */
  NOP, /* 0x3d */
  NOP, /* 0x3e */
  NOP, /* 0x3f */
};

static opcode decode_opcode(uint8_t _opcode) {
  return ops[_opcode & 0x3f];
}

static reg decode_gp_reg(uint8_t operand) {
  if (operand > 0x07)
    error("failed to decode general purpose register");
  /* XXX: R0 is at 8 in reg*/
  return operand + 8;
}

static reg decode_dd_reg(uint8_t operand) {
  if (operand > 0x02)
    error("failed to decode general purpose register");
  return operand + 0;
}

static inst *decode_mov_idx(inst *_inst, uint8_t *op, size_t idx_len) {
  _inst->idx_len = idx_len;
  _inst->op1_idx = 0x00;
  _inst->op2_idx = 0x00;
  if (_inst->is_op1_idx) {
    for (int k = 0; k < idx_len; k++)
      _inst->op1_idx += ((uint64_t)op[k + 2] << (k * 8));
  }
  if (_inst->is_op2_idx) {
    int i = _inst->is_op1_idx ? idx_len + 2 : 2;
    for (int k = i; k < i + idx_len; k++)
      _inst->op2_idx += ((uint64_t)op[k + 2] << (k * 8));
  }

  return _inst;
}

static inst *decode_mov(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  if (_inst->opcode >= MOVbw && _inst->opcode <= MOVqq)
    goto fail;

  if (op[0] & 0x80)
    _inst->is_op1_idx = true;
  else
    _inst->is_op1_idx = false;

  if (op[0] & 0x40)
    _inst->is_op2_idx = true;
  else
    _inst->is_op2_idx = false;

  switch (_inst->opcode) {
    case MOVbw:
    case MOVbd:
      _inst->op_len = 1;
      break;
    case MOVww:
    case MOVwd:
      _inst->op_len = 2;
      break;
    case MOVdw:
    case MOVdd:
      _inst->op_len = 4;
      break;
    case MOVqw:
    case MOVqd:
    case MOVqq:
      _inst->op_len = 8;
      break;
    default:
      ;
      /* do nothing */
  }

  if (_inst->opcode >= MOVbw && _inst->opcode <= MOVqw)
    _inst = decode_mov_idx(_inst, op, 2);
  else if (_inst->opcode >= MOVbd && _inst->opcode <= MOVqd)
    _inst = decode_mov_idx(_inst, op, 4);
  else if (_inst->opcode == MOVqq)
    _inst = decode_mov_idx(_inst, op, 8);

    return _inst;

fail:
  error("failed to decode MOV");
  return NULL;
}

inst *decode_op(uint8_t *op) {
  if (!op)
    goto fail;

  inst *_inst = malloc(sizeof(inst));
  if (!_inst)
    goto fail;

  _inst->opcode = decode_opcode(op[0] & 0x3f);

  if (_inst->opcode >= MOVbw && _inst->opcode <= MOVqq) {
    _inst = decode_mov(_inst, op);
  } else {
    if (op[0] & 0x80)
      _inst->is_imm = true;
    else
      _inst->is_imm = false;
    if (op[0] & 0x40)
      _inst->is_64op = true;
    else
      _inst->is_64op = false;
    if (_inst->is_imm)
      _inst->imm = (op[2] << 0) + (op[3] << 8);
  }

  if (op[1] & 0x80)
    _inst->op2_indirect = true;
  else
    _inst->op2_indirect = false;

  if (_inst->opcode == STORESP)
    _inst->operand2 = decode_dd_reg((op[1] & 0x70));
  else
    _inst->operand2 = decode_gp_reg(op[1] & 0x70);

  if (op[1] & 0x40)
    _inst->op1_indirect = true;
  else
    _inst->op1_indirect = false;

  _inst->operand1 = decode_gp_reg(op[1] & 0x07);

  free(op);

  return _inst;

fail:
  error("failed to decode inst");
  return NULL;
}
