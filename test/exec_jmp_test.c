/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void jmp_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->inst_len = 2;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[IP] = 0x01234567;
    _vm->regs->regs[_inst->operand1] = 0x02468ace;
    _vm = exec_op(_vm, _inst);
    /* XXX: if JMP32 and operand1 == R0, the register content is 0 */
    if (_inst->operand1 == R0)
      assert(_vm->regs->regs[IP] == 0x00);
    else
      assert(_vm->regs->regs[IP] == 0x02468ace);
    fini_vm(_vm);
  }
}

static void jmp_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x02468ace02468ace;
  _inst->is_jmp64 = true;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->inst_len = 10;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace02468ace);
  fini_vm(_vm);
}

static void jmp_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = true;
  _inst->is_cs = false; /* jmp if FLAGS.C is clear */
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x01234567;
  _vm->regs->regs[_inst->operand1] = 0x02468ace;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x01234567;
  _vm->regs->regs[_inst->operand1] = 0x02468ace;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234567 + 2);
  fini_vm(_vm);
}

static void jmp_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = true;
  _inst->is_cs = true; /* jmp if FLAGS.C is set */
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x01234567;
  _vm->regs->regs[_inst->operand1] = 0x02468ace;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234567 + 2);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x01234567;
  _vm->regs->regs[_inst->operand1] = 0x02468ace;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace);
  fini_vm(_vm);
}

static void jmp_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = true;
  _inst->op1_indirect = false;
  _inst->operand1 = R1;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm->regs->regs[_inst->operand1] = 0x08;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + 0x08);
  fini_vm(_vm);
}

static void jmp_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = false;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, _vm->regs->regs[_inst->operand1], 0x02468ace);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace);
  fini_vm(_vm);
}

static void jmp_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = false;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->jmp_imm = 0x20000002; /* XXX: in 64-bit, it means +0x10 */
  _inst->inst_len = 6;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x20, 0x02468ace);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace);
  fini_vm(_vm);
}

static void jmp_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = true;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, _vm->regs->regs[_inst->operand1], 0x00000008);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + 0x08);
  fini_vm(_vm);
}

static void jmp_test09() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->is_jmp64 = false;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = true;
  _inst->op1_indirect = true;
  _inst->operand1 = R1;
  _inst->jmp_imm = 0x20000002; /* XXX: in 64-bit, it means +0x10 */
  _inst->inst_len = 6;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x20, 0x00000008);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 6 + 0x08);
  fini_vm(_vm);
}

static void jmp_test0a() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x02468ace02468ace;
  _inst->is_jmp64 = true;
  _inst->opcode = JMP;
  _inst->is_cond = true;
  _inst->is_cs = false; /* jmp if FLAGS.C is clear */
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->inst_len = 10;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace02468ace);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x0123456789abcdef + 10);
  fini_vm(_vm);
}

static void jmp_test0b() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x02468ace02468ace;
  _inst->is_jmp64 = true;
  _inst->opcode = JMP;
  _inst->is_cond = true;
  _inst->is_cs = true; /* jmp if FLAGS.C is set */
  _inst->is_rel = false;
  _inst->op1_indirect = false;
  _inst->inst_len = 10;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x0123456789abcdef + 10);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x0123456789abcdef;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x02468ace02468ace);
  fini_vm(_vm);
}

static void jmp_test0c() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x02;
  _inst->is_jmp64 = true;
  _inst->opcode = JMP;
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->is_rel = true;
  _inst->op1_indirect = false;
  _inst->inst_len = 10;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x0123456789abcde0;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x0123456789abcde0 + 10 + 0x02);
  fini_vm(_vm);
}

int main() {
  jmp_test01();
  jmp_test02();
  jmp_test03();
  jmp_test04();
  jmp_test05();
  jmp_test06();
  jmp_test07();
  jmp_test08();
  jmp_test09();
  jmp_test0a();
  jmp_test0b();
  jmp_test0c();

  return 0;
}
