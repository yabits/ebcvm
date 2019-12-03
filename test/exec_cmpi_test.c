/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void cmpi_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (int16_t)-256;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)-256;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)256;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
  }
}

static void cmpi_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIlte;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (int16_t)128;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)-128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)256;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
  }
}

static void cmpi_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIgte;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (int16_t)-128;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)-128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int32_t)-256;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
  }
}

static void cmpi_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIulte;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (uint16_t)128;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)64;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)256;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
  }
}

static void cmpi_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIugte;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (uint16_t)128;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)256;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);
  }

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (uint32_t)64;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
  }
}

static void cmpi_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (int32_t)-256;
  _inst->operand1 = R0;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (int32_t)-256;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = (uint16_t)-128;
  _inst->operand1 = R0;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint16_t)-128;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = false;
  _inst->imm_data = 0xffffffff;
  _inst->operand1 = R0;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x00000000ffffffff;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test09() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = true;
  _inst->imm_data = 0x0123;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x10, 0x00000123);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0a() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = true; /* operand1 index present */
  _inst->op1_indirect = true;
  _inst->opt_idx = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->imm_data = 0x0123;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x20, 0x00000123);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0b() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = true;
  _inst->imm_data = 0x0123;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x10, 0x0000000000000123);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0c() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 2; /* 16-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = true; /* operand1 index present */
  _inst->op1_indirect = true;
  _inst->opt_idx = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->imm_data = 0x0123;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x20, 0x0000000000000123);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0d() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = true;
  _inst->imm_data = 0x01234567;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x10, 0x01234567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0e() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 4; /* 32-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = true; /* operand1 index present */
  _inst->op1_indirect = true;
  _inst->opt_idx = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->imm_data = 0x01234567;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x20, 0x01234567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test0f() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = false; /* operand1 index absent */
  _inst->op1_indirect = true;
  _inst->imm_data = 0x01234567;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x10, 0x0000000001234567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}

static void cmpi_test10() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->imm_len = 4; /* 32-bit immediate data */
  _inst->mov_len = 8; /* 64-bit comparison */
  _inst->opcode = CMPIeq;
  _inst->is_opt_idx = true; /* operand1 index present */
  _inst->op1_indirect = true;
  _inst->opt_idx = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->imm_data = 0x01234567;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x20, 0x0000000001234567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);
}


int main() {
  cmpi_test01();
  cmpi_test02();
  cmpi_test03();
  cmpi_test04();
  cmpi_test05();
  cmpi_test06();
  cmpi_test07();
  cmpi_test08();
  cmpi_test09();
  cmpi_test0a();
  cmpi_test0b();
  cmpi_test0c();
  cmpi_test0d();
  cmpi_test0e();
  cmpi_test0f();
  cmpi_test10();

  return 0;
}
