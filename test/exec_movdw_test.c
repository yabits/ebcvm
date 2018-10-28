#include "ebcvm.h"
#include <assert.h>

static void movdw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x20) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem32(_vm->mem, 0x20, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem32(_vm->mem, 0x20, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x10) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test12() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0x2002;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x30) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test14() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = false;
  _inst->idx_len = 2;
  _inst->op1_idx = 0x2002;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x30;
  write_mem32(_vm->mem, 0x30, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x20) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test21() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = true;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0x2002;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem32(_vm->mem, 0x30, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test23() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = true;
  _inst->idx_len = 2;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem32(_vm->mem, 0x30, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x10) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movdw_test35() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVdw;
  _inst->op_len = 4;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = true;
  _inst->idx_len = 2;
  _inst->op1_idx = 0x2002;
  _inst->op2_idx = 0x2002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x40;
  write_mem32(_vm->mem, 0x50, 0x89abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x30) == 0x89abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movdw_test01();
  movdw_test02();
  movdw_test03();
  movdw_test04();

  movdw_test12();
  movdw_test14();

  movdw_test21();
  movdw_test23();

  movdw_test35();

  return 0;
}
