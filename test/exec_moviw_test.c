#include "ebcvm.h"
#include <assert.h>

static void movibw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void movibw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void movibw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 1;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void moviww_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void moviww_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void moviww_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 2;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void movidw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void movidw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x20) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movidw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 4;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x30) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
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

static void moviqw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x20) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void moviqw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVI;
  _inst->mov_len = 8;
  _inst->imm_len = 2;
  _inst->imm_data = (uint16_t)0xcdef;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movibw_test01();
  movibw_test02();
  movibw_test03();

  moviww_test01();
  moviww_test02();
  moviww_test03();

  movidw_test01();
  movidw_test02();
  movidw_test03();

  moviqw_test01();
  moviqw_test02();
  moviqw_test03();

  return 0;
}
