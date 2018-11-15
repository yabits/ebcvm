#include "ebcvm.h"
#include <assert.h>

static void call_test01() {
  /*
   * is_jmp_imm = false
   * is_rel = false
   * op1_indirect = false
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x01234560;
    _vm->regs->regs[_inst->operand1] = 0x01234567;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234562);
    assert(_vm->regs->regs[IP] == 0x01234567);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test02() {
  /*
   * is_jmp_imm = true
   * is_rel = false
   * op1_indirect = false
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x00004567;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = false;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R0; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x01234560;
    if (_inst->operand1 != R0)
      _vm->regs->regs[_inst->operand1] = 0x01230000;
    _vm = exec_op(_vm, _inst);
    if (_inst->operand1 != R0) {
      assert(_vm->regs->regs[R0] == 0x18);
      assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234560 + 6);
      assert(_vm->regs->regs[IP] == 0x01234567);
    } else {
      assert(_vm->regs->regs[R0] == 0x18);
      assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234560 + 6);
      assert(_vm->regs->regs[IP] == 0x00004567);
    }
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test03() {
  /*
   * is_jmp_imm = false
   * is_rel = true
   * op1_indirect = false
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x45670000;
    _vm->regs->regs[_inst->operand1] = 0x00001230;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x45670002);
    assert(_vm->regs->regs[IP] == 0x45671232);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test04() {
  /*
   * is_jmp_imm = false
   * is_rel = false
   * op1_indirect = true
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = false;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x01234560;
    _vm->regs->regs[_inst->operand1] = 0x30;
    write_mem32(_vm->mem, 0x30, 0x89abcdef);
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234562);
    assert(_vm->regs->regs[IP] == 0x89abcdef);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test05() {
  /*
   * is_jmp_imm = true
   * is_rel = true
   * op1_indirect = false
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x00001200;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x45670000;
    _vm->regs->regs[_inst->operand1] = 0x00000030;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x45670006);
    assert(_vm->regs->regs[IP] == 0x45671236);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test06() {
  /*
   * is_jmp_imm = true
   * is_rel = false
   * op1_indirect = true
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x20000002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = false;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x01234560;
    _vm->regs->regs[_inst->operand1] = 0x30;
    write_mem32(_vm->mem, 0x40, 0x89abcdef);
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x01234566);
    assert(_vm->regs->regs[IP] == 0x89abcdef);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test07() {
  /*
   * is_jmp_imm = true
   * is_rel = true
   * op1_indirect = false
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x00001200;
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = true;
  _inst->op1_indirect = false;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x45670000;
    _vm->regs->regs[_inst->operand1] = 0x00000030;
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x45670006);
    assert(_vm->regs->regs[IP] == 0x45671236);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test08() {
  /*
   * is_jmp_imm = true
   * is_rel = true
   * op1_indirect = true
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x20000002; /* XXX: in 64-bit, it means +0x10 */
  _inst->is_jmp64 = false;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = true;
  _inst->op1_indirect = true;

  for (_inst->operand1 = R1; _inst->operand1 <= R7; _inst->operand1++) {
    _vm = init_vm();
    _vm->regs->regs[R0] = 0x20;
    _vm->regs->regs[IP] = 0x45670000;
    _vm->regs->regs[_inst->operand1] = 0x30;
    write_mem32(_vm->mem, 0x40, 0x00001230);
    _vm = exec_op(_vm, _inst);
    assert(_vm->regs->regs[R0] == 0x18);
    assert(read_mem32(_vm->mem, _vm->regs->regs[R0]) == 0x45670006);
    assert(_vm->regs->regs[IP] == 0x45671236);
    fini_vm(_vm);
  }
  free(_inst);
}

static void call_test09() {
  /*
   * is_jmp_imm = true
   * is_jmp64 = true
   */
  vm *_vm;
  inst *_inst = malloc(sizeof(inst));
  _inst->is_jmp_imm = true;
  _inst->jmp_imm = 0x0123456789abcdef;
  _inst->is_jmp64 = true;
  _inst->opcode = CALL;
  _inst->is_native = false;
  _inst->is_rel = false;
  _inst->op1_indirect = false;

  _vm = init_vm();
  _vm->regs->regs[R0] = 0x20;
  _vm->regs->regs[IP] = 0xfedcba9876543210;
  _vm = exec_op(_vm, _inst);
  assert(_vm->regs->regs[IP] == 0x0123456789abcdef);
  fini_vm(_vm);

  free(_inst);
}

int main() {
  call_test01();
  call_test02();
  call_test03();
  call_test04();
  call_test05();
  call_test06();
  call_test07();
  call_test08();
  call_test09();

  return 0;
}
