#include "ebcvm.h"

static regs *init_regs(void);
static size_t maybe_fetch_opts(vm *, uint8_t **, size_t);
static size_t maybe_fetch_imms(vm *, uint8_t **);
static size_t maybe_fetch_jmp_imms(vm *, uint8_t **);
static size_t maybe_fetch_cmpi_imms(vm *, uint8_t **);
static size_t fetch_op(vm *, uint8_t **);

static regs *init_regs() {
  regs *_regs = malloc(sizeof(regs));
  for (int i = 0; i < 16; i++)
    _regs->regs[i] = 0x00000000000000000;

  /* XXX: set single-step flag */
  if (FLAGS_step)
    _regs->regs[FLAGS] |= 0x02;

  return _regs;
}

static size_t maybe_fetch_opts(vm *_vm, uint8_t **op, size_t bytes) {
  if (!*op)
    goto fail;
  bool is_op1_idx = (*op)[0] & 0x80;
  bool is_op2_idx = (*op)[0] & 0x40;
  size_t opts_len = 0;
  opts_len += is_op1_idx ? bytes : 0;
  opts_len += is_op2_idx ? bytes : 0;
  if (is_op1_idx || is_op2_idx) {
    *op = realloc(*op, sizeof(uint8_t) * (2 + opts_len));
    if (!*op)
      goto fail;
    uint64_t ip = _vm->regs->regs[IP];
    for (int i = 0; i < opts_len; i++)
      (*op)[2 + i] = read_mem8(_vm->mem, ip + 2 + i);
  }
  return 2 + opts_len;

fail:
  raise_except(UNDEF, "fetch opts");
  return 0;
}

static size_t maybe_fetch_imms(vm *_vm, uint8_t **op) {
  if (!*op)
    goto fail;
  uint64_t ip = _vm->regs->regs[IP];
  size_t imm_len = 0;
  switch ((*op)[0] & 0xc0 >> 6) {
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

  int i = 2;
  if ((*op)[1] & 0x80) {
    *op = realloc(*op, sizeof(uint8_t) * (2 + i));
    if (!*op)
      goto fail;
    (*op)[2] = read_mem8(_vm->mem, ip + 2);
    (*op)[3] = read_mem8(_vm->mem, ip + 3);
    i += 2;
  }

  *op = realloc(*op, sizeof(uint8_t) * (i + imm_len));
  if (!*op)
    goto fail;
  for (int k = 0; k < imm_len; k++)
    (*op)[i + k] = read_mem8(_vm->mem, ip + i + k);

  return i + imm_len;

fail:
  raise_except(UNDEF, "fetch imms");
  return 0;
}

static size_t maybe_fetch_jmp_imms(vm *_vm, uint8_t **op) {
  if (!*op)
    goto fail;
  int i = 2;
  uint64_t ip = _vm->regs->regs[IP];
  int imm_len = (*op)[0] & 0x40 ? 8 : 4;
  if ((*op)[0] & 0x80) {
    *op = realloc(*op, sizeof(uint8_t) * (i + imm_len));
    if (!*op)
      goto fail;
    for (int k = 0; k < imm_len; k++)
      (*op)[i + k] = read_mem8(_vm->mem, ip + i + k);
  }

  return 2 + imm_len;

fail:
  raise_except(UNDEF, "fetch jmp imms");
  return 0;
}

static size_t maybe_fetch_cmpi_imms(vm *_vm, uint8_t **op) {
  if (!*op)
    goto fail;
  uint64_t ip = _vm->regs->regs[IP];
  int i = 2;
  if (*op[1] & 0x10) {
    *op = realloc(*op, sizeof(uint8_t) * (2 + i));
    if (!*op)
      goto fail;
    (*op)[2] = read_mem8(_vm->mem, ip + 2);
    (*op)[3] = read_mem8(_vm->mem, ip + 3);
    i += 2;
  }

  size_t imm_len = ((*op)[0] & 0x80) ? 4 : 2;
  for (int k = 0; k < imm_len; k++)
    (*op)[i + k] = read_mem8(_vm->mem, ip + i + k);

  return i + imm_len;

fail:
  raise_except(UNDEF, "fetch cmpi imms");
  return 0;
}

static size_t fetch_op(vm *_vm, uint8_t **op) {
  *op = malloc(sizeof(uint8_t) * 2);
  if (!*op)
    goto fail;
  uint64_t ip = _vm->regs->regs[IP];
  (*op)[0] = read_mem8(_vm->mem, ip + 0);
  (*op)[1] = read_mem8(_vm->mem, ip + 1);
  size_t op_len = 2;
  if (((*op)[0] & 0x3f) == 0x01 || ((*op)[0] & 0x3f) == 0x03) {
    /* XXX: CALL or JMP */
    op_len = maybe_fetch_jmp_imms(_vm, op);
  } else if (((*op)[0] & 0x3f) >= 0x2d && ((*op)[0] & 0x3f) <= 0x31) {
    /* XXX: CMPI */
    op_len = maybe_fetch_cmpi_imms(_vm, op);
  } else if (((*op)[0] & 0x3f) >= 0x1d && ((*op)[0] & 0x3f) <= 0x20) {
    /* XXX: MOVbw to MOVqw */
    op_len = maybe_fetch_opts(_vm, op, 2);
  } else if (((*op)[0] & 0x3f) >= 0x21 && ((*op)[0] & 0x3f) <= 0x24) {
    /* XXX: MOVbd to MOVqd */
    op_len = maybe_fetch_opts(_vm, op, 4);
  } else if (((*op)[0] & 0x3f) == 0x28) {
    /* XXX: MOVqq */
    op_len = maybe_fetch_opts(_vm, op, 8);
  } else if (((*op)[0] & 0x3f) >= 0x37 && ((*op)[0] & 0x3f) <= 0x39) {
    /* XXX: MOVI, MOVIn, or MOVREL */
    op_len = maybe_fetch_imms(_vm, op);
  } else if (((*op)[0] & 0x3f) == 0x32 || ((*op)[0] & 0x3f) == 0x25) {
    /* XXX: MOVnw or MOVsnw */
    op_len = maybe_fetch_opts(_vm, op, 2);
  } else if (((*op)[0] & 0x3f) == 0x33 || ((*op)[0] & 0x3f) == 0x26) {
    /* XXX: MOVnd or MOVsnw */
    op_len = maybe_fetch_opts(_vm, op, 4);
  } else if ((*op)[0] & 0x80){
    *op = realloc(*op, sizeof(uint8_t) * 4);
    if (!*op)
      goto fail;
    (*op)[2] = read_mem8(_vm->mem, ip + 2);
    (*op)[3] = read_mem8(_vm->mem, ip + 3);
    op_len = 4;
  }
  return op_len;

fail:
  raise_except(UNDEF, "fetch op");
  return 0;
}

vm *init_vm() {
  vm *_vm = malloc(sizeof(vm));
  _vm->regs = init_regs();
  _vm->mem = init_mem();
  _vm->memmap = NULL;
  return _vm;
}

void fini_vm(vm *_vm) {
  free(_vm->regs);
  fini_mem(_vm->mem);
  free(_vm);
}

vm *step_inst(vm *_vm) {
  uint8_t *op = NULL;
  fetch_op(_vm, &op);
  inst *_inst = decode_op(op);
  _vm = exec_op(_vm, _inst);

  free(op);
  free(_inst);

  return _vm;
}

void exec_vm(vm *_vm) {
  if (FLAGS_debug)
    raise_except(DEBUG, "debug");
  while (true) {
    step_inst(_vm);
  }
}

size_t dump_inst(vm *_vm) {
  uint8_t *op = NULL;
  size_t op_len = fetch_op(_vm, &op);
  inst *_inst = decode_op(op);

  char *disas = disas_inst(_inst);

  fprintf(stdout, "0x%016llx:\t", _vm->regs->regs[IP]);
  for (int i = 0; i < op_len; i++) {
    if (i > 0)
      fprintf(stdout, " ");
    fprintf(stdout, "%02x", op[i]);
  }
  fprintf(stdout, "\t%s\n", disas);

  free(op);
  free(_inst);

  return op_len;
}

void dump_vm(vm *_vm) {
  /* XXX: we assume entry point is the address of text section */
  uint64_t text_end = _vm->regs->regs[IP];
  for (int i = 0; i < _vm->memmap_size; i++) {
    if (_vm->memmap[i].mem_type == MEM_TEXT) {
      text_end = _vm->memmap[i].addr + _vm->memmap[i].size;
      break;
    }
  }
  while (_vm->regs->regs[IP] < text_end) {
    size_t op_len = dump_inst(_vm);

    /* move IP */
    _vm->regs->regs[IP] += op_len;
  }
}

void raise_except(except _except, const char *str) {
  char *exceptions[] = {
    "DIV0",
    "DEBUG",
    "STEP",
    "OPCODE",
    "STACK",
    "ALIGN",
    "ENCODE",
    "BADBREAK",
    "EXIT",
    "UNDEF",
  };

  if (FLAGS_debug)
    handle_except(_dbg, _except, str);
  else {
    switch (_except) {
      case DIV0:
      case OPCODE:
      case STACK:
      case ALIGN:
      case ENCODE:
      case UNDEF:
        error("exception %s: %s\n", exceptions[_except], str);
        break;
      case EXIT:
        exit(0);
        break;
      default:
        ; /* XXX: DEBUG and STEP is ignored */
    }
  }
}

void raise_excall(uint64_t addr, vm *_vm) {
  if (FLAGS_debug)
    raise_except(DEBUG, "excall");

  handle_excall(addr, _vm);
}
