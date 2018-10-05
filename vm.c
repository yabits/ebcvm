#include "ebcvm.h"

static void init_regs(regs *);
static uint64_t fetch_op(vm *);

static void init_regs(regs *_regs) {
  _regs = malloc(sizeof(regs));
  for (int i = 0; i < 10; i++)
    _regs->regs[i] = 0x0;
}

static uint64_t fetch_op(vm *_vm) {
  uint64_t op = 0x0;
  uint64_t ip = _vm->regs->regs[IP];
  op += read_mem8(_vm->mem, ip + 0) << 0;
  op += read_mem8(_vm->mem, ip + 1) << 8;
  /* XXX: if bit 7 of byte 0 is present, load 16-bit imm */
  if (op & 0x80)
    op += read_mem16(_vm->mem, ip + 2) << 16;
  return op;
}

vm *init_vm() {
  vm *_vm = malloc(sizeof(vm));
  init_regs(_vm->regs);
  init_mem(_vm->mem);
  return _vm;
}

vm *step_inst(vm *_vm) {
  uint64_t op = fetch_op(_vm);
  inst *_inst = decode_op(op);
  _vm = exec_op(_vm, _inst);
  return _vm;
}
