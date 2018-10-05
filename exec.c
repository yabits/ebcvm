#include "ebcvm.h"

vm *exec_op(vm *_vm, inst *_inst) {
  switch (_inst->opcode) {
    case ADD:
      exec_add(_vm, _inst);
      inc_ip(_vm);
      break;
    default:
      exec_nop(_vm, _inst);
      inc_ip(_vm);
      /* do nothing */
  }
  return _vm;
}

vm *exec_add(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    uint64_t op2;
    uint64_t op2val = _vm->regs->regs[_inst->operand2];
    if (_inst->op2_indirect)
      op2 = read_mem64(_vm->mem, op2val + _inst->imm * 4); /* FIXME */
    else
      op2 = op2val + _inst->imm;

    uint64_t op1;
    uint64_t op1val = _vm->regs->regs[_inst->operand1];
    if (_inst->op1_indirect)
      op1 = read_mem64(_vm->mem, op1val);
    else
      op1 = op1val;

    op1 = op1 + op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, op1val, op1);
    else
      _vm->regs->regs[_inst->operand1] = op1;
  } else {
    uint32_t op2;
    uint64_t op2val = _vm->regs->regs[_inst->operand2];
    if (_inst->op2_indirect)
      op2 = read_mem32(_vm->mem, op2val + _inst->imm * 2); /* FIXME */
    else
      op2 = op2val + _inst->imm;

    uint32_t op1;
    uint64_t op1val = _vm->regs->regs[_inst->operand1];
    if (_inst->op1_indirect)
      op1 = read_mem32(_vm->mem, op1val);
    else
      op1 = op1val;

    op1 = op1 + op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, op1val, op1);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op1;
  }

  return _vm;
}

vm *exec_nop(vm *_vm, inst *_inst) {
  /* do nothing */
  return _vm;
}

vm *inc_ip(vm *_vm) {
  _vm->regs->regs[IP]++;
  return _vm;
}
