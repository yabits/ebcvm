/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"

static opcode decode_opcode(uint8_t);
static reg decode_gp_reg(uint8_t);
static reg decode_dd_reg(uint8_t);
static inst *decode_jmp(inst *, uint8_t *);
static inst *decode_jmp8(inst *, uint8_t *);
static inst *decode_cmpi(inst *, uint8_t *);
static inst *decode_mov(inst *, uint8_t *);
static inst *decode_movi(inst *, uint8_t *);

opcode ops[] = {
  BREAK,   /* 0x00 */
  JMP,     /* 0x01 */
  JMP8,    /* 0x02 */
  CALL,    /* 0x03 */
  RET,     /* 0x04 */
  CMPeq,   /* 0x05 */
  CMPlte,  /* 0x06 */
  CMPgte,  /* 0x07 */
  CMPulte, /* 0x08 */
  CMPugte, /* 0x09 */
  NOT,     /* 0x0a */
  NEG,     /* 0x0b */
  ADD,     /* 0x0c */
  SUB,     /* 0x0d */
  MUL,     /* 0x0e */
  MULU,    /* 0x0f */
  DIV,     /* 0x10 */
  DIVU,    /* 0x11 */
  MOD,     /* 0x12 */
  MODU,    /* 0x13 */
  AND,     /* 0x14 */
  OR,      /* 0x15 */
  XOR,     /* 0x16 */
  SHL,     /* 0x17 */
  SHR,     /* 0x18 */
  ASHR,    /* 0x19 */
  EXTNDB,  /* 0x1a */
  EXTNDW,  /* 0x1b */
  EXTNDD,  /* 0x1c */
  MOVbw,   /* 0x1d */
  MOVww,   /* 0x1e */
  MOVdw,   /* 0x1f */
  MOVqw,   /* 0x20 */
  MOVbd,   /* 0x21 */
  MOVwd,   /* 0x22 */
  MOVdd,   /* 0x23 */
  MOVqd,   /* 0x24 */
  MOVsnw,  /* 0x25 */
  MOVsnd,  /* 0x26 */
  NOP,     /* 0x27 */
  MOVqq,   /* 0x28 */
  LOADSP,  /* 0x29 */
  STORESP, /* 0x2a */
  PUSH,    /* 0x2b */
  POP,     /* 0x2c */
  CMPIeq,  /* 0x2d */
  CMPIlte, /* 0x2e */
  CMPIgte, /* 0x2f */
  CMPIulte,/* 0x30 */
  CMPIugte,/* 0x31 */
  MOVnw,   /* 0x32 */
  MOVnd,   /* 0x33 */
  NOP,     /* 0x34 */
  PUSHn,   /* 0x35 */
  POPn,    /* 0x36 */
  MOVI,    /* 0x37 */
  MOVIn,   /* 0x38 */
  MOVREL,  /* 0x39 */
  NOP,     /* 0x3a */
  NOP,     /* 0x3b */
  NOP,     /* 0x3c */
  NOP,     /* 0x3d */
  NOP,     /* 0x3e */
  NOP,     /* 0x3f */
};

static opcode decode_opcode(uint8_t _opcode) {
  return ops[_opcode & 0x3f];
}

static reg decode_gp_reg(uint8_t operand) {
  if (operand > 0x07)
    raise_except(ENCODE, "general purpose register", __FILE__, __LINE__);
  /* XXX: R0 is at 8 in reg*/
  return operand + 8;
}

static reg decode_dd_reg(uint8_t operand) {
  /* XXX: allow reserved resigters */
  if (operand > 0x07)
    raise_except(ENCODE, "dedicated register", __FILE__, __LINE__);
  return operand + 0;
}

static bool is_native_opcode(inst *_inst) {
  switch (_inst->opcode) {
    case POPn:
    case PUSHn:
      return true;
    default:
      return false;
  }
}

static inst *decode_jmp(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  _inst->is_jmp_imm = (op[0] & 0x80) ? true : false;
  _inst->is_jmp64   = (op[0] & 0x40) ? true : false;
  _inst->is_cond    = (op[1] & 0x80) ? true : false;
  if (_inst->opcode == JMP)
    _inst->is_cs    = (op[1] & 0x40) ? true : false;
  else if (_inst->opcode == CALL)
    _inst->is_native= (op[1] & 0x20) ? true : false;
  _inst->is_rel     = (op[1] & 0x10) ? true : false;

  if (_inst->is_jmp_imm) {
    _inst->jmp_imm = 0x00;
    int imm_len = _inst->is_jmp64 ? 8 : 4;
    for (int k = 0; k < imm_len; k++)
      _inst->jmp_imm += ((uint64_t)op[2 + k] << (k * 8));
    _inst->inst_len += imm_len;
  }

  return _inst;

fail:
  raise_except(ENCODE, "JMP", __FILE__, __LINE__);
  return NULL;
}

static inst *decode_jmp8(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  _inst->is_jmp_imm = true;
  _inst->is_cond    = (op[0] & 0x80) ? true : false;
  _inst->is_cs      = (op[0] & 0x40) ? true : false;

  _inst->jmp_imm = op[1];

  return _inst;

fail:
  raise_except(ENCODE, "JMP8", __FILE__, __LINE__);
  return NULL;
}

static inst *decode_cmpi(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  _inst->is_opt_idx = (op[1] & 0x10) ? true : false;
  _inst->imm_len    = (op[0] & 0x80) ? 4 : 2;
  _inst->mov_len    = (op[0] & 0x40) ? 8 : 4;

  int i = 2;
  if (_inst->is_opt_idx) {
    _inst->opt_idx = ((uint16_t)op[i] << 0) + ((uint16_t)op[i + 1] << 8);
    i += 2;
    _inst->inst_len += 2;
  }

  _inst->imm_data = 0x00;
  for (int k = 0; k < _inst->imm_len; k++)
    _inst->imm_data += (uint64_t)op[i + k] << (8 * k);
  _inst->inst_len += _inst->imm_len;

  return _inst;

fail:
  raise_except(ENCODE, "CMPI", __FILE__, __LINE__);
  return NULL;
}

static inst *decode_mov_idx(inst *_inst, uint8_t *op, size_t idx_len) {
  _inst->idx_len = idx_len;
  _inst->op1_idx = 0x00;
  _inst->op2_idx = 0x00;
  if (_inst->is_op1_idx) {
    for (int k = 0; k < idx_len; k++)
      _inst->op1_idx += ((uint64_t)op[2 + k] << (k * 8));
    _inst->inst_len += idx_len;
  }
  if (_inst->is_op2_idx) {
    int i = _inst->is_op1_idx ? idx_len + 2 : 2;
    for (int k = 0; k < idx_len; k++)
      _inst->op2_idx += ((uint64_t)op[i + k] << (k * 8));
    _inst->inst_len += idx_len;
  }

  return _inst;
}

static inst *decode_mov(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  if (op[0] & 0x80)
    _inst->is_op1_idx = true;
  else
    _inst->is_op1_idx = false;

  if (op[0] & 0x40)
    _inst->is_op2_idx = true;
  else
    _inst->is_op2_idx = false;

  switch (_inst->opcode) {
    case MOVbw:
    case MOVbd:
      _inst->op_len = 1;
      break;
    case MOVww:
    case MOVwd:
    case MOVnw:
    case MOVsnw:
      _inst->op_len = 2;
      break;
    case MOVdw:
    case MOVdd:
    case MOVnd:
    case MOVsnd:
      _inst->op_len = 4;
      break;
    case MOVqw:
    case MOVqd:
    case MOVqq:
      _inst->op_len = 8;
      break;
    default:
      ;
      /* do nothing */
  }

  switch (_inst->opcode) {
    case MOVbw:
    case MOVww:
    case MOVdw:
    case MOVqw:
    case MOVnw:
    case MOVsnw:
      _inst = decode_mov_idx(_inst, op, 2);
      break;
    case MOVbd:
    case MOVwd:
    case MOVdd:
    case MOVqd:
    case MOVnd:
    case MOVsnd:
      _inst = decode_mov_idx(_inst, op, 4);
      break;
    case MOVqq:
      _inst = decode_mov_idx(_inst, op, 8);
      break;
    default:
      ;
      /* do nothing */
  }

  return _inst;

fail:
  raise_except(ENCODE, "MOV", __FILE__, __LINE__);
  return NULL;
}

static inst *decode_movi(inst *_inst, uint8_t *op) {
  if (!_inst || !op)
    goto fail;

  switch ((op[0] & 0xc0) >> 6) {
    case 0:
      /* Reserved */
      _inst->imm_len = 0;
      break;
    case 1:
      _inst->imm_len = 2;
      break;
    case 2:
      _inst->imm_len = 4;
      break;
    case 3:
      _inst->imm_len = 8;
      break;
    default:
      goto fail;
  }

  if (op[1] & 0x40)
    _inst->is_opt_idx = true;
  else
    _inst->is_opt_idx = false;

  if (_inst->opcode == MOVI) {
    switch ((op[1] & 0x30) >> 4) {
      case 0:
        _inst->mov_len = 1;
        break;
      case 1:
        _inst->mov_len = 2;
        break;
      case 2:
        _inst->mov_len = 4;
        break;
      case 3:
        _inst->mov_len = 8;
        break;
      default:
        goto fail;
    }
  }

  int i = 2;
  if (_inst->is_opt_idx) {
    _inst->opt_idx = ((uint16_t)op[i] << 0) + ((uint16_t)op[i + 1] << 8);
    i += 2;
    _inst->inst_len += 2;
  }

  _inst->imm_data = 0x00;
  for (int k = 0; k < _inst->imm_len; k++)
    _inst->imm_data += (uint64_t)op[i + k] << (8 * k);
  _inst->inst_len += _inst->imm_len;

  return _inst;

fail:
  raise_except(ENCODE, "MOVI", __FILE__, __LINE__);
  return NULL;
}

#ifdef DEBUG_INST
static void print_inst(inst *_inst) {
  char *disas = disas_inst(_inst);
  fprintf(stdout, "%s\n", disas);
  free(disas);
}
#endif /* DEBUG_INST */

inst *decode_op(uint8_t *op) {
  if (!op)
    goto fail;

  inst *_inst = malloc(sizeof(inst));
  if (!_inst)
    goto fail;

  _inst->opcode = decode_opcode(op[0] & 0x3f);
  _inst->inst_len = 2;

  if (_inst->opcode == BREAK) {
    _inst->break_code = op[1];
    goto done;
  } else if (_inst->opcode == JMP || _inst->opcode == CALL) {
    _inst = decode_jmp(_inst, op);
  } else if (_inst->opcode == JMP8) {
    _inst = decode_jmp8(_inst, op);
  } else if (_inst->opcode >= CMPIeq && _inst->opcode <= CMPIugte) {
    _inst = decode_cmpi(_inst, op);
  } else if (_inst->opcode >= MOVbw && _inst->opcode <= MOVqq) {
    _inst = decode_mov(_inst, op);
  } else if (_inst->opcode >= MOVI && _inst->opcode <= MOVREL) {
    _inst = decode_movi(_inst, op);
  } else if (_inst->opcode >= MOVnw && _inst->opcode <= MOVsnd) {
    _inst = decode_mov(_inst, op);
  } else {
    if (op[0] & 0x80)
      _inst->is_imm = true;
    else
      _inst->is_imm = false;
    if (is_native_opcode(_inst)) {
      _inst->is_64op = ARCH_BYTES == 8 ? true : false;
    } else {
      if (op[0] & 0x40)
        _inst->is_64op = true;
      else
        _inst->is_64op = false;
    }

    if (_inst->is_imm) {
      _inst->imm = (op[2] << 0) + (op[3] << 8);
      _inst->inst_len += 2;
    }
  }

  if (op[1] & 0x80)
    _inst->op2_indirect = true;
  else
    _inst->op2_indirect = false;

  if (_inst->opcode == STORESP)
    _inst->operand2 = decode_dd_reg((op[1] & 0x70) >> 4);
  else
    _inst->operand2 = decode_gp_reg((op[1] & 0x70) >> 4);

  if (op[1] & 0x08)
    _inst->op1_indirect = true;
  else
    _inst->op1_indirect = false;

  if (_inst->opcode == LOADSP)
    _inst->operand1 = decode_dd_reg(op[1] & 0x07);
  else
    _inst->operand1 = decode_gp_reg(op[1] & 0x07);

#ifdef DEBUG_INST
  print_inst(_inst);
#endif

done:
  return _inst;

fail:
  raise_except(ENCODE, "inst", __FILE__, __LINE__);
  return NULL;
}
