#include "ebcvm.h"
#include <assert.h>

static void extndb_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x20, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, _vm->regs->regs[_inst->operand2], (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x30, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x30, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test11() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test12() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test13() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x20, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test14() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test15() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, _vm->regs->regs[_inst->operand2], (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test16() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x30, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test17() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint8_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndb_test18() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDB;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem8(_vm->mem, 0x30, (uint8_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

int main() {
  extndb_test01();
  extndb_test02();
  extndb_test03();
  extndb_test04();
  extndb_test05();
  extndb_test06();
  extndb_test07();
  extndb_test08();

  extndb_test11();
  extndb_test12();
  extndb_test13();
  extndb_test14();
  extndb_test15();
  extndb_test16();
  extndb_test17();
  extndb_test18();

  return 0;
}
