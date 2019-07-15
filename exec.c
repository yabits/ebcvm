#include "ebcvm.h"

#define DECODE_INDEX(bits)                                          \
static int##bits##_t decode_index##bits(uint##bits##_t index) {     \
  bool s = index >> (bits - 1);                                     \
  uint8_t r = bits - 4;                                             \
  uint8_t a = (index & ((uint##bits##_t)0x7 << r)) >> r;            \
  uint8_t nl = a * (bits / 8);                                      \
  uint8_t cl = bits - (4 + nl);                                     \
  uint##bits##_t nm = 0;                                            \
  uint##bits##_t cm = 0;                                            \
  if (4 + cl != bits)                                               \
    nm = UINT##bits##_MAX >> (4 + cl);                              \
  if (4 + nl != bits)                                               \
    cm = (UINT##bits##_MAX >> (4 + nl)) << nl;                      \
  int##bits##_t n = (s ? -1 : 1) * (index & nm);                    \
  int##bits##_t c = (s ? -1 : 1) * ((index & cm) >> nl);            \
  int##bits##_t offset = (c + n * ARCH_BYTES);                      \
  return offset;                                                    \
}

#define OP(bits)                                                    \
typedef struct op##bits {                                           \
  uint##bits##_t op1;                                               \
  uint##bits##_t op2;                                               \
  uint64_t op1val;                                                  \
  uint64_t op2val;                                                  \
} op##bits;

#define READ_OP(bits)                                               \
static op##bits *read_op##bits(vm *_vm, inst *_inst) {              \
  op##bits *_op##bits = malloc(sizeof(op##bits));                   \
  _op##bits->op2val = _vm->regs->regs[_inst->operand2];             \
  if (_inst->op2_indirect) {                                        \
    if (_inst->is_imm) {                                            \
      _op##bits->op2 = read_mem##bits(_vm->mem,                     \
                  _op##bits->op2val + decode_index16(_inst->imm));  \
    } else                                                          \
      _op##bits->op2 = read_mem##bits(_vm->mem, _op##bits->op2val); \
  } else {                                                          \
    if (_inst->is_imm)                                              \
      _op##bits->op2 = _op##bits->op2val + _inst->imm;              \
    else                                                            \
      _op##bits->op2 = _op##bits->op2val;                           \
  }                                                                 \
  _op##bits->op1val = _vm->regs->regs[_inst->operand1];             \
  if (_inst->op1_indirect)                                          \
    _op##bits->op1 = read_mem##bits(_vm->mem, _op##bits->op1val);   \
  else                                                              \
    _op##bits->op1 = _op##bits->op1val;                             \
  return _op##bits;                                                 \
}

#define EXEC_OP(type, name, op)                                     \
static type op_##name(type op1, type op2) {                         \
  if (!strcmp(#name, "div") || !strcmp(#name, "divu") ||            \
      !strcmp(#name, "mod") || !strcmp(#name, "modu"))              \
    if (op2 == 0)                                                   \
      raise_except(DIV0, "devide by 0", __FILE__, __LINE__);        \
  return op;                                                        \
}                                                                   \
static vm *exec_##name(vm *_vm, inst *_inst) {                      \
  if (_inst->is_64op) {                                             \
    op64 *_op64 = read_op64(_vm, _inst);                            \
    type _op = op_##name(_op64->op1, _op64->op2);                   \
    if (_inst->op1_indirect)                                        \
      write_mem64(_vm->mem, _op64->op1val, _op);                    \
    else                                                            \
      _vm->regs->regs[_inst->operand1] = _op;                       \
    free(_op64);                                                    \
  } else {                                                          \
    op32 *_op32 = read_op32(_vm, _inst);                            \
    type _op = op_##name(_op32->op1, _op32->op2);                   \
    if (_inst->op1_indirect)                                        \
      write_mem32(_vm->mem, _op32->op1val, _op);                    \
    else {                                                          \
      _vm->regs->regs[_inst->operand1] = _op;                       \
      _vm->regs->regs[_inst->operand1] &= ~0xffffffff00000000;      \
    }                                                               \
    free(_op32);                                                    \
  }                                                                 \
  return _vm;                                                       \
}

DECODE_INDEX(16);
DECODE_INDEX(32);
DECODE_INDEX(64);

OP(32);
OP(64);

READ_OP(32);
READ_OP(64);

EXEC_OP(int64_t, add, op1 + op2);
EXEC_OP(int64_t, sub, op1 - op2);
EXEC_OP(int64_t, mul, op1 * op2);
EXEC_OP(int64_t, div, op1 / op2);
EXEC_OP(int64_t, mod, op1 % op2);
EXEC_OP(int64_t, and, op1 & op2);
EXEC_OP(int64_t, or,  op1 | op2);
EXEC_OP(int64_t, xor, op1 ^ op2);
EXEC_OP(uint64_t, shl, op1 << op2);
EXEC_OP(uint64_t, shr, op1 >> op2);
EXEC_OP(int64_t, ashr, op1 >> op2);
EXEC_OP(int64_t, neg, -1 * op2);
EXEC_OP(int64_t, not, ~op2);
EXEC_OP(uint64_t, mulu, op1 * op2);
EXEC_OP(uint64_t, divu, op1 / op2);
EXEC_OP(uint64_t, modu, op1 % op2);

typedef vm *(*arith_op)(vm *, inst *);

arith_op arith_ops[] = {
  exec_add,
  exec_sub,
  exec_mul,
  exec_div,
  exec_mod,
  exec_and,
  exec_or,
  exec_xor,
  exec_shl,
  exec_shr,
  exec_ashr,
  exec_neg,
  exec_not,
  exec_mulu,
  exec_divu,
  exec_modu,
};

static vm *exec_break(vm *_vm, inst *_inst) {
  switch (_inst->break_code) {
    case 1:
      _vm->regs->regs[R7] = (MAJOR_VERSION << 16) + MINOR_VERSION;
      break;
    case 3:
      /* FIXME: debug breakpoint */
      raise_except(DEBUG, "breakpoint", __FILE__, __LINE__);
      break;
    case 4:
      /* FIXME: system call */
      break;
    case 5:
      /* FIXME: create thunk */
      break;
    case 6:
      _vm->compiler_version = _vm->regs->regs[R7];
      break;
    default:
      raise_except(BADBREAK, "bad break", __FILE__, __LINE__);
  }

  return _vm;
}

static vm *exec_jmp(vm *_vm, inst *_inst) {
  bool do_jmp = false;
  if (_inst->is_jmp64) {
    if (!_inst->is_jmp_imm)
      raise_except(ENCODE, "JMP", __FILE__, __LINE__);
    if (_inst->is_cond) {
      if (_inst->is_cs && (_vm->regs->regs[FLAGS] & 0x01))
        do_jmp = true;
      else if (!_inst->is_cs && !(_vm->regs->regs[FLAGS] & 0x01))
        do_jmp = true;
    } else
      do_jmp = true;
    if (do_jmp) {
      if (_inst->jmp_imm % 2)
        raise_except(ALIGN, "alignemnt check", __FILE__, __LINE__);
      if (_inst->is_rel) {
        /* XXX: JMP64 only supports immediate */
        _vm->regs->regs[IP] += 10 + _inst->jmp_imm;
      } else
        _vm->regs->regs[IP] = _inst->jmp_imm;
    }
  } else {
    if (_inst->is_cond) {
      if (_inst->is_cs && (_vm->regs->regs[FLAGS] & 0x01))
        do_jmp = true;
      else if (!_inst->is_cs && !(_vm->regs->regs[FLAGS] & 0x01))
        do_jmp = true;
    } else
      do_jmp = true;
    if (do_jmp) {
      uint64_t op1;
      if (_inst->op1_indirect) {
        if (_inst->is_jmp_imm) {
          op1 = read_mem32(_vm->mem,
              _vm->regs->regs[_inst->operand1]
              + decode_index32((uint32_t)_inst->jmp_imm));
        } else
          op1 = read_mem32(_vm->mem,
              _vm->regs->regs[_inst->operand1]);
      } else {
        if (_inst->operand1 == R0)
          op1 = 0;
        else
          op1 = _vm->regs->regs[_inst->operand1];
        if (_inst->is_jmp_imm)
          op1 += _inst->jmp_imm;
      }
      if (op1 % 2)
        raise_except(ALIGN, "alignemnt check", __FILE__, __LINE__);
      if (_inst->is_rel) {
        size_t inst_len = _inst->is_jmp_imm ? 6 : 2;
        _vm->regs->regs[IP] += inst_len + op1;
      } else
        _vm->regs->regs[IP] = op1;
    }
  }

  if (!do_jmp)
    _vm->regs->regs[IP] += _inst->inst_len;

  return _vm;
}

static vm *exec_jmp8(vm *_vm, inst *_inst) {
  bool do_jmp = false;
  if (_inst->is_cond) {
    if (_inst->is_cs && (_vm->regs->regs[FLAGS] & 0x01))
      do_jmp = true;
    else if (!_inst->is_cs && !(_vm->regs->regs[FLAGS] & 0x01))
      do_jmp = true;
  } else
    do_jmp = true;
  if (do_jmp)
    _vm->regs->regs[IP] += _inst->inst_len  + (int8_t)_inst->jmp_imm * 2;
  else
    _vm->regs->regs[IP] += _inst->inst_len;

  return _vm;
}

static vm *exec_call(vm *_vm, inst *_inst) {
  size_t inst_len = _inst->is_jmp64 ? 10 : (_inst->is_jmp_imm ? 6 : 2);
  /* XXX: PUSH64 return address */
  _vm->regs->regs[R0] -= 16;
  write_mem64(_vm->mem, _vm->regs->regs[R0],
              _vm->regs->regs[IP] + inst_len);

  int64_t op = 0;
  if (_inst->is_jmp64) {
    if (!_inst->is_jmp_imm)
        raise_except(ENCODE, "CALL", __FILE__, __LINE__);
    if (_inst->is_rel)
      raise_except(ENCODE, "CALL", __FILE__, __LINE__);
    op = (uint64_t)_inst->jmp_imm;
    if (_inst->is_native)
      raise_excall(op, _vm);
    else
      _vm->regs->regs[IP] = op;
  } else {
    if (_inst->operand1 != R0) {
      if (_inst->op1_indirect) {
        uint64_t addr = _vm->regs->regs[_inst->operand1];
        if (_inst->is_jmp_imm)
          addr += decode_index32(_inst->jmp_imm);
        op = read_mem64(_vm->mem, addr);
      } else {
        op = _vm->regs->regs[_inst->operand1];
        if (_inst->is_jmp_imm)
          op += (int32_t)_inst->jmp_imm;
      }
    } else
      op = (int32_t)_inst->jmp_imm;
    if (_inst->is_rel) {
      op += _vm->regs->regs[IP];
      if (!_inst->is_native)
        op += inst_len;
    }
  }
  if (_inst->is_native)
    raise_excall((uint64_t)op, _vm);
  else
    _vm->regs->regs[IP] = (uint64_t)op;

  return _vm;
}

static vm *exec_extnd(vm *_vm, inst *_inst) {
  uint64_t op2;
  if (_inst->opcode == EXTNDB) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (uint8_t)read_mem8(_vm->mem,
            _vm->regs->regs[_inst->operand2]
                + decode_index16(_inst->imm));
      } else {
        op2 = (uint8_t)read_mem8(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm) {
        op2 = (uint8_t)_vm->regs->regs[_inst->operand2];
        op2 += (int16_t)_inst->imm;
      } else
        op2 = (uint8_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else if (_inst->opcode == EXTNDW) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (uint16_t)read_mem16(_vm->mem,
            _vm->regs->regs[_inst->operand2]
                + decode_index16(_inst->imm));
      } else {
        op2 = (uint16_t)read_mem16(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm) {
        op2 = (uint16_t)_vm->regs->regs[_inst->operand2];
        op2 += (int16_t)_inst->imm;
      } else
        op2 = (uint16_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else if (_inst->opcode == EXTNDD) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (uint32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2]
                + decode_index16(_inst->imm));
      } else {
        op2 = (uint32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm) {
        op2 = (uint32_t)_vm->regs->regs[_inst->operand2];
        op2 += (int16_t)_inst->imm;
      } else
        op2 = (uint32_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else
    raise_except(ENCODE, "EXTND", __FILE__, __LINE__);

  if (_inst->is_64op) {
    if (_inst->op1_indirect) {
      write_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1], (int64_t)op2);
    } else
      _vm->regs->regs[_inst->operand1] = (int64_t)op2;
  } else {
    if (_inst->op1_indirect) {
      write_mem32(_vm->mem,
          _vm->regs->regs[_inst->operand1], (int32_t)op2);
    } else
      _vm->regs->regs[_inst->operand1] = (int32_t)op2;
  }

  return _vm;
}

static vm *exec_cmp(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    uint64_t op2;
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand2]
            + decode_index16(_inst->imm));
      } else {
        op2 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm)
        op2 = _vm->regs->regs[_inst->operand2] + _inst->imm;
      else
        op2 = _vm->regs->regs[_inst->operand2];
    }
    uint64_t op1 = _vm->regs->regs[_inst->operand1];
    switch (_inst->opcode) {
      case CMPeq:
        if ((int64_t)op1 == (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPlte:
        if ((int64_t)op1 <= (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPgte:
        if ((int64_t)op1 >= (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPulte:
        if ((uint64_t)op1 <= (uint64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPugte:
        if ((uint64_t)op1 >= (uint64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      default:
        raise_except(OPCODE, "CMP", __FILE__, __LINE__);
    }
  } else {
    uint32_t op2;
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2]
            + decode_index16(_inst->imm));
      } else {
        op2 = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm)
        op2 = _vm->regs->regs[_inst->operand2] + _inst->imm;
      else
        op2 = _vm->regs->regs[_inst->operand2];
    }
    uint32_t op1 = _vm->regs->regs[_inst->operand1];
    switch (_inst->opcode) {
      case CMPeq:
        if ((int32_t)op1 == (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPlte:
        if ((int32_t)op1 <= (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPgte:
        if ((int32_t)op1 >= (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPulte:
        if ((uint32_t)op1 <= (uint32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPugte:
        if ((uint32_t)op1 >= (uint32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      default:
        raise_except(OPCODE, "CMP", __FILE__, __LINE__);
    }
  }

  return _vm;
}

static vm *exec_cmpi(vm *_vm, inst *_inst) {
  if (_inst->mov_len == 8) {
    uint64_t op1, op2;
    if (_inst->op1_indirect) {
      if (_inst->is_opt_idx) {
        op1 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->opt_idx));
      } else {
        op1 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      }
    } else {
      if (_inst->is_opt_idx)
        raise_except(ENCODE, "CMPI", __FILE__, __LINE__);
      else
        op1 = _vm->regs->regs[_inst->operand1];
    }
    op2 = (uint64_t)_inst->imm_data;

    switch (_inst->opcode) {
      case CMPIeq:
        if ((int64_t)op1 == (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIlte:
        if ((int64_t)op1 <= (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIgte:
        if ((int64_t)op1 >= (int64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIulte:
        if ((uint64_t)op1 <= (uint64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIugte:
        if ((uint64_t)op1 >= (uint64_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      default:
        raise_except(OPCODE, "CMPI", __FILE__, __LINE__);
    }
  } else if (_inst->mov_len == 4) {
    uint32_t op1, op2;
    if (_inst->op1_indirect) {
      if (_inst->is_opt_idx) {
        op1 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->opt_idx));
      } else {
        op1 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      }
    } else {
      if (_inst->is_opt_idx)
        raise_except(ENCODE, "CMPI", __FILE__, __LINE__);
      else
        op1 = (int32_t)_vm->regs->regs[_inst->operand1];
    }
    op2 = (uint32_t)_inst->imm_data;

    switch (_inst->opcode) {
      case CMPIeq:
        if ((int32_t)op1 == (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIlte:
        if ((int32_t)op1 <= (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIgte:
        if ((int32_t)op1 >= (int32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIulte:
        if ((uint32_t)op1 <= (uint32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      case CMPIugte:
        if ((uint32_t)op1 >= (uint32_t)op2)
          _vm->regs->regs[FLAGS] |= 0x01;
        else
          _vm->regs->regs[FLAGS] &= ~0x01;
        break;
      default:
        raise_except(OPCODE, "CMPI", __FILE__, __LINE__);
    }
  } else {
    raise_except(ENCODE, "CMPI", __FILE__, __LINE__);
  }

  return _vm;
}

static vm *exec_mov(vm *_vm, inst *_inst) {
  uint64_t op;
  if (_inst->op2_indirect) {
    uint64_t op2_addr = _vm->regs->regs[_inst->operand2];
    if (_inst->is_op2_idx) {
      switch (_inst->idx_len) {
        case 2:
          op2_addr += decode_index16(_inst->op2_idx);
          break;
        case 4:
          op2_addr += decode_index32(_inst->op2_idx);
          break;
        case 8:
          op2_addr += decode_index64(_inst->op2_idx);
          break;
        default:
          raise_except(ENCODE, "MOV", __FILE__, __LINE__);
      }
    }
    switch (_inst->op_len) {
      case 1:
        op = read_mem8(_vm->mem, op2_addr);
        break;
      case 2:
        op = read_mem16(_vm->mem, op2_addr);
        break;
      case 4:
        op = read_mem32(_vm->mem, op2_addr);
        break;
      case 8:
        op = read_mem64(_vm->mem, op2_addr);
        break;
      default:
        raise_except(ENCODE, "MOV", __FILE__, __LINE__);
    }
  } else {
      switch (_inst->op_len) {
        case 1:
          op = (uint8_t)_vm->regs->regs[_inst->operand2];
          break;
        case 2:
          op = (uint16_t)_vm->regs->regs[_inst->operand2];
          break;
        case 4:
          op = (uint32_t)_vm->regs->regs[_inst->operand2];
          break;
        case 8:
          op = (uint64_t)_vm->regs->regs[_inst->operand2];
          break;
        default:
          raise_except(ENCODE, "MOV", __FILE__, __LINE__);
      }
    if (_inst->is_op2_idx) {
      switch (_inst->idx_len) {
        case 2:
          op += decode_index16(_inst->op2_idx);
          break;
        case 4:
          op += decode_index32(_inst->op2_idx);
          break;
        case 8:
          op += decode_index64(_inst->op2_idx);
          break;
        default:
          raise_except(ENCODE, "MOV", __FILE__, __LINE__);
      }
    }
  }

  if (_inst->op1_indirect) {
    uint64_t op1_addr = _vm->regs->regs[_inst->operand1];
    if (_inst->is_op1_idx) {
      switch (_inst->idx_len) {
        case 2:
          op1_addr += decode_index16(_inst->op1_idx);
          break;
        case 4:
          op1_addr += decode_index32(_inst->op1_idx);
          break;
        case 8:
          op1_addr += decode_index64(_inst->op1_idx);
          break;
        default:
          raise_except(ENCODE, "MOV", __FILE__, __LINE__);
      }
    }
    switch (_inst->op_len) {
      case 1:
        write_mem8(_vm->mem, op1_addr, (uint8_t)op);
        break;
      case 2:
        write_mem16(_vm->mem, op1_addr, (uint16_t)op);
        break;
      case 4:
        write_mem32(_vm->mem, op1_addr, (uint32_t)op);
        break;
      case 8:
        write_mem64(_vm->mem, op1_addr, (uint64_t)op);
        break;
      default:
        raise_except(ENCODE, "MOV", __FILE__, __LINE__);
    }
  } else {
    if (_inst->is_op1_idx)
      raise_except(ENCODE, "MOV", __FILE__, __LINE__);
    else
      _vm->regs->regs[_inst->operand1] = op;
  }

  return _vm;
}

static vm *exec_movi(vm *_vm, inst *_inst) {
  if (_inst->op1_indirect) {
    uint64_t op = _vm->regs->regs[_inst->operand1];
    if (_inst->is_opt_idx)
      op += decode_index16(_inst->opt_idx);
    switch (_inst->mov_len) {
      case 1:
        write_mem8(_vm->mem, op, (uint8_t)_inst->imm_data);
        break;
      case 2:
        write_mem16(_vm->mem, op, (uint16_t)_inst->imm_data);
        break;
      case 4:
        write_mem32(_vm->mem, op, (uint32_t)_inst->imm_data);
        break;
      case 8:
        write_mem64(_vm->mem, op, (uint64_t)_inst->imm_data);
        break;
      default:
        raise_except(ENCODE, "MOVI", __FILE__, __LINE__);
    }
  } else {
    if (_inst->is_opt_idx)
      raise_except(ENCODE, "MOVI", __FILE__, __LINE__);
    else {
      switch (_inst->mov_len) {
        case 1:
          _vm->regs->regs[_inst->operand1] = (uint8_t)_inst->imm_data;
          break;
        case 2:
          _vm->regs->regs[_inst->operand1] = (uint16_t)_inst->imm_data;
          break;
        case 4:
          _vm->regs->regs[_inst->operand1] = (uint32_t)_inst->imm_data;
          break;
        case 8:
          _vm->regs->regs[_inst->operand1] = (uint64_t)_inst->imm_data;
          break;
        default:
          raise_except(ENCODE, "MOVI", __FILE__, __LINE__);
      }
    }
  }

  return _vm;
}

static vm *exec_movin(vm *_vm, inst *_inst) {
  uint64_t op2;
  switch (_inst->imm_len) {
    case 2:
      op2 = decode_index16(_inst->imm_data);
      break;
    case 4:
      op2 = decode_index32(_inst->imm_data);
      break;
    case 8:
      op2 = decode_index64(_inst->imm_data);
      break;
    default:
      raise_except(ENCODE, "MOVIn", __FILE__, __LINE__);
  }
  if (_inst->op1_indirect) {
    uint64_t op1 = _vm->regs->regs[_inst->operand1];
    if (_inst->is_opt_idx)
      op1 += decode_index16(_inst->opt_idx);
    write_memn(_vm->mem, op1, op2);
  } else {
    if (_inst->is_opt_idx)
      raise_except(ENCODE, "MOVIn", __FILE__, __LINE__);
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_movrel(vm *_vm, inst *_inst) {
  size_t inst_len = 2;
  if (_inst->is_opt_idx)
    inst_len += 2;
  inst_len += _inst->imm_len;

  uint64_t op2 = _vm->regs->regs[IP] + inst_len + _inst->imm_data;
  if (_inst->op1_indirect) {
    uint64_t op1_addr = _vm->regs->regs[_inst->operand1];
    if (_inst->is_opt_idx)
      op1_addr += decode_index16(_inst->opt_idx);
    write_mem64(_vm->mem, op1_addr, op2);
  } else {
    if (_inst->is_opt_idx)
      raise_except(ENCODE, "MOVREL", __FILE__, __LINE__);
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_movn(vm *_vm, inst *_inst) {
  uint64_t op2;
  if (_inst->op2_indirect) {
    uint64_t op2_addr = _vm->regs->regs[_inst->operand2];
    if (_inst->is_op2_idx) {
      switch (_inst->idx_len) {
        case 2:
          op2_addr += decode_index16(_inst->op2_idx);
          break;
        case 4:
          op2_addr += decode_index32(_inst->op2_idx);
          break;
        default:
          raise_except(ENCODE, "MOVn", __FILE__, __LINE__);
      }
    }
    op2 = read_memn(_vm->mem, op2_addr);
  } else {
    op2 = _vm->regs->regs[_inst->operand2];
    if (_inst->is_op2_idx) {
      switch (_inst->idx_len) {
        case 2:
          op2 += decode_index16(_inst->op2_idx);
          break;
        case 4:
          op2 += decode_index32(_inst->op2_idx);
          break;
        default:
          raise_except(ENCODE, "MOVn", __FILE__, __LINE__);
      }
    }
    op2 = uintn(op2);
  }

  if (_inst->op1_indirect) {
    uint64_t op1 = _vm->regs->regs[_inst->operand1];
    if (_inst->is_op1_idx) {
      switch (_inst->idx_len) {
        case 2:
          op1 += decode_index16(_inst->op1_idx);
          break;
        case 4:
          op1 += decode_index32(_inst->op1_idx);
          break;
        default:
          raise_except(ENCODE, "MOVn", __FILE__, __LINE__);
      }
    }
    write_memn(_vm->mem, op1, op2);
  } else {
    if (_inst->is_op1_idx)
      raise_except(ENCODE, "MOVn", __FILE__, __LINE__);
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_movsn(vm *_vm, inst *_inst) {
  int64_t op2;
  if (_inst->op2_indirect) {
    int64_t op2_addr = _vm->regs->regs[_inst->operand2];
    if (_inst->is_op2_idx) {
      switch (_inst->idx_len) {
        case 2:
          op2_addr += decode_index16(_inst->op2_idx);
          break;
        case 4:
          op2_addr += decode_index32(_inst->op2_idx);
          break;
        default:
          raise_except(ENCODE, "MOVsn", __FILE__, __LINE__);
      }
    }
    op2 = read_memn(_vm->mem, (uint64_t)op2_addr);
  } else {
    op2 = _vm->regs->regs[_inst->operand2];
    if (_inst->is_op2_idx)
      op2 += (int64_t)_inst->op2_idx;
  }

  if (_inst->op1_indirect) {
    int64_t op1_addr = _vm->regs->regs[_inst->operand1];
    if (_inst->is_op1_idx) {
      switch (_inst->idx_len) {
        case 2:
          op1_addr += decode_index16(_inst->op1_idx);
          break;
        case 4:
          op1_addr += decode_index32(_inst->op1_idx);
          break;
        default:
          raise_except(ENCODE, "MOVsn", __FILE__, __LINE__);
      }
    }
    write_memn(_vm->mem, (uint64_t)op1_addr, op2);
  } else {
    if (_inst->is_op1_idx)
      raise_except(ENCODE, "MOVsn", __FILE__, __LINE__);
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_pop(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    uint64_t op = read_mem64(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm), op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (int64_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1], op);
      } else {
        _vm->regs->regs[_inst->operand1] = op;
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] + 8;
  } else {
    uint32_t op = read_mem32(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm), op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (int32_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1], op);
      } else {
        _vm->regs->regs[_inst->operand1] = op;
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] + 4;
  }

  return _vm;
}

static vm *exec_popn(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    int64_t op = read_mem64(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm), op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (int64_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1], op);
      } else {
        _vm->regs->regs[_inst->operand1] = op;
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] + ARCH_BYTES;
  } else {
    int32_t op = read_mem32(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm), op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (int32_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1], op);
      } else {
        _vm->regs->regs[_inst->operand1] = op;
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] + ARCH_BYTES;
  }

  return _vm;
}

static vm *exec_push(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    uint64_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm));
      } else {
        op = _vm->regs->regs[_inst->operand1] + (int64_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      } else {
        op = _vm->regs->regs[_inst->operand1];
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] - 8;
    write_mem64(_vm->mem, _vm->regs->regs[R0], op);
  } else {
    uint32_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm));
      } else {
        op = _vm->regs->regs[_inst->operand1] + (int32_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        op = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      } else {
        op = _vm->regs->regs[_inst->operand1];
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] - 4;
    write_mem32(_vm->mem, _vm->regs->regs[R0], op);
  }

  return _vm;
}

static vm *exec_pushn(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    int64_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm));
      } else {
        op = _vm->regs->regs[_inst->operand1] + (int64_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      } else {
        op = _vm->regs->regs[_inst->operand1];
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] - ARCH_BYTES;
    write_mem64(_vm->mem, _vm->regs->regs[R0], op);
  } else {
    int32_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]
            + decode_index16(_inst->imm));
      } else {
        op = _vm->regs->regs[_inst->operand1] + (int32_t)_inst->imm;
      }
    } else {
      if (_inst->op1_indirect) {
        op = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      } else {
        op = _vm->regs->regs[_inst->operand1];
      }
    }
    _vm->regs->regs[R0] = _vm->regs->regs[R0] - ARCH_BYTES;
    write_mem32(_vm->mem, _vm->regs->regs[R0], op);
  }

  return _vm;
}

static vm *exec_ret(vm *_vm, inst *_inst) {
  _vm->regs->regs[IP] = read_mem64(_vm->mem, _vm->regs->regs[R0]);
  _vm->regs->regs[R0] = _vm->regs->regs[R0] + 16;

  if (_vm->regs->regs[R0] % sizeof(uint16_t))
    raise_except(ALIGN, "align", __FILE__, __LINE__);

  if (_vm->regs->regs[IP] == RET_MAGIC)
    raise_except(EXIT, "exit", __FILE__, __LINE__);

  return _vm;
}

static vm *exec_loadsp(vm *_vm, inst *_inst) {
  if (_inst->operand1 <= RV2 && _inst->operand1 >= RV7)
    raise_except(ENCODE, "LOADSP", __FILE__, __LINE__);

  if (_inst->operand1 != FLAGS)
    raise_except(ENCODE, "LOADSP", __FILE__, __LINE__);

  if (_inst->operand1 == FLAGS) {
    /* FIXME: save reserved bits 2..63 */
    _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2];
  }

  return _vm;
}

static vm *exec_storesp(vm *_vm, inst *_inst) {
  switch (_inst->operand2) {
    case FLAGS:
      _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2];
      break;
    case IP:
      /* XXX: according to EbcExecute.c, pass next IP */
      _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2] + 2;
      break;
    default:
      raise_except(ENCODE, "STORESP", __FILE__, __LINE__);
  }

  return _vm;
}

static vm *exec_nop(vm *_vm, inst *_inst) {
  /* do nothing */

  return _vm;
}

static vm *inc_ip(vm *_vm, inst *_inst) {
  _vm->regs->regs[IP] += _inst->inst_len;

  return _vm;
}

vm *exec_op(vm *_vm, inst *_inst) {
  if (_inst->opcode >= ADD && _inst->opcode <= MODU) {
    arith_ops[_inst->opcode - ADD](_vm, _inst);
    goto done_inc;
  }
  if (_inst->opcode >= CMPeq && _inst->opcode <= CMPugte) {
    exec_cmp(_vm, _inst);
    goto done_inc;
  }
  if (_inst->opcode >= CMPIeq && _inst->opcode <= CMPIugte) {
    exec_cmpi(_vm, _inst);
    goto done_inc;
  }
  if (_inst->opcode >= MOVbw && _inst->opcode <= MOVqq) {
    exec_mov(_vm, _inst);
    goto done_inc;
  }
  switch (_inst->opcode) {
    case BREAK:
      exec_break(_vm, _inst);
      goto done_inc;
    case JMP:
      exec_jmp(_vm, _inst);
      goto done_ret;
    case JMP8:
      exec_jmp8(_vm, _inst);
      goto done_ret;
    case CALL:
      exec_call(_vm, _inst);
      goto done_ret;
    case EXTNDB:
    case EXTNDD:
    case EXTNDW:
      exec_extnd(_vm, _inst);
      goto done_inc;
    case MOVI:
      exec_movi(_vm, _inst);
      goto done_inc;
    case MOVIn:
      exec_movin(_vm, _inst);
      goto done_inc;
    case MOVREL:
      exec_movrel(_vm, _inst);
      goto done_inc;
    case MOVnd:
    case MOVnw:
      exec_movn(_vm, _inst);
      goto done_inc;
    case MOVsnw:
    case MOVsnd:
      exec_movsn(_vm, _inst);
      goto done_inc;
    case POP:
      exec_pop(_vm, _inst);
      goto done_inc;
    case POPn:
      exec_popn(_vm, _inst);
      goto done_inc;
    case PUSH:
      exec_push(_vm, _inst);
      goto done_inc;
    case PUSHn:
      exec_pushn(_vm, _inst);
      goto done_inc;
    case RET:
      exec_ret(_vm, _inst);
      goto done_ret;
    case LOADSP:
      exec_loadsp(_vm, _inst);
      goto done_inc;
    case STORESP:
      exec_storesp(_vm, _inst);
      goto done_inc;
    default:
      exec_nop(_vm, _inst);
      goto done_inc;
      /* do nothing */
  }

done_inc:
  inc_ip(_vm, _inst);

done_ret:
  if (_vm->regs->regs[FLAGS] & 0x02)
    raise_except(STEP, "single step", __FILE__, __LINE__);

  return _vm;
}
