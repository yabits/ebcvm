#include "ebcvm.h"
#include <assert.h>

static void storesp_test() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = STORESP;
  for (_inst->operand2 = IP; _inst->operand2 <= RV7; _inst->operand2++) {
    for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x0123456789abcdef;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
      fini_vm(_vm);
    }
  }
}

int main() {
  storesp_test();

  return 0;
}
