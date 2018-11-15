#include "ebcvm.h"
#include <assert.h>

static void movrelw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 2;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x10 + 4 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movrelw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 2;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x10 + 4 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movrelw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 2;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x40) == 0x10 + 6 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movreld_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 4;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x10 + 6 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movreld_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 4;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x10 + 6 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movreld_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 4;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x40) == 0x10 + 8 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movrelq_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 8;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x10 + 10 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movrelq_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 8;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = false;
  _inst->opt_idx = 0;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x30) == 0x10 + 10 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

static void movrelq_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVREL;
  _inst->imm_len = 8;
  _inst->imm_data = 0x10;
  _inst->is_opt_idx = true;
  _inst->opt_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x30;
  _vm->regs->regs[IP] = 0x10;
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x40) == 0x10 + 12 + 0x10);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movrelw_test01();
  movrelw_test02();
  movrelw_test03();

  movreld_test01();
  movreld_test02();
  movreld_test03();

  movrelq_test01();
  movrelq_test02();
  movrelq_test03();

  return 0;
}
