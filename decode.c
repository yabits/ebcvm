#include "ebcvm.h"

static opcode decode_opcode(uint8_t);
static reg decode_operand(uint8_t);

static opcode decode_opcode(uint8_t _opcode) {
  switch (_opcode & 0x3f) {
    case 0x0c:
      return ADD;
    case 0x14:
      return AND;
    default:
      return NOP;
  }
}

static reg decode_operand(uint8_t operand) {
  if (operand > 0x07)
    error("failed to decode operand");
  /* XXX: R0 is 2 in reg */
  return operand + 2;
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

  _inst->operand2 = decode_operand((op & 0x7000) >> 12);

  if (op & 0x4000)
    _inst->op1_indirect = true;
  else
    _inst->op1_indirect = false;

  _inst->operand1 = decode_operand((op & 0x0700) >> 8);

  if (_inst->is_imm)
    _inst->imm = ((op & 0xffff0000) >> 16);

  return _inst;
}
