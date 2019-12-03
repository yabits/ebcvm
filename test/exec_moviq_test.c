/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void movibq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 8;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movibq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x20) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movibq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x30) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void moviwq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviwq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x20) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviwq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x30) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movidq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movidq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x20) == 0x089abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movidq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x30) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint64_t)0x0123456789abcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movibq_test01();
  movibq_test02();
  movibq_test03();

  moviwq_test01();
  moviwq_test02();
  moviwq_test03();

  movidq_test01();
  movidq_test02();
  movidq_test03();

  moviqq_test01();
  moviqq_test02();
  moviqq_test03();

  return 0;
}
