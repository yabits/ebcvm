/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void movnd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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

static void movnd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x20, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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

static void movnd_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = false;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcd00;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcd10);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = false;
  _inst->op1_idx = 0;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcd00;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcd10);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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

static void movnd_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x20, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test09() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = true;
  _inst->op1_idx = 0x20000002;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test0a() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = true;
  _inst->op1_idx = 0x20000002;
  _inst->is_op2_idx = false;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x20, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x40) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test0b() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
  _inst->idx_len = 4;
  _inst->is_op1_idx = true;
  _inst->op1_idx = 0x20000002;
  _inst->is_op2_idx = true;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcd00;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x0123456789abcd10);
  fini_vm(_vm);

  free(_inst);
}

static void movnd_test0c() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVnd;
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
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem64(_vm->mem, 0x30, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x40) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movnd_test01();
  movnd_test02();
  movnd_test03();
  movnd_test04();
  movnd_test05();
  movnd_test06();
  movnd_test07();
  movnd_test08();
  movnd_test09();
  movnd_test0a();
  movnd_test0b();
  movnd_test0c();

  return 0;
}
