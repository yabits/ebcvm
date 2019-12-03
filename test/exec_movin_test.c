/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void movinw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0x2002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == (int16_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movinw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0x2002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == (int16_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movinw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0x2002;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == (int16_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movind_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x20000002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == (int32_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movind_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x20000002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == (int32_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movind_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x20000002;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == (int32_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movinq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 8;
  _inst->imm_data = (uint64_t)0x2000000000000002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == (int64_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movinq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 8;
  _inst->imm_data = (uint64_t)0x2000000000000002;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == (int64_t)+16);
  fini_vm(_vm);

  free(_inst);
}

static void movinq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVIn;
  _inst->imm_len = 8;
  _inst->imm_data = (uint32_t)0x2000000000000002;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == (int64_t)+16);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movinw_test01();
  movinw_test02();
  movinw_test03();

  movind_test01();
  movind_test02();
  movind_test03();

  movinq_test01();
  movinq_test02();
  movinq_test03();

  return 0;
}
