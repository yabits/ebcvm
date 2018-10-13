#include "ebcvm.h"
#include <assert.h>

static void opecode_test(uint8_t *op, opcode _opcode) {
  inst *_inst = decode_op(op);
  assert(_inst->opcode == _opcode);
  free(_inst);
}

static void ret_test() {
  uint8_t *op = malloc(sizeof(uint8_t) * 2);
  op[0] = 0x04;
  op[1] = 0x00;

  opecode_test(op, RET);
  free(op);
}

int main() {
  ret_test();

  return 0;
}
