#include "ebcvm.h"

#define OP(bits)          \
typedef struct op##bits { \
  uint##bits##_t op1;     \
  uint##bits##_t op2;     \
  uint64_t op1val;        \
  uint64_t op2val;        \
} op##bits;

#define READ_OP(bits)                                   \
static op##bits *read_op##bits(vm *_vm, inst *_inst) {  \
  op##bits *_op##bits = malloc(sizeof(op##bits));       \
  _op##bits->op2val = _vm->regs->regs[_inst->operand2]; \
  if (_inst->op2_indirect)                              \
    _op##bits->op2 = read_mem##bits(_vm->mem,           \
                  _op##bits->op2val + _inst->imm * 2);  \
  else                                                  \
    _op##bits->op2 = _op##bits->op2val + _inst->imm;    \
  _op##bits->op1val = _vm->regs->regs[_inst->operand1]; \
  if (_inst->op1_indirect)                              \
    _op##bits->op1 = read_mem##bits(_vm->mem,           \
                  _op##bits->op1val);                   \
  else                                                  \
    _op##bits->op1 = _op##bits->op1val;                 \
  return _op##bits;                                     \
}

#define EXEC_OP(type, name, op)                                     \
static type _##name(type op1, type op2) {                           \
  return op;                                                        \
}                                                                   \
static vm *exec_##name(vm *_vm, inst *_inst) {                      \
  if (_inst->is_64op) {                                             \
    op64 *_op64 = read_op64(_vm, _inst);                            \
    type _op = _##name(_op64->op1, _op64->op2);                     \
    if (_inst->op1_indirect)                                        \
      write_mem64(_vm->mem, _op64->op1val, _op);                    \
    else                                                            \
      _vm->regs->regs[_inst->operand1] = _op;                       \
    free(_op64);                                                    \
  } else {                                                          \
    op32 *_op32 = read_op32(_vm, _inst);                            \
    type _op = (type)_##name(_op32->op1, _op32->op2);               \
    if (_inst->op1_indirect)                                        \
      write_mem32(_vm->mem, _op32->op1val, _op);                    \
    else                                                            \
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & _op;\
    free(_op32);                                                    \
  }                                                                 \
  return _vm;                                                       \
}

OP(32);
OP(64);

READ_OP(32);
READ_OP(64);

EXEC_OP(int64_t, add, (op1 + op2));
EXEC_OP(int64_t, sub, op1 - op2);
EXEC_OP(int64_t, mul, op1 * op2);
EXEC_OP(int64_t, div, op1 / op2);
EXEC_OP(int64_t, mod, op1 % op2);
EXEC_OP(int64_t, and, op1 & op2);
EXEC_OP(int64_t, or,  op1 | op2);
EXEC_OP(int64_t, xor, op1 ^ op2);
EXEC_OP(int64_t, shl, op1 << op2);
EXEC_OP(int64_t, shr, op1 >> op2);
EXEC_OP(int64_t, neg, -1 * op2);
EXEC_OP(int64_t, not, ~op2);
EXEC_OP(uint64_t, mulu, op1 * op2);
EXEC_OP(uint64_t, divu, op1 / op2);
EXEC_OP(uint64_t, modu, op1 % op2);

typedef vm *(*arith_op)(vm *, inst *);

arith_op arith_ops[] = {
  NULL,
  exec_add,
  exec_sub,
  exec_mul,
  exec_div,
  exec_mod,
  exec_and,
  exec_or,
  exec_xor,
  exec_shl,
  exec_shr,
  exec_neg,
  exec_not,
  exec_mulu,
  exec_divu,
  exec_modu,
};

static vm *exec_ret(vm *_vm, inst *_inst) {
  _vm->regs->regs[IP] = read_mem64(_vm->mem, _vm->regs->regs[R0]);
  _vm->regs->regs[R0] = _vm->regs->regs[R0] + 16;

  return _vm;
}

static vm *exec_storesp(vm *_vm, inst *_inst) {
  _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2];

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
  if (_inst->opcode >= ADD && _inst->opcode <= MODU) {
    arith_ops[_inst->opcode](_vm, _inst);
    inc_ip(_vm);
    goto done;
  }
  switch (_inst->opcode) {
    case RET:
      exec_ret(_vm, _inst);
      break;
    case STORESP:
      exec_storesp(_vm, _inst);
      inc_ip(_vm);
      break;
    default:
      exec_nop(_vm, _inst);
      inc_ip(_vm);
      /* do nothing */
  }

done:
  free(_inst);
  return _vm;
}
