#include "ebcvm.h"

static void init_regs(regs *);
static uint8_t *maybe_fetch_opts(vm *, uint8_t *, size_t);
static uint8_t *maybe_fetch_imms(vm *, uint8_t *);
static uint8_t *fetch_op(vm *);

static void init_regs(regs *_regs) {
  _regs = malloc(sizeof(regs));
  for (int i = 0; i < 10; i++)
    _regs->regs[i] = 0x0;
}

static uint8_t *maybe_fetch_opts(vm *_vm, uint8_t *op, size_t bytes) {
  if (!op)
    goto fail;
  bool is_op1_idx = op[0] & 0x80;
  bool is_op2_idx = op[0] & 0x40;
  size_t opts_len = is_op2_idx ? bytes * 2 : bytes;
  if (is_op1_idx || is_op2_idx) {
    op = realloc(op, sizeof(uint8_t) * (2 + opts_len));
    if (!op)
      goto fail;
    uint64_t ip = _vm->regs->regs[IP];
    for (int i = 2; i < opts_len; i++)
      op[i] = read_mem8(_vm->mem, ip + i);
  }
  return op;

fail:
  error("failed to fetch opts");
  return NULL;
}

static uint8_t *maybe_fetch_imms(vm *_vm, uint8_t *op) {
  if (!op)
    goto fail;
  uint64_t ip = _vm->regs->regs[IP];
  size_t imm_len = 0;
  switch (op[0] & 0xc0) {
    case 1:
      imm_len = 2;
      break;
    case 2:
      imm_len = 4;
      break;
    case 3:
      imm_len = 8;
      break;
    default:
      goto fail;
  }

  size_t mov_len = 0;
  switch (op[1] & 0x30) {
    case 0:
      mov_len = 1;
      break;
    case 1:
      mov_len = 2;
      break;
    case 2:
      mov_len = 4;
      break;
    case 3:
      mov_len = 8;
      break;
    default:
      goto fail;
  }

  int i = 2;
  if (op[1] & 0x80) {
    op = realloc(op, sizeof(uint8_t) * (2 + i));
    if (!op)
      goto fail;
    op[2] = read_mem8(_vm->mem, ip + 2);
    op[3] = read_mem8(_vm->mem, ip + 3);
    i += 2;
  }

  op = realloc(op, sizeof(uint8_t) * (i + imm_len));
  if (!op)
    goto fail;
  for (int k = 0; k < imm_len; k++)
    op[i + k] = read_mem8(_vm->mem, ip + i + k);

  return op;

fail:
  error("failed to fetch imms");
  return NULL;
}

static uint8_t *fetch_op(vm *_vm) {
  uint8_t *op = malloc(sizeof(uint8_t) * 2);
  if (!op)
    goto fail;
  uint64_t ip = _vm->regs->regs[IP];
  op[0] = read_mem8(_vm->mem, ip + 0);
  op[1] = read_mem8(_vm->mem, ip + 1);
  /* XXX: MOVbw to MOVqw */
  if ((op[0] & 0x3f) >= 0x1d && (op[0] & 0x3f) <= 0x1f) {
    op = maybe_fetch_opts(_vm, op, 2);
  /* XXX: MOVbd to MOVqd */
  } else if ((op[0] & 0x3f) >= 0x21 && (op[0] & 0x3f) <= 0x24) {
    op = maybe_fetch_opts(_vm, op, 4);
  /* XXX: MOVqq */
  } else if ((op[0] & 0x3f) == 0x28) {
    op = maybe_fetch_opts(_vm, op, 8);
  } else if ((op[0] & 0x3f) == 0x37) {
    op = maybe_fetch_imms(_vm, op);
  } else if (op[0] & 0x80){
    op = realloc(op, sizeof(uint8_t) * 4);
    if (!op)
      goto fail;
    op[2] = read_mem8(_vm->mem, ip + 2);
    op[3] = read_mem8(_vm->mem, ip + 3);
  }
  return op;

fail:
  error("failed to fetch op");
  return NULL;
}

vm *init_vm() {
  vm *_vm = malloc(sizeof(vm));
  init_regs(_vm->regs);
  init_mem(_vm->mem);
  return _vm;
}

vm *step_inst(vm *_vm) {
  uint8_t *op = fetch_op(_vm);
  inst *_inst = decode_op(op);
  _vm = exec_op(_vm, _inst);
  return _vm;
}
