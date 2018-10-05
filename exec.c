#include "ebcvm.h"

typedef struct op32 {
  uint32_t op1;
  uint32_t op2;
  uint64_t op1val;
  uint64_t op2val;
} op32;

typedef struct op64 {
  uint32_t op1;
  uint32_t op2;
  uint64_t op1val;
  uint64_t op2val;
} op64;

static op32 *read_op32(vm *, inst *);
static op64 *read_op64(vm *, inst *);
static vm *exec_add(vm *, inst *);
static vm *exec_sub(vm *, inst *);
static vm *exec_mul(vm *, inst *);
static vm *exec_div(vm *, inst *);
static vm *exec_mod(vm *, inst *);
static vm *exec_and(vm *, inst *);
static vm *exec_or(vm *, inst *);
static vm *exec_xor(vm *, inst *);
static vm *exec_nop(vm *, inst *);

static op32 *read_op32(vm *_vm, inst *_inst) {
  op32 *_op32 = malloc(sizeof(op32));

  _op32->op2val = _vm->regs->regs[_inst->operand2];
  if (_inst->op2_indirect)
    _op32->op2 = read_mem32(_vm->mem, _op32->op2val + _inst->imm * 2);
  else
    _op32->op2 = _op32->op2val + _inst->imm;

  _op32->op1val = _vm->regs->regs[_inst->operand1];
  if (_inst->op1_indirect)
    _op32->op1 = read_mem32(_vm->mem, _op32->op1val);
  else
    _op32->op1 = _op32->op1val;

  return _op32;
}

static op64 *read_op64(vm *_vm, inst *_inst) {
  op64 *_op64 = malloc(sizeof(op64));

  _op64->op2val = _vm->regs->regs[_inst->operand2];
  if (_inst->op2_indirect)
    _op64->op2 = read_mem64(_vm->mem, _op64->op2val + _inst->imm * 4);
  else
    _op64->op2 = _op64->op2val + _inst->imm;

  _op64->op1val = _vm->regs->regs[_inst->operand1];
  if (_inst->op1_indirect)
    _op64->op1 = read_mem64(_vm->mem, _op64->op1val);
  else
    _op64->op1 = _op64->op1val;

  return _op64;
}

static vm *exec_add(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 + _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 + _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_sub(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 - _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 - _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_mul(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 * _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 * _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_div(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 / _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 / _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_mod(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 % _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 % _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_and(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 & _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 & _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_or(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 | _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 | _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_xor(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = _op64->op1 ^ _op64->op2;

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op  = _op32->op1 ^ _op32->op2;

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_nop(vm *_vm, inst *_inst) {
  /* do nothing */
  return _vm;
}

static vm *inc_ip(vm *_vm) {
  _vm->regs->regs[IP]++;
  return _vm;
}

vm *exec_op(vm *_vm, inst *_inst) {
  switch (_inst->opcode) {
    case ADD:
      exec_add(_vm, _inst);
      inc_ip(_vm);
      break;
    case SUB:
      exec_sub(_vm, _inst);
      inc_ip(_vm);
      break;
    case MUL:
      exec_mul(_vm, _inst);
      inc_ip(_vm);
      break;
    case DIV:
      exec_div(_vm, _inst);
      inc_ip(_vm);
      break;
    case MOD:
      exec_mod(_vm, _inst);
      inc_ip(_vm);
      break;
    case AND:
      exec_and(_vm, _inst);
      inc_ip(_vm);
      break;
    case OR:
      exec_or(_vm, _inst);
      inc_ip(_vm);
      break;
    case XOR:
      exec_xor(_vm, _inst);
      inc_ip(_vm);
      break;
    default:
      exec_nop(_vm, _inst);
      inc_ip(_vm);
      /* do nothing */
  }
  free(_inst);
  return _vm;
}
