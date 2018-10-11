#include "ebcvm.h"

#define OP(bits)          \
typedef struct op##bits { \
  uint##bits##_t op1;     \
  uint##bits##_t op2;     \
  uint64_t op1val;        \
  uint64_t op2val;        \
} op##bits;

#define READ_OP(bits)                                   \
static op##bits *read_op##bits(vm *_vm, inst *_inst) {  \
  op##bits *_op##bits = malloc(sizeof(op##bits));       \
  _op##bits->op2val = _vm->regs->regs[_inst->operand2]; \
  if (_inst->op2_indirect)                              \
    _op##bits->op2 = read_mem##bits(_vm->mem,           \
                  _op##bits->op2val + _inst->imm);      \
  else                                                  \
    _op##bits->op2 = _op##bits->op2val + _inst->imm;    \
  _op##bits->op1val = _vm->regs->regs[_inst->operand1]; \
  if (_inst->op1_indirect)                              \
    _op##bits->op1 = read_mem##bits(_vm->mem,           \
                  _op##bits->op1val);                   \
  else                                                  \
    _op##bits->op1 = _op##bits->op1val;                 \
  return _op##bits;                                     \
}

#define EXEC_OP(type, name, op)                                     \
static type _##name(type op1, type op2) {                           \
  return op;                                                        \
}                                                                   \
static vm *exec_##name(vm *_vm, inst *_inst) {                      \
  if (_inst->is_64op) {                                             \
    op64 *_op64 = read_op64(_vm, _inst);                            \
    type _op = _##name(_op64->op1, _op64->op2);                     \
    if (_inst->op1_indirect)                                        \
      write_mem64(_vm->mem, _op64->op1val, _op);                    \
    else                                                            \
      _vm->regs->regs[_inst->operand1] = _op;                       \
    free(_op64);                                                    \
  } else {                                                          \
    op32 *_op32 = read_op32(_vm, _inst);                            \
    type _op = (type)_##name(_op32->op1, _op32->op2);               \
    if (_inst->op1_indirect)                                        \
      write_mem32(_vm->mem, _op32->op1val, _op);                    \
    else                                                            \
      _vm->regs->regs[_inst->operand1] = (uint64_t)0x00 << 32 & _op;\
    free(_op32);                                                    \
  }                                                                 \
  return _vm;                                                       \
}

OP(32);
OP(64);

READ_OP(32);
READ_OP(64);

EXEC_OP(int64_t, add, (op1 + op2));
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
  NULL,
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
      error("debug breakpoint");
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
      error("bad break exception");
  }

  return _vm;
}

static vm *exec_jmp(vm *_vm, inst *_inst) {
  bool do_jmp = false;
  if (_inst->is_jmp64) {
    if (!_inst->jmp_imm)
      error("invalid instruction");
    if (_inst->is_cond) {
      if (_inst->is_cs && (_vm->regs->regs[IP] & 0x01))
        do_jmp = true;
      else if (!_inst->is_cs && !(_vm->regs->regs[IP] & 0x01))
        do_jmp = true;
    } else
      do_jmp = true;
    if (do_jmp) {
      if (_inst->jmp_imm % 2)
        error("alignemnt check");
      if (_inst->is_rel) {
        /* XXX: JMP64 only supports immediate */
        _vm->regs->regs[IP] += 10 + _inst->jmp_imm;
      } else
        _vm->regs->regs[IP] = _inst->jmp_imm;
    }
  } else {
    if (_inst->is_cond) {
      if (_inst->is_cs && (_vm->regs->regs[IP] & 0x01))
        do_jmp = true;
      else if (!_inst->is_cs && !(_vm->regs->regs[IP] & 0x01))
        do_jmp = true;
    } else
      do_jmp = true;
    if (do_jmp) {
      uint64_t op1;
      if (_inst->op1_indirect) {
        if (_inst->is_jmp_imm) {
          op1 = read_mem64(_vm->mem,
              _vm->regs->regs[_inst->operand1] + _inst->jmp_imm);
        } else
          op1 = read_mem64(_vm->mem,
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
        error("alignemnt check");
      if (_inst->is_rel) {
        size_t inst_len = _inst->is_jmp_imm ? 6 : 2;
        _vm->regs->regs[IP] += inst_len + op1;
      } else
        _vm->regs->regs[IP] = op1;
    }
  }

  return _vm;
}

static vm *exec_jmp8(vm *_vm, inst *_inst) {
  bool do_jmp = false;
  if (_inst->is_cond) {
    if (_inst->is_cs && (_vm->regs->regs[IP] & 0x01))
      do_jmp = true;
    else if (!_inst->is_cs && !(_vm->regs->regs[IP] & 0x01))
      do_jmp = true;
  } else
    do_jmp = true;
  if (do_jmp)
    _vm->regs->regs[IP] += 2 + (int8_t)_inst->jmp_imm * 2;

  return _vm;
}

static vm *exec_call(vm *_vm, inst *_inst) {
  _vm->regs->regs[R0] -= 8;

  /* XXX: PUSH64 return address */
  _vm->regs->regs[R0] -= 8;
  write_mem64(_vm->mem,
      _vm->regs->regs[R0], _vm->regs->regs[IP] + 0x80);

  if (_inst->is_jmp64) {
    if (_inst->is_native)
      ; /* FIXME: call to native code */
    else
      _vm->regs->regs[IP] = _inst->jmp_imm;
  } else {
    uint64_t op;
    size_t inst_len = _inst->is_jmp_imm ? 6 : 2;
    if (_inst->operand1 != R0) {
      if (_inst->op1_indirect)
        op = read_mem64(_vm->mem, _vm->regs->regs[_inst->operand1]);
      else
        op = _inst->operand1;
    } else
      op = (uint32_t)_inst->jmp_imm;
    if (_inst->is_native) {
      if (_inst->is_rel)
        ; /* FIXME: call to native code IP + op */
      else
        ; /* FIXME: call to native code op */
    } else {
      if (_inst->is_rel)
        _vm->regs->regs[IP] += op + inst_len;
      else
        _vm->regs->regs[IP] = op;
    }
  }

  return _vm;
}

static vm *exec_extnd(vm *_vm, inst *_inst) {
  int64_t op2;
  if (_inst->opcode == EXTNDB) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (int8_t)read_mem8(_vm->mem,
            _vm->regs->regs[_inst->operand2] + _inst->imm);
      } else {
        op2 = (int8_t)read_mem8(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm)
        op2 = (int8_t)(_vm->regs->regs[_inst->operand2] + _inst->imm);
      else
        op2 = (int8_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else if (_inst->opcode == EXTNDW) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (int16_t)read_mem16(_vm->mem,
            _vm->regs->regs[_inst->operand2] + _inst->imm);
      } else {
        op2 = (int16_t)read_mem16(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm)
        op2 = (int16_t)(_vm->regs->regs[_inst->operand2] + _inst->imm);
      else
        op2 = (int16_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else if (_inst->opcode == EXTNDD) {
    if (_inst->op2_indirect) {
      if (_inst->is_imm) {
        op2 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2] + _inst->imm);
      } else {
        op2 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand2]);
      }
    } else {
      if (_inst->is_imm)
        op2 = (int32_t)(_vm->regs->regs[_inst->operand2] + _inst->imm);
      else
        op2 = (int32_t)(_vm->regs->regs[_inst->operand2]);
    }
  } else
    error("invalid instruction");

  if (_inst->is_64op) {
    if (_inst->op1_indirect)
      write_mem64(_vm->mem, _vm->regs->regs[_inst->operand1], (int64_t)op2);
    else
      _vm->regs->regs[_inst->operand1] = (int64_t)op2;
  } else {
    if (_inst->op1_indirect)
      write_mem32(_vm->mem, _vm->regs->regs[_inst->operand1], (int32_t)op2);
    else
      _vm->regs->regs[_inst->operand1] = (int32_t)op2;
  }

  return _vm;
}

static vm *exec_cmp(vm *_vm, inst *_inst) {
  uint64_t op2;
  if (_inst->op2_indirect) {
    if (_inst->is_imm) {
      op2 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand2] + _inst->imm);
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
      break;
    case CMPlte:
      if ((int64_t)op1 <= (int64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPgte:
      if ((int64_t)op1 >= (int64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPulte:
      if ((uint64_t)op1 <= (uint64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPugte:
      if ((uint64_t)op1 >= (uint64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    default:
      error("invalid instruction");
  }

  return _vm;
}

static vm *exec_cmpi(vm *_vm, inst *_inst) {
  int64_t op1, op2;
  if (_inst->mov_len == 8) {
    if (_inst->op1_indirect) {
      if (_inst->is_opt_idx) {
        op1 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->opt_idx);
      } else {
        op1 = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      }
    } else {
      if (_inst->is_opt_idx)
        error("invalid instruction");
      else
        op1 = _vm->regs->regs[_inst->operand1];
    }
    op2 = _inst->imm_data;
  } else if (_inst->mov_len == 4) {
    if (_inst->op1_indirect) {
      if (_inst->is_opt_idx) {
        op1 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->opt_idx);
      } else {
        op1 = (int32_t)read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1]);
      }
    } else {
      if (_inst->is_opt_idx)
        error("invalid instruction");
      else
        op1 = (int32_t)_vm->regs->regs[_inst->operand1];
    }
    op2 = (int32_t)_inst->imm_data;
  }

  switch (_inst->opcode) {
    case CMPIeq:
      if ((int64_t)op1 == (int64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPIlte:
      if ((int64_t)op1 <= (int64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPIgte:
      if ((int64_t)op1 >= (int64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPIulte:
      if ((uint64_t)op1 <= (uint64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    case CMPIugte:
      if ((uint64_t)op1 >= (uint64_t)op2)
        _vm->regs->regs[FLAGS] |= 0x01;
      break;
    default:
      error("invalid instruction");
  }

  return _vm;
}

static vm *exec_mov(vm *_vm, inst *_inst) {
  uint64_t op;
  if (_inst->op2_indirect) {
    switch (_inst->op_len) {
      case 1:
        op = read_mem8(_vm->mem, _vm->regs->regs[_inst->operand2]);
        break;
      case 2:
        op = read_mem16(_vm->mem, _vm->regs->regs[_inst->operand2]);
        break;
      case 4:
        op = read_mem32(_vm->mem, _vm->regs->regs[_inst->operand2]);
        break;
      case 8:
        op = read_mem64(_vm->mem, _vm->regs->regs[_inst->operand2]);
        break;
      default:
        error("invalid instruction");
    }
  } else
    op = _vm->regs->regs[_inst->operand2];

  if (_inst->is_op2_idx)
    op += _inst->op2_idx;

  if (_inst->op1_indirect) {
    if (_inst->is_op1_idx) {
      switch (_inst->op_len) {
        case 1:
          write_mem8(_vm->mem,
              _vm->regs->regs[_inst->operand1] + _inst->op1_idx,
              (uint8_t)op);
          break;
        case 2:
          write_mem16(_vm->mem,
              _vm->regs->regs[_inst->operand1] + _inst->op1_idx,
              (uint16_t)op);
          break;
        case 4:
          write_mem32(_vm->mem,
              _vm->regs->regs[_inst->operand1] + _inst->op1_idx,
              (uint32_t)op);
          break;
        case 8:
          write_mem64(_vm->mem,
              _vm->regs->regs[_inst->operand1] + _inst->op1_idx,
              (uint64_t)op);
          break;
        default:
          error("invalid instruction");
      }
    }
    switch (_inst->op_len) {
      case 1:
        write_mem8(_vm->mem,
            _vm->regs->regs[_inst->operand1], (uint8_t)op);
        break;
      case 2:
        write_mem16(_vm->mem,
            _vm->regs->regs[_inst->operand1], (uint16_t)op);
        break;
      case 4:
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1], (uint32_t)op);
        break;
      case 8:
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1], (uint64_t)op);
        break;
      default:
        error("invalid instruction");
    }
  } else {
    if (_inst->is_op1_idx)
      error("invalid instruction");
    else
      _vm->regs->regs[_inst->operand1] = op;
  }

  return _vm;
}

static vm *exec_movi(vm *_vm, inst *_inst) {
  uint64_t op1;
  if (_inst->op1_indirect) {
    if (_inst->is_opt_idx)
      op1 = _vm->regs->regs[_inst->operand1] + _inst->opt_idx;
    else
      op1 = _vm->regs->regs[_inst->operand1];
    switch (_inst->imm_len) {
      case 2:
        write_mem16(_vm->mem, op1, (uint16_t)_inst->imm_data);
        break;
      case 4:
        write_mem32(_vm->mem, op1, (uint32_t)_inst->imm_data);
        break;
      case 8:
        write_mem64(_vm->mem, op1, (uint64_t)_inst->imm_data);
        break;
      default:
        error("invalid instruction");
    }
  } else {
    if (_inst->is_opt_idx)
      error("invalid instruction");
    else
      _vm->regs->regs[_inst->operand1] = _inst->imm_data;
  }

  return _vm;
}

static vm *exec_movin(vm *_vm, inst *_inst) {
  uint64_t op1;
  if (_inst->op1_indirect) {
    if (_inst->is_opt_idx)
      op1 = _vm->regs->regs[_inst->operand2] + _inst->opt_idx;
    else
      op1 = _vm->regs->regs[_inst->operand2];
    write_mem64(_vm->mem, op1, _inst->imm_data);
  } else {
    if (_inst->is_opt_idx)
      error("invalid instruction");
    else
      _vm->regs->regs[_inst->operand1] = _inst->imm_data;
  }

  return _vm;
}

static vm *exec_movrel(vm *_vm, inst *_inst) {
  size_t inst_len = 2;
  if (_inst->is_opt_idx)
    inst_len += 2;
  inst_len += _inst->imm_len;

  uint64_t op2 = read_mem64(_vm->mem,
      _vm->regs->regs[IP] + inst_len + _inst->imm_data);
  if (_inst->op1_indirect) {
    uint64_t op1;
    if (_inst->is_opt_idx) {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1] + _inst->opt_idx);
    } else {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1]);
    }
    write_mem64(_vm->mem, op1, op2);
  } else {
    if (_inst->is_opt_idx)
      error("invalid instruction");
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_movn(vm *_vm, inst *_inst) {
  uint64_t op2;
  if (_inst->op2_indirect) {
    if (_inst->is_op2_idx) {
      op2 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand2] + _inst->op2_idx);
    } else {
      op2 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand2]);
    }
  } else {
    if (_inst->is_op2_idx)
      op2 = _vm->regs->regs[_inst->operand2] + _inst->op2_idx;
    else
      op2 = _vm->regs->regs[_inst->operand2];
  }
  if (_inst->op1_indirect) {
    uint64_t op1;
    if (_inst->is_op1_idx) {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1] + _inst->op1_idx);
    } else {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1]);
    }
    write_mem64(_vm->mem, op1, op2);
  } else {
    if (_inst->is_op1_idx)
      error("invalid instruction");
    else
      _vm->regs->regs[_inst->operand1] = op2;
  }

  return _vm;
}

static vm *exec_movsn(vm *_vm, inst *_inst) {
  int64_t op2;
  if (_inst->op2_indirect) {
    if (_inst->is_op2_idx) {
      op2 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand2] + _inst->op2_idx);
    } else {
      op2 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand2]);
    }
  } else {
    if (_inst->is_op2_idx)
      op2 = _vm->regs->regs[_inst->operand2] + _inst->op2_idx;
    else
      op2 = _vm->regs->regs[_inst->operand2];
  }

  if (_inst->op1_indirect) {
    int64_t op1;
    if (_inst->is_op1_idx) {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1] + _inst->op1_idx);
    } else {
      op1 = read_mem64(_vm->mem,
          _vm->regs->regs[_inst->operand1]);
    }
    write_mem64(_vm->mem, op1, op2);
  } else {
    if (_inst->is_op1_idx)
      error("invalid instruction");
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
            _vm->regs->regs[_inst->operand1] + _inst->imm, op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (uint64_t)_inst->imm;
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
            _vm->regs->regs[_inst->operand1] + _inst->imm, op);
      } else {
        _vm->regs->regs[_inst->operand1] = op + (uint32_t)_inst->imm;
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

/*FIXME: is POP identical with POPn? */
static vm *exec_popn(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    int64_t op = read_mem64(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->imm, op);
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
    int32_t op = read_mem32(_vm->mem, _vm->regs->regs[R0]);
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        write_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->imm, op);
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

static vm *exec_push(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    uint64_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->imm);
      } else {
        op = _vm->regs->regs[_inst->operand1] + _inst->imm;
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
            _vm->regs->regs[_inst->operand1] + _inst->imm);
      } else {
        op = _vm->regs->regs[_inst->operand1] + _inst->imm;
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

/*FIXME: is PUSH identical with PUSHn? */
static vm *exec_pushn(vm *_vm, inst *_inst) {
  if (_inst->is_64op) {
    int64_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem64(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->imm);
      } else {
        op = _vm->regs->regs[_inst->operand1] + _inst->imm;
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
    int32_t op;
    if (_inst->is_imm) {
      if (_inst->op1_indirect) {
        op = read_mem32(_vm->mem,
            _vm->regs->regs[_inst->operand1] + _inst->imm);
      } else {
        op = _vm->regs->regs[_inst->operand1] + _inst->imm;
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

static vm *exec_ret(vm *_vm, inst *_inst) {
  _vm->regs->regs[IP] = read_mem64(_vm->mem, _vm->regs->regs[R0]);
  _vm->regs->regs[R0] = _vm->regs->regs[R0] + 16;

  return _vm;
}

static vm *exec_loadsp(vm *_vm, inst *_inst) {
  if (_inst->operand1 <= RV2 && _inst->operand1 >= RV7)
    error("invalid instruction");

  if (_inst->operand1 != FLAGS)
    error("invalid instruction");

  if (_inst->operand1 == FLAGS) {
    /* FIXME: save reserved bits 2..63 */
    _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2];
  }

  return _vm;
}

static vm *exec_storesp(vm *_vm, inst *_inst) {
  _vm->regs->regs[_inst->operand1] = _vm->regs->regs[_inst->operand2];

  return _vm;
}

static vm *exec_nop(vm *_vm, inst *_inst) {
  /* do nothing */
  return _vm;
}

static vm *inc_ip(vm *_vm) {
  _vm->regs->regs[IP]++;
  return _vm;
}

vm *exec_op(vm *_vm, inst *_inst) {
  if (_inst->opcode >= ADD && _inst->opcode <= MODU) {
    arith_ops[_inst->opcode](_vm, _inst);
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
  inc_ip(_vm);

done_ret:
  return _vm;
}
