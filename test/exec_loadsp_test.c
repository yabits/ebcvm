/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void loadsp_test() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = LOADSP;
  _inst->operand1 = FLAGS;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    _vm = init_vm();
    _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
    fini_vm(_vm);
  }
}

int main() {
  loadsp_test();

  return 0;
}
