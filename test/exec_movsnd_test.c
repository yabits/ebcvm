#include "ebcvm.h"
#include <assert.h>

static void movsnd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x10, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x20, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0xcdef;
  _inst->op1_indirect = false;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456789ab0000;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0xcdef;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x0123456789ab0000;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x20, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x30, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test09() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = true;
  _inst->op1_idx = 0x20000002;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0xcdef;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x0123456789ab0000;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movsnd_test0a() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVsnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = true;
  _inst->op1_idx = 0x20000002;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x30, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movsnd_test01();
  movsnd_test02();
  movsnd_test03();
  movsnd_test04();
  movsnd_test05();
  movsnd_test06();
  movsnd_test07();
  movsnd_test08();
  movsnd_test09();
  movsnd_test0a();

  return 0;
}
