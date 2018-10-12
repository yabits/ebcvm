#include <assert.h>
#include "ebcvm.h"

static void break_test() {
  /* BREAK */
  uint8_t *op = malloc(sizeof(uint8_t) * 2);
  inst *_inst;
  for (int i = 0; i < 7; i++) {
    op[0] = 0x00;
    op[1] = 0x00 + i;
    _inst = decode_op(op);
    assert(_inst->opcode == BREAK);
    assert(_inst->break_code == i);
    free(_inst);
  }
  free(op);
}

int main() {
  break_test();

  return 0;
}
