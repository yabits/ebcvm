#include "ebcvm.h"
#include <assert.h>

static void extndw_test01() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test02() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test03() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x20, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test04() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test05() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, _vm->regs->regs[_inst->operand2], (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test06() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x30, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test07() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = false;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test08() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = false;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x30, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem32(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int32_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test11() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test12() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = (int16_t)-256;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)-128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test13() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x20, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test14() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = (uint16_t)128;
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)128);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test15() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = false;
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, _vm->regs->regs[_inst->operand2], (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test16() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x30, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(_vm->regs->regs[_inst->operand1] == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

static void extndw_test17() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
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

static void extndw_test18() {
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->opcode = EXTNDW;
  _inst->is_imm = true;
  _inst->imm = 0x2002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_64op = true;
  _inst->op2_indirect = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    for (_inst->operand2 = R0; _inst->operand2 <= R7; _inst->operand2++) {
      _vm = init_vm();
      _vm->regs->regs[_inst->operand2] = 0x20;
      write_mem16(_vm->mem, 0x30, (uint16_t)64);
      _vm = exec_op(_vm, _inst);
      assert(read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]) == (int64_t)64);
      fini_vm(_vm);
    }
  }
  free(_inst);
}

int main() {
  extndw_test01();
  extndw_test02();
  extndw_test03();
  extndw_test04();
  extndw_test05();
  extndw_test06();
  extndw_test07();
  extndw_test08();

  extndw_test11();
  extndw_test12();
  extndw_test13();
  extndw_test14();
  extndw_test15();
  extndw_test16();
  extndw_test17();
  extndw_test18();

  return 0;
}
