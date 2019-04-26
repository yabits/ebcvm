#include "ebcvm.h"

#define OP_SIZE 128

#define DISAS_INDEX(bits)                                             \
static char *disas_index##bits(uint##bits##_t index) {                \
  uint##bits##_t mask;                                                \
  uint8_t b = sizeof(uint##bits##_t) * 8;                             \
  int64_t s = index >> (b - 1) ? -1 : 1;                              \
  uint8_t a = ((index >> (b - 4)) & 0x07) * sizeof(uint##bits##_t);   \
  mask = 0x00;                                                        \
  for (int i = a; i < b - 4; i++)                                     \
    mask |= 0x01 << i;                                                \
  uint##bits##_t c = (index & mask) >> a;                             \
  mask = 0x00;                                                        \
  for (int i = 0; i < a; i++)                                         \
    mask |= 0x01 << i;                                                \
  uint##bits##_t n = (index & mask) >> 0;                             \
  char *idx = malloc(sizeof(char) * OP_SIZE);                         \
  if (!idx)                                                           \
    error("malloc failed");                                           \
  sprintf(idx, "(%+ld, %+ld)", s * n, s * c);                         \
  return idx;                                                         \
}

DISAS_INDEX(16)
DISAS_INDEX(32)
DISAS_INDEX(64)

const char *opcodes[] = {
  "BREAK",
  "JMP",
  "JMP8",
  "CALL",
  "CMPeq",
  "CMPlte",
  "CMPgte",
  "CMPulte",
  "CMPugte",
  "CMPIeq",
  "CMPIlte",
  "CMPIgte",
  "CMPIulte",
  "CMPIugte",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD",
  "AND",
  "OR",
  "XOR",
  "SHL",
  "SHR",
  "ASHR",
  "NEG",
  "NOT",
  "MULU",
  "DIVU",
  "MODU",
  "EXTNDB",
  "EXTNDD",
  "EXTNDW",
  "MOVbw",
  "MOVww",
  "MOVdw",
  "MOVqw",
  "MOVbd",
  "MOVwd",
  "MOVdd",
  "MOVqd",
  "MOVqq",
  "MOVI",
  "MOVIn",
  "MOVREL",
  "MOVnw",
  "MOVnd",
  "MOVsnw",
  "MOVsnd",
  "POP",
  "POPn",
  "PUSH",
  "PUSHn",
  "RET",
  "LOADSP",
  "STORESP",
  "NOP",
};

const char *regsstr[] = {
  "[FLAGS]",
  "[IP]",
  "RV2",
  "RV3",
  "RV4",
  "RV5",
  "RV6",
  "RV7",
  "R0",
  "R1",
  "R2",
  "R3",
  "R4",
  "R5",
  "R6",
  "R7",
};

static char *disas_break(inst *_inst) {
  if (_inst->opcode != BREAK)
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  sprintf(op, " %d", _inst->break_code);

  return op;
}

static char *disas_call_jmp_jmp8(inst *_inst) {
  opcode opcode = _inst->opcode;
  if (!(opcode == CALL || opcode == JMP || opcode == JMP8))
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

#if 0
  if (opcode == CALL || opcode == JMP) {
    if (!_inst->is_jmp64)
      strcat(op, "32");
    else
      strcat(op, "64");
  }
#endif

  if (opcode == JMP || opcode == JMP8) {
    if (_inst->is_cond) {
      if (_inst->is_cs)
        strcat(op, "cs");
      else
        strcat(op, "cc");
    }
  } else if (opcode == CALL) {
    if (_inst->is_native)
      strcat(op, "EX");
#if 0
    if (!_inst->is_rel)
      strcat(op, "a");
#endif
  }

  strcat(op, " ");

  if (opcode == CALL || opcode == JMP) {
    if (_inst->is_jmp64) {
      char imm64[OP_SIZE];
      sprintf(imm64, "0x%016lx", _inst->jmp_imm);
      strcat(op, imm64);
    } else {
      if (_inst->op1_indirect)
        strcat(op, "@");
      strcat(op, regsstr[_inst->operand1]);
      if (_inst->is_jmp_imm) {
        strcat(op, " ");
        if (!_inst->op1_indirect) {
          char imm32[OP_SIZE];
          sprintf(imm32, "(0x%08x)", (uint32_t)_inst->jmp_imm);
          strcat(op, imm32);
        } else
          strcat(op, disas_index32((uint32_t)_inst->jmp_imm));
      }
    }
  }

  if (opcode == JMP8) {
    char imm8[OP_SIZE];
    sprintf(imm8, "0x%02x", (uint8_t)_inst->jmp_imm);
    strcat(op, imm8);
  }

  return op;
}

static char *disas_arith_stack_cmp_extnd(inst *_inst) {
  opcode opcode = _inst->opcode;
  if (!((opcode >= ADD && opcode <= MODU)
      || (opcode >= POP && opcode <= PUSHn)
      || (opcode >= CMPeq && opcode <= CMPugte)
      || (opcode >= EXTNDB && opcode <= EXTNDW)))
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

#if 0
  if (!_inst->is_64op)
    strcat(op, "32");
  else
    strcat(op, "64");
#endif

  strcat(op, " ");

  if (!(opcode >= CMPeq && opcode <= CMPugte)) {
    if (_inst->op1_indirect)
      strcat(op, "@");
  }
  strcat(op, regsstr[_inst->operand1]);

  if ((opcode >= ADD && opcode <= MODU)
      || (opcode >= CMPeq && opcode <= CMPugte)
      || (opcode >= EXTNDB && opcode <= EXTNDW)) {
    strcat(op, ", ");
    if (_inst->op2_indirect)
      strcat(op, "@");
    strcat(op, regsstr[_inst->operand2]);

    if (_inst->is_imm) {
      strcat(op, " ");
      if (!_inst->op2_indirect) {
        char imm16[OP_SIZE];
        sprintf(imm16, "(0x%04x)", _inst->imm);
        strcat(op, imm16);
      } else {
        char *idx16 = disas_index16(_inst->imm);
        strcat(op, idx16);
        free(idx16);
      }
    }
  } else {
    if (_inst->is_imm) {
      strcat(op, " ");
      if (!_inst->op1_indirect) {
        char imm16[OP_SIZE];
        sprintf(imm16, "(0x%04x)", _inst->imm);
        strcat(op, imm16);
      } else {
        char *idx16 = disas_index16(_inst->imm);
        strcat(op, idx16);
        free(idx16);
      }
    }
  }

  return op;
}

static char *disas_loadsp_store_sp(inst *_inst) {
  opcode opcode = _inst->opcode;
  if (!(opcode == LOADSP || opcode == STORESP))
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

  strcat(op, " ");
  strcat(op, regsstr[_inst->operand1]);
  strcat(op, ", ");
  strcat(op, regsstr[_inst->operand2]);

  return op;
}

static char *disas_mov_movn_movsn(inst *_inst) {
  opcode opcode = _inst->opcode;
  if (!((opcode >= MOVbw && opcode <= MOVqq)
      || (opcode == MOVnw || opcode == MOVnd)
      || (opcode == MOVsnw || opcode == MOVsnd)))
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

  strcat(op, " ");
  if (_inst->op1_indirect)
    strcat(op, "@");
  strcat(op, regsstr[_inst->operand1]);

  if (_inst->is_op1_idx) {
    strcat(op, " ");
    if (!_inst->op1_indirect && (opcode == MOVsnw || opcode == MOVsnd)) {
      char imm[OP_SIZE];
      switch (_inst->idx_len) {
        case 2:
          sprintf(imm, "(0x%04x)", (uint16_t)_inst->op1_idx);
          break;
        case 4:
          sprintf(imm, "(0x%08x)", (uint32_t)_inst->op1_idx);
          break;
        case 8:
          sprintf(imm, "(0x%016lx)", (uint64_t)_inst->op1_idx);
          break;
        default:
          error("invalid operand");
      }
      strcat(op, imm);
    } else {
      char *idx = NULL;
      switch (_inst->idx_len) {
        case 2:
          idx = disas_index16((uint16_t)_inst->op1_idx);
          break;
        case 4:
          idx = disas_index32((uint32_t)_inst->op1_idx);
          break;
        case 8:
          idx = disas_index64((uint64_t)_inst->op1_idx);
          break;
        default:
          error("invalid operand");
      }
      strcat(op, idx);
      free(idx);
    }
  }

  strcat(op, ", ");
  if (_inst->op2_indirect)
    strcat(op, "@");
  strcat(op, regsstr[_inst->operand2]);

  if (_inst->is_op2_idx) {
    strcat(op, " ");
    if (!_inst->op2_indirect && (opcode == MOVsnw || opcode == MOVsnd)) {
      char imm[OP_SIZE];
      switch (_inst->idx_len) {
        case 2:
          sprintf(imm, "(0x%04x)", (uint16_t)_inst->op2_idx);
          break;
        case 4:
          sprintf(imm, "(0x%08x)", (uint32_t)_inst->op2_idx);
          break;
        case 8:
          sprintf(imm, "(0x%016lx)", (uint64_t)_inst->op2_idx);
          break;
        default:
          error("invalid operand");
      }
      strcat(op, imm);
    } else {
      char *idx = NULL;
      switch (_inst->idx_len) {
        case 2:
          idx = disas_index16((uint16_t)_inst->op2_idx);
          break;
        case 4:
          idx = disas_index32((uint32_t)_inst->op2_idx);
          break;
        case 8:
          idx = disas_index64((uint64_t)_inst->op2_idx);
          break;
        default:
          error("invalid operand");
      }
      strcat(op, idx);
      free(idx);
    }
  }

  return op;
}

static char *disas_movi_movin_movrel_cmpi(inst *_inst) {
  opcode opcode = _inst->opcode;
  if (!((opcode == MOVI || opcode == MOVIn || opcode == MOVREL)
      || (opcode >= CMPIeq && opcode <= CMPIugte)))
    error("invalid opcode");

  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

  if (opcode >= CMPIeq && opcode <= CMPIugte) {
    strcat(op, "CMPI");
#if 0
    if (_inst->mov_len == 4)
      strcat(op, "32");
    else if (_inst->mov_len == 8)
      strcat(op, "64");
    else
      error("invalid operand");
#endif
  }

  if (opcode >= CMPIeq && opcode <= CMPIugte) {
    switch (opcode) {
      case CMPIeq:
        strcat(op, "eq");
        break;
      case CMPIlte:
        strcat(op, "lte");
        break;
      case CMPIgte:
        strcat(op, "gte");
        break;
      case CMPIulte:
        strcat(op, "ulte");
        break;
      case CMPIugte:
        strcat(op, "ugte");
        break;
      default:
        error("invalid opcode");
    }
  }

  strcat(op, " ");
  if (_inst->op1_indirect)
    strcat(op, "@");
  strcat(op, regsstr[_inst->operand1]);

  if (_inst->is_opt_idx) {
    strcat(op, " ");
    char *idx = NULL;
    idx = disas_index16((uint16_t)_inst->op1_idx);
    strcat(op, idx);
    free(idx);
  }

  strcat(op, ", ");
  if (opcode == MOVI
      || (opcode >= CMPIeq && opcode <= CMPIugte)) {
    char imm[OP_SIZE];
    switch (_inst->imm_len) {
      case 2:
        sprintf(imm, "(0x%04x)", (uint16_t)_inst->imm_data);
        break;
      case 4:
        sprintf(imm, "(0x%08x)", (uint32_t)_inst->imm_data);
        break;
      case 8:
        sprintf(imm, "(0x%016lx)", (uint64_t)_inst->imm_data);
        break;
      default:
        error("invalid operand");
    }
    strcat(op, imm);
  } else if (opcode == MOVREL) {
    char imm[OP_SIZE];
    switch (_inst->imm_len) {
      case 2:
        sprintf(imm, "0x%04x", (uint16_t)_inst->imm_data);
        break;
      case 4:
        sprintf(imm, "0x%08x", (uint32_t)_inst->imm_data);
        break;
      case 8:
        sprintf(imm, "0x%016lx", (uint64_t)_inst->imm_data);
        break;
      default:
        error("invalid operand");
    }
    strcat(op, imm);
  } else if (opcode == MOVIn) {
      char *idx = NULL;
      switch (_inst->imm_len) {
        case 2:
          idx = disas_index16((uint16_t)_inst->imm_data);
          break;
        case 4:
          idx = disas_index32((uint32_t)_inst->imm_data);
          break;
        case 8:
          idx = disas_index64((uint64_t)_inst->imm_data);
          break;
        default:
          error("invalid operand");
      }
      strcat(op, idx);
      free(idx);
  } else
    error("invalid operand");

  return op;
}

char *disas_inst(inst *_inst) {
  opcode opcode = _inst->opcode;
  char *op = malloc(sizeof(char) * OP_SIZE);
  if (!op)
    error("malloc failed");
  op[0] = '\0';

  /* XXX: to match the format in CMPI */
  if (!(opcode >= CMPIeq && opcode <= CMPIugte))
    strcpy(op, opcodes[opcode]);

  char *operands = NULL;
  if (_inst->opcode == BREAK) {
    operands = disas_break(_inst);
  } else if (opcode == CALL || opcode == JMP || opcode == JMP8) {
    operands = disas_call_jmp_jmp8(_inst);
  } else if ((opcode >= ADD && opcode <= MODU)
          || (opcode >= POP && opcode <= PUSHn)
          || (opcode >= CMPeq && opcode <= CMPugte)
          || (opcode >= EXTNDB && opcode <= EXTNDW)) {
    operands = disas_arith_stack_cmp_extnd(_inst);
  } else if (opcode == LOADSP || opcode == STORESP){
    operands = disas_loadsp_store_sp(_inst);
  } else if ((opcode >= MOVbw && opcode <= MOVqq)
          || (opcode == MOVnw || opcode == MOVnd)
          || (opcode == MOVsnw || opcode == MOVsnd)) {
    operands = disas_mov_movn_movsn(_inst);
  } else if (opcode == MOVI || opcode == MOVIn || opcode == MOVREL
          || (opcode >= CMPIeq && opcode <= CMPIugte)) {
    operands = disas_movi_movin_movrel_cmpi(_inst);
  } else {
    operands = malloc(sizeof(char) * OP_SIZE);
    operands[0] = '\0';
  }
  strcat(op, operands);
  free(operands);

  return op;
}
