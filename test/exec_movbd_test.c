#include "ebcvm.h"
#include <assert.h>

static void movbd_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
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
  assert(read_mem8(_vm->mem, 0x20) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem8(_vm->mem, 0x20, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem8(_vm->mem, 0x20, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x10) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test12() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
  _inst->op1_idx = 0x20000002;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x30) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test14() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = false;
  _inst->idx_len = 4;
  _inst->op1_idx = 0x20000002;
  _inst->op2_idx = 0;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x30;
  write_mem8(_vm->mem, 0x30, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x20) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test21() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = true;
  _inst->idx_len = 4;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = false;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem8(_vm->mem, 0x30, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test23() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = false;
  _inst->is_op2_idx = true;
  _inst->idx_len = 4;
  _inst->op1_idx = 0;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  _vm->regs->regs[_inst->operand2] = 0x20;
  write_mem8(_vm->mem, 0x30, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x10) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

static void movbd_test35() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = MOVbd;
  _inst->op_len = 1;
  _inst->is_op1_idx = true;
  _inst->is_op2_idx = true;
  _inst->idx_len = 4;
  _inst->op1_idx = 0x20000002;
  _inst->op2_idx = 0x20000002;
  _inst->op1_indirect = true;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x20;
  _vm->regs->regs[_inst->operand2] = 0x40;
  write_mem8(_vm->mem, 0x50, 0xef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem8(_vm->mem, 0x30) == 0xef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  movbd_test01();
  movbd_test02();
  movbd_test03();
  movbd_test04();

  movbd_test12();
  movbd_test14();

  movbd_test21();
  movbd_test23();

  movbd_test35();

  return 0;
}
