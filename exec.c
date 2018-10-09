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
EXEC_OP(int64_t, shl, op1 << op2);
EXEC_OP(int64_t, shr, op1 >> op2);
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
  exec_neg,
  exec_not,
  exec_mulu,
  exec_divu,
  exec_modu,
};

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
  switch (_inst->opcode) {
    case MOVbw:
    case MOVww:
    case MOVdw:
    case MOVqw:
    case MOVbd:
    case MOVwd:
    case MOVdd:
    case MOVqd:
    case MOVqq:
      exec_mov(_vm, _inst);
      goto done_inc;
    case MOVI:
      exec_movi(_vm, _inst);
      goto done_inc;
    case MOVIn:
      exec_movin(_vm, _inst);
      goto done_inc;
    case MOVnd:
    case MOVnw:
      exec_movn(_vm, _inst);
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
      goto done_free;
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

done_free:
  free(_inst);
  return _vm;
}
