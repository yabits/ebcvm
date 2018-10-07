#include "ebcvm.h"

static opcode decode_opcode(uint8_t);
static reg decode_gp_reg(uint8_t);
static reg decode_dd_reg(uint8_t);

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
  NOP, /* 0x1d */
  NOP, /* 0x1e */
  NOP, /* 0x1f */
  NOP, /* 0x20 */
  NOP, /* 0x21 */
  NOP, /* 0x22 */
  NOP, /* 0x23 */
  NOP, /* 0x24 */
  NOP, /* 0x25 */
  NOP, /* 0x26 */
  NOP, /* 0x27 */
  NOP, /* 0x28 */
  NOP, /* 0x29 */
  STORESP, /* 0x2a */
  NOP, /* 0x2b */
  NOP, /* 0x2c */
  NOP, /* 0x2d */
  NOP, /* 0x2e */
  NOP, /* 0x2f */
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

inst *decode_op(uint64_t op) {
  inst *_inst = malloc(sizeof(inst));

  if (op & 0x80)
    _inst->is_imm = true;
  else
    _inst->is_imm = false;

  if (op & 0x40)
    _inst->is_64op = true;
  else
    _inst->is_64op = false;

  _inst->opcode = decode_opcode(op & 0x3f);

  if (op & 0x8000)
    _inst->op2_indirect = true;
  else
    _inst->op2_indirect = false;

  if (_inst->opcode == STORESP)
    _inst->operand2 = decode_dd_reg((op & 0x7000) >> 12);
  else
    _inst->operand2 = decode_gp_reg((op & 0x7000) >> 12);

  if (op & 0x4000)
    _inst->op1_indirect = true;
  else
    _inst->op1_indirect = false;

  _inst->operand1 = decode_gp_reg((op & 0x0700) >> 8);

  if (_inst->is_imm)
    _inst->imm = ((op & 0xffff0000) >> 16);

  return _inst;
}
