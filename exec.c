#include "ebcvm.h"

#define ARITH_OP(NAME, OP) \
  static int64_t NAME(int64_t op1, int64_t op2) { return op1 OP op2; }
#define UARITH_OP(NAME, OP) \
  static uint64_t NAME(uint64_t op1, uint64_t op2) { return op1 OP op2; }

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

typedef int64_t (*arith_op)(int64_t, int64_t);
typedef uint64_t (*uarith_op)(uint64_t, uint64_t);

static op32 *read_op32(vm *, inst *);
static op64 *read_op64(vm *, inst *);
static vm *exec_arith(vm *, inst *, int64_t (*func)(int64_t, int64_t));
static vm *exec_uarith(vm *, inst *, uint64_t (*func)(uint64_t, uint64_t));
static vm *exec_ret(vm *, inst *);
static vm *exec_nop(vm *, inst *);

ARITH_OP(_add, +)
ARITH_OP(_sub, -)
ARITH_OP(_mul, *)
ARITH_OP(_div, /)
ARITH_OP(_mod, %)
ARITH_OP(_and, &)
ARITH_OP(_or, |)
ARITH_OP(_xor, ^)
ARITH_OP(_shl, <<)
ARITH_OP(_shr, >>)
static int64_t _neg(int64_t op1, int64_t op2) { return -1 * op2; }
static int64_t _not(int64_t op1, int64_t op2) { return ~op2; }
UARITH_OP(_mulu, *)
UARITH_OP(_divu, /)
UARITH_OP(_modu, %)

arith_op arith_ops[] = {
  NULL,
  _add,
  _sub,
  _mul,
  _div,
  _mod,
  _and,
  _or,
  _xor,
  _shl,
  _shr,
  _neg,
  _not,
};

uarith_op uarith_ops[] = {
  _mulu,
  _divu,
  _modu,
};

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

static vm *exec_arith(vm *_vm, inst *_inst,
    int64_t (*func)(int64_t, int64_t)) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    int64_t op = (int64_t)func(_op64->op1, _op64->op2);

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    int32_t op = (int32_t)func(_op32->op1, _op32->op2);

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_uarith(vm *_vm, inst *_inst,
    uint64_t (*func)(uint64_t, uint64_t)) {
  if (_inst->is_64op) {
    op64 *_op64 = read_op64(_vm, _inst);
    uint64_t op = (uint64_t)func(_op64->op1, _op64->op2);

    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _op64->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = op;
    free(_op64);
  } else {
    op32 *_op32 = read_op32(_vm, _inst);
    uint32_t op = (uint32_t)func(_op32->op1, _op32->op2);

    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _op32->op1val, op);
    else
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & op;
    free(_op32);
  }

  return _vm;
}

static vm *exec_ret(vm *_vm, inst *_inst) {
  _vm->regs->regs[IP] = read_mem64(_vm->mem, _vm->regs->regs[R0]);
  _vm->regs->regs[R0] = _vm->regs->regs[R0] + 16;

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
    case SUB:
    case MUL:
    case DIV:
    case MOD:
    case AND:
    case OR:
    case XOR:
    case SHL:
    case SHR:
    case NEG:
    case NOT:
      exec_arith(_vm, _inst, arith_ops[_inst->opcode]);
      inc_ip(_vm);
      break;
    case MULU:
    case DIVU:
    case MODU:
      exec_uarith(_vm, _inst, uarith_ops[_inst->opcode - 0x10]);
      inc_ip(_vm);
      break;
    case RET:
      exec_ret(_vm, _inst);
      break;
    default:
      exec_nop(_vm, _inst);
      inc_ip(_vm);
      /* do nothing */
  }
  free(_inst);
  return _vm;
}
