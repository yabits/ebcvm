#include "ebcvm.h"
#include <assert.h>

static void break1_test() {
  vm *_vm = init_vm();
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = BREAK;
  _inst->break_code = 0x01;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[R7] == (MAJOR_VERSION << 16) + MINOR_VERSION);
  free(_vm);
  free(_inst);
}

static void break6_test() {
  vm *_vm = init_vm();
  _vm->regs->regs[R7] = 0x01234567;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = BREAK;
  _inst->break_code = 0x06;
  _vm = exec_op(_vm, _inst);
  assert(_vm->compiler_version == 0x01234567);
  free(_vm);
  free(_inst);
}

int main() {
  break1_test();
  break6_test();

  return 0;
}
