#include "ebcvm.h"

static void init_regs(regs *);
static uint64_t fetch_op(vm *);
static opcode decode_opcode(uint8_t);
static reg decode_operand(uint8_t);
static inst *decode_op(uint64_t);

static void init_regs(regs *_regs) {
  _regs = malloc(sizeof(regs));
  for (int i = 0; i < 10; i++)
    _regs->regs[i] = 0x0;
}

static uint64_t fetch_op(vm *_vm) {
  uint64_t op = 0x0;
  uint64_t ip = _vm->regs->regs[IP];
  op = read_mem(_vm->mem, ip + 0) << 0;
  op = read_mem(_vm->mem, ip + 1) << 1;
  /* XXX: if bit 7 of byte 0 is present, load 16-bit imm */
  if (op & 0x80) {
    op = read_mem(_vm->mem, ip + 2) << 2;
    op = read_mem(_vm->mem, ip + 3) << 3;
  }
  return op;
}

static opcode decode_opcode(uint8_t _opcode) {
  switch (_opcode & 0x3f) {
    case 0x0c:
      return ADD;
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

static inst *decode_op(uint64_t op) {
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

vm *init_vm() {
  vm *_vm = malloc(sizeof(vm));
  init_regs(_vm->regs);
  init_mem(_vm->mem);
  return _vm;
}

vm *step_inst(vm *_vm) {
  uint64_t op = fetch_op(_vm);
  inst *_inst = decode_op(op);
  return _vm;
}
