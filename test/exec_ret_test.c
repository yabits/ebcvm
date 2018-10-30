#include "ebcvm.h"
#include <assert.h>

static void ret_test() {
  vm *_vm = init_vm();
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = RET;
  _vm->regs->regs[R0] = 0x10;
  write_mem64(_vm->mem, 0x10, 0x0123456789abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x0123456789abcdef);
  assert(_vm->regs->regs[R0] == 0x20);
  fini_vm(_vm);
  free(_inst);
}

int main() {
  ret_test();

  return 0;
}
