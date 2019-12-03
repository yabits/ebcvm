/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"
#include <assert.h>

static void cmp_test01() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPeq;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = 0xffffffff;
      _vm->regs->regs[_inst->operand2] = 0xffffffff;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = 0xffffffff;
      _vm->regs->regs[_inst->operand2] = 0xfffffffe;
      _vm = exec_op(_vm, _inst);
      assert(!(_vm->regs->regs[FLAGS] & 0x01));
      fini_vm(_vm);
    }
  }
}

static void cmp_test02() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPlte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPlte;
  _inst->op2_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)-128;
      _vm->regs->regs[_inst->operand2] = (int32_t)256;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)128;
      _vm->regs->regs[_inst->operand2] = (int32_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)128;
      _vm->regs->regs[_inst->operand2] = (int32_t)-256;
      _vm = exec_op(_vm, _inst);
      assert(!(_vm->regs->regs[FLAGS] & 0x01));
      fini_vm(_vm);
    }
  }
}

static void cmp_test03() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPgte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPgte;
  _inst->op2_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)128;
      _vm->regs->regs[_inst->operand2] = (int32_t)-256;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)-128;
      _vm->regs->regs[_inst->operand2] = (int32_t)-128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (int32_t)-128;
      _vm->regs->regs[_inst->operand2] = (int32_t)256;
      _vm = exec_op(_vm, _inst);
      assert(!(_vm->regs->regs[FLAGS] & 0x01));
      fini_vm(_vm);
    }
  }
}

static void cmp_test04() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPulte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPulte;
  _inst->op2_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)128;
      _vm->regs->regs[_inst->operand2] = (uint32_t)256;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)128;
      _vm->regs->regs[_inst->operand2] = (uint32_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)256;
      _vm->regs->regs[_inst->operand2] = (uint32_t)128;
      _vm = exec_op(_vm, _inst);
      assert(!(_vm->regs->regs[FLAGS] & 0x01));
      fini_vm(_vm);
    }
  }
}

static void cmp_test05() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPugte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPugte;
  _inst->op2_indirect = false;
  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)256;
      _vm->regs->regs[_inst->operand2] = (uint32_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)128;
      _vm->regs->regs[_inst->operand2] = (uint32_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[FLAGS] & 0x01);
      fini_vm(_vm);
    }
  }

  for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
    for (_inst->operand1 = _inst->operand2 + 1; _inst->operand1 <= R7; _inst->operand1++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand1] = (uint32_t)128;
      _vm->regs->regs[_inst->operand2] = (uint32_t)256;
      _vm = exec_op(_vm, _inst);
      assert(!(_vm->regs->regs[FLAGS] & 0x01));
      fini_vm(_vm);
    }
  }
}

static void cmp_test06() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPeq;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0xffffffffffffffff;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0xfffffffffffffffe;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test07() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPlte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPlte;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int64_t)-128;
    _vm->regs->regs[_inst->operand2] = (int64_t)256;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);

    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int64_t)-128;
    _vm->regs->regs[_inst->operand2] = (int64_t)-128;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[FLAGS] & 0x01);
    fini_vm(_vm);

    _vm = init_vm();
    _vm->regs->regs[_inst->operand1] = (int64_t)128;
    _vm->regs->regs[_inst->operand2] = (int64_t)-256;
    _vm = exec_op(_vm, _inst);
    assert(!(_vm->regs->regs[FLAGS] & 0x01));
    fini_vm(_vm);
}

static void cmp_test08() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPgte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPgte;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (int64_t)128;
  _vm->regs->regs[_inst->operand2] = (int64_t)-256;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (int64_t)-128;
  _vm->regs->regs[_inst->operand2] = (int64_t)-128;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (int64_t)-128;
  _vm->regs->regs[_inst->operand2] = (int64_t)256;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test09() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPulte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPulte;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)128;
  _vm->regs->regs[_inst->operand2] = (uint64_t)256;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)128;
  _vm->regs->regs[_inst->operand2] = (uint64_t)128;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)256;
  _vm->regs->regs[_inst->operand2] = (uint64_t)128;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0a() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPugte;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPugte;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)256;
  _vm->regs->regs[_inst->operand2] = (uint64_t)128;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)128;
  _vm->regs->regs[_inst->operand2] = (uint64_t)128;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = (uint64_t)128;
  _vm->regs->regs[_inst->operand2] = (uint64_t)256;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0b() {
  /*
   * is_imm = true;
   * imm = 0xabcd;
   * is_64op = false;
   * opcode = CMPeq;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->imm = 0xabcd;
  _inst->is_64op = false;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffabcd;
  _vm->regs->regs[_inst->operand2] = 0xffff0000;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffff;
  _vm->regs->regs[_inst->operand2] = 0xffff0000;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0c() {
  /*
   * is_imm = false;
   * is_64op = false;
   * opcode = CMPeq;
   * op2_indirect = true;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem32(_vm->mem, 0x10, 0xffffffff);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem32(_vm->mem, 0x10, 0xfffffffe);
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0d() {
  /*
   * is_imm = true;
   * imm = 0x2002;
   * is_64op = false;
   * opcode = CMPeq;
   * op2_indirect = true;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = false;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem32(_vm->mem, 0x20, 0xffffffff);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem32(_vm->mem, 0x20, 0xfffffffe);
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0e() {
  /*
   * is_imm = true;
   * imm = 0xabcd;
   * is_64op = true;
   * opcode = CMPeq;
   * op2_indirect = false;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->imm = 0xabcd;
  _inst->is_64op = true;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = false;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffabcd;
  _vm->regs->regs[_inst->operand2] = 0xffffffffffff0000;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0xffffffffffff0000;
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test0f() {
  /*
   * is_imm = false;
   * is_64op = true;
   * opcode = CMPeq;
   * op2_indirect = true;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x10, 0xffffffffffffffff);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x10, 0xfffffffffffffffe);
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

static void cmp_test10() {
  /*
   * is_imm = true;
   * imm = 0x2002;
   * is_64op = true;
   * opcode = CMPeq;
   * op2_indirect = true;
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = true;
  _inst->opcode = CMPeq;
  _inst->op2_indirect = true;
  _inst->operand1 = R0;
  _inst->operand2 = R1;

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x20, 0xffffffffffffffff);
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[FLAGS] & 0x01);
  fini_vm(_vm);

  _vm = init_vm();
  _vm->regs->regs[_inst->operand1] = 0xffffffffffffffff;
  _vm->regs->regs[_inst->operand2] = 0x10;
  write_mem64(_vm->mem, 0x20, 0xfffffffffffffffe);
  _vm = exec_op(_vm, _inst);
  assert(!(_vm->regs->regs[FLAGS] & 0x01));
  fini_vm(_vm);
}

int main() {
  cmp_test01();
  cmp_test02();
  cmp_test03();
  cmp_test04();
  cmp_test05();
  cmp_test06();
  cmp_test07();
  cmp_test08();
  cmp_test09();
  cmp_test0a();
  cmp_test0b();
  cmp_test0c();
  cmp_test0d();
  cmp_test0e();
  cmp_test0f();
  cmp_test10();

  return 0;
}
