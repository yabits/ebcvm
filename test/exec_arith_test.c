#include "ebcvm.h"
#include <assert.h>

static void add_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1;
        _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = 0x01230000;
      _vm->regs->regs[_inst->operand2] = 0x00004567;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == 0x01234567);
      fini_vm(_vm);
    }
  }
}

static void add_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1;
        _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = 0x0123456700000000;
      _vm->regs->regs[_inst->operand2] = 0x0000000089abcdef;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
      fini_vm(_vm);
    }
  }
}

static void add_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0x4567;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x01230000;
  _vm->regs->regs[_inst->operand1] = 0xf0000000;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0xf1234567);
  fini_vm(_vm);
}

static void add_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0xcdef;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x89ab0000;
  _vm->regs->regs[_inst->operand1] = 0x0123456700000000;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void add_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  _vm->regs->regs[_inst->operand1] = 0x01230000;
  write_mem32(_vm->mem, 0x10, 0x00004567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x01234567);
  fini_vm(_vm);
}

static void add_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  _vm->regs->regs[_inst->operand1] = 0x0123456700000000;
  write_mem64(_vm->mem, 0x10, 0x0000000089abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void add_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x01230000;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x10, 0x00004567);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x10) == 0x01234567);
  fini_vm(_vm);
}

static void add_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x0123456700000000;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x10, 0x0000000089abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void add_test09() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x20, 0x01230000);
  write_mem32(_vm->mem, 0x10, 0x00004567);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x10) == 0x01234567);
  fini_vm(_vm);
}

static void add_test0a() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x20, 0x0123456700000000);
  write_mem64(_vm->mem, 0x10, 0x0000000089abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void add_test0b() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  _vm->regs->regs[_inst->operand1] = 0x01230000;
  write_mem32(_vm->mem, 0x20, 0x00004567);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x01234567);
  fini_vm(_vm);
}

static void add_test0c() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x10;
  _vm->regs->regs[_inst->operand1] = 0x0123456700000000;
  write_mem64(_vm->mem, 0x20, 0x0000000089abcdef);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[_inst->operand1] == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void add_test0d() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = false;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem32(_vm->mem, 0x30, 0x01230000);
  write_mem32(_vm->mem, 0x10, 0x00004567);
  _vm = exec_op(_vm, _inst);
  assert(read_mem32(_vm->mem, 0x10) == 0x01234567);
  fini_vm(_vm);
}

static void add_test0e() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->is_64op = true;
  _inst->opcode = ADD;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;
  _inst->operand2 = R0;
  _inst->operand1 = R1;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _vm = init_vm();
  _vm->regs->regs[_inst->operand2] = 0x20;
  _vm->regs->regs[_inst->operand1] = 0x10;
  write_mem64(_vm->mem, 0x30, 0x0123456700000000);
  write_mem64(_vm->mem, 0x10, 0x0000000089abcdef);
  _vm = exec_op(_vm, _inst);
  assert(read_mem64(_vm->mem, 0x10) == 0x0123456789abcdef);
  fini_vm(_vm);
}

static void arith_test(opcode op, uint32_t op1, uint32_t op2, uint32_t exp) {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = op;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1;
        _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = op1;
      _vm->regs->regs[_inst->operand2] = op2;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == exp);
      fini_vm(_vm);
    }
  }
}

int main() {
  add_test01();
  add_test02();
  add_test03();
  add_test04();
  add_test05();
  add_test06();
  add_test07();
  add_test08();
  add_test09();
  add_test0a();
  add_test0b();
  add_test0c();
  add_test0d();
  add_test0e();

  arith_test(SUB, 0x01234567, 0x00004567, 0x01230000);
  arith_test(MUL, 0x01230000, 0x20, 0x24600000);
  arith_test(MULU, 0x01230000, 0x20, 0x24600000);
  arith_test(DIV, 0x01230000, 0x20, 0x91800);
  arith_test(DIVU, 0x01230000, 0x20, 0x91800);
  arith_test(MOD, 0x01230000, 0x21, 0x01230000 % 0x21);
  arith_test(MODU, 0x01230000, 0x21, 0x01230000 % 0x21);

  arith_test(AND, 0xc0bebeef, 0xdeadbeef, 0xc0bebeef & 0xdeadbeef);
  arith_test(OR, 0xc0bebeef, 0xdeadbeef, 0xc0bebeef | 0xdeadbeef);
  arith_test(XOR, 0xc0bebeef, 0xdeadbeef, 0xc0bebeef ^ 0xdeadbeef);
  arith_test(SHL, 0x0000beef, 16, 0x0000beef << 16);
  arith_test(SHR, 0xc0bebeef, 16, 0xc0bebeef >> 16);
  arith_test(ASHR, 0xc0bebeef, 16, 0xc0bebeef >> 16);
  arith_test(NEG, 0, 0xc0bebeef, -1 * 0xc0bebeef);
  arith_test(NOT, 0, 0xc0bebeef, ~0xc0bebeef);

  return 0;
}
