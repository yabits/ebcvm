#include <assert.h>
#include "ebcvm.h"

void break_test(void);

void break_test() {
  /* BREAK */
  for (int i = 0; i < 7; i++) {
    uint8_t *op = malloc(sizeof(uint8_t) * 2);
    op[0] = 0x00;
    op[1] = 0x00 + i;
    inst *_inst = decode_op(op);
    assert(_inst->opcode == BREAK);
    assert(_inst->break_code == i);
  }
}

int main() {
  break_test();
}
