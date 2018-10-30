#include "ebcvm.h"
#include <assert.h>

static void movibd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void movibd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void movibd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void moviwd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void moviwd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void moviwd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void movidd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void movidd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x20) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movidd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void moviqd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
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

static void moviqd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 4;
  _inst->imm_data = (uint32_t)0x89abcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movibd_test01();
  movibd_test02();
  movibd_test03();

  moviwd_test01();
  moviwd_test02();
  moviwd_test03();

  movidd_test01();
  movidd_test02();
  movidd_test03();

  moviqd_test01();
  moviqd_test02();
  moviqd_test03();

  return 0;
}
