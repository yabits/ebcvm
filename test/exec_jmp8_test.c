/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void jmp8_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->opcode = JMP8;
  _inst->jmp_imm = 4;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + 4 * 2);
  fini_vm(_vm);
}

static void jmp8_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_cond = false;
  _inst->is_cs = false;
  _inst->opcode = JMP8;
  _inst->jmp_imm = (int8_t)-51;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + -51 * 2);
  fini_vm(_vm);
}

static void jmp8_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_cond = true;
  _inst->is_cs = false; /* jmp if FLAGS.C is clear */
  _inst->opcode = JMP8;
  _inst->jmp_imm = 4;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + 4 * 2);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234562);
  fini_vm(_vm);
}

static void jmp8_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_cond = true;
  _inst->is_cs = true; /* jmp if FLAGS.C is set */
  _inst->opcode = JMP8;
  _inst->jmp_imm = 4;
  _inst->inst_len = 2;

  _vm = init_vm();
  _vm->regs->regs[FLAGS] &= ~0x01; /* FLAGS.C is clear */
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234562);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[FLAGS] |= 0x01; /* FLAGS.C is set */
  _vm->regs->regs[IP] = 0x01234560;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x01234560 + 2 + 4 * 2);
  fini_vm(_vm);
}

int main() {
  jmp8_test01();
  jmp8_test02();
  jmp8_test03();
  jmp8_test04();

  return 0;
}
