#include "ebcvm.h"
#include <assert.h>

static void movww_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  assert(_vm->regs->regs[_inst->operand1] == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  assert(read_mem16(_vm->mem, 0x20) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x20, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x20, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x10) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test12() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  assert(read_mem16(_vm->mem, 0x30) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test14() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x30, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x20) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test21() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x30, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test23() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x30, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x10) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

static void movww_test35() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVww;
  _inst->op_len = 2;
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
  write_mem16(_vm->mem, 0x50, 0xcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem16(_vm->mem, 0x30) == 0xcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movww_test01();
  movww_test02();
  movww_test03();
  movww_test04();

  movww_test12();
  movww_test14();

  movww_test21();
  movww_test23();

  movww_test35();

  return 0;
}
