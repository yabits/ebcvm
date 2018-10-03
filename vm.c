#include "ebcvm.h"

static void init_regs(regs *_regs) {
  _regs = malloc(sizeof(regs));
  _regs->r0 = 0x0;
  _regs->r1 = 0x0;
  _regs->r2 = 0x0;
  _regs->r3 = 0x0;
  _regs->r4 = 0x0;
  _regs->r5 = 0x0;
  _regs->r6 = 0x0;
  _regs->r7 = 0x0;
  _regs->ip = 0x0;
  _regs->flags = 0x0;
}

vm *init_vm() {
  vm *_vm = malloc(sizeof(vm));
  init_regs(_vm->regs);
  init_mem(_vm->mem);
  return _vm;
}
