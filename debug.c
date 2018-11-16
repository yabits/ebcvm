#include "ebcvm.h"

typedef enum cmd_type {
  INVALID,
  CONTINUE,
  REG,
  EXAMINE,
  MEMMAP,
  BACKTRACE,
  HELP,
  QUIT,
} cmd_type;

typedef struct cmds {
  cmd_type type;
  int value;
} cmds;

static void print_reg(dbg *_dbg) {
  const char *regs[] = {
  "IP", "FLAGS", "", "", "", "", "", "",
  "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
  };
  for (int i = 0; i < 16; i++) {
    if (!strcmp(regs[i], ""))
      continue;
    fprintf(stdout, "%s\t0x%llx\n", regs[i], _dbg->_vm->regs->regs[i]);
  }
}

static void print_mem(dbg *_dbg, size_t addr) {
  if (_dbg->_vm->mem->size < addr)
    fprintf(stderr, "out of memory\n");
  for (int i = 0; i < 8; i++) {
    if (i > 0)
      fprintf(stdout, " ");
    fprintf(stdout, "%02x", read_mem8(_dbg->_vm->mem, addr + i));
  }
  fprintf(stdout, "\n");
}

static void print_memmap(dbg *_dbg) {
  const char *mem_types[] = {
  "data", "text", "bss", "efi", "unknown",
  };
  for (int i = 0; i < _dbg->_vm->memmap_size; i++) {
    fprintf(stdout, "%s\t0x%016llx - 0x%016llx\n",
            mem_types[_dbg->_vm->memmap[i].mem_type],
            _dbg->_vm->memmap[i].addr,
            _dbg->_vm->memmap[i].addr + _dbg->_vm->memmap[i].size);
  }
}

static void print_backtrace(dbg *_dbg) {
  int i = 0;
  for (uint64_t p = _dbg->_vm->regs->regs[R0];
      read_mem64(_dbg->_vm->mem, p) != STACK_MAGIC;
      p += 8) {
    fprintf(stdout, "frame #%d: 0x%016llx 0x%016llx\n",
            i, p, read_mem64(_dbg->_vm->mem, p));
    i++;
  }
}

static void print_help() {
  fprintf(stdout, "List of commands\n");
  fprintf(stdout, "continue -- continue program\n");
  fprintf(stdout, "reg -- show registers\n");
  fprintf(stdout, "examine -- show memory\n");
  fprintf(stdout, "memmap -- show memory map\n");
  fprintf(stdout, "backtrace -- show backtrace\n");
  fprintf(stdout, "quit -- quit program\n");
  fprintf(stdout, "help -- show this help\n");
}

static cmds *parse_cmd(const char *str) {
  cmds *_cmds = malloc(sizeof(cmds));

  int n;
  if (!strcmp(str, "continue\n") || !strcmp(str, "c\n")) {
    _cmds->type = CONTINUE;
  } else if (!strcmp(str, "reg\n") || !strcmp(str, "r\n")) {
    _cmds->type = REG;
  } else if ((sscanf(str, "examine 0x%x\n", &n) == 1)
          || (sscanf(str, "x 0x%x\n", &n) == 1)) {
    _cmds->type = EXAMINE;
    _cmds->value = n;
  } else if (!strcmp(str, "memmap\n") || !strcmp(str, "m\n")) {
    _cmds->type = MEMMAP;
  } else if (!strcmp(str, "backtrace\n") || !strcmp(str, "bt\n")) {
    _cmds->type = BACKTRACE;
  } else if (!strcmp(str, "help\n") || !strcmp(str, "h\n")) {
    _cmds->type = HELP;
  } else if (!strcmp(str, "quit\n") || !strcmp(str, "q\n")) {
    _cmds->type = QUIT;
  } else {
    _cmds->type = INVALID;
  }

  return _cmds;
}

static int exec_cmd(dbg *_dbg, cmds *_cmds) {
  int res = 1;
  switch (_cmds->type) {
    case CONTINUE:
      res = 0;
      break;
    case REG:
      print_reg(_dbg);
      break;
    case EXAMINE:
      print_mem(_dbg, _cmds->value);
      break;
    case MEMMAP:
      print_memmap(_dbg);
      break;
    case BACKTRACE:
      print_backtrace(_dbg);
      break;
    case HELP:
      print_help();
      break;
    case QUIT:
      exit(0);
      break;
    default:
      fprintf(stderr, "invalid command\n");
  }

  return res;
}

static void prompt(dbg *_dbg) {
  fprintf(stdout, "IP = %llx\n", _dbg->_vm->regs->regs[IP]);
  if (_dbg->_vm->regs->regs[IP] != RET_MAGIC)
    dump_inst(_dbg->_vm);
  while (true) {
    fprintf(stdout, "(dbg) ");

    char str[1024];
    if (!fgets(str, sizeof(str), stdin))
      break;

    cmds *_cmds = parse_cmd(str);
    int res = exec_cmd(_dbg, _cmds);
    free(_cmds);

    if (res == 0)
      break;
  }
}

dbg *init_dbg(vm *_vm) {
  dbg *_dbg = malloc(sizeof(dbg));

  _dbg->_vm = _vm;

  return _dbg;
}

void fini_dbg(dbg *_dbg) {
  free(_dbg);
}

void handle_except(dbg *_dbg, except _except, const char *str) {
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
  
  fprintf(stdout, "exception %s: %s\n", exceptions[_except], str);

  prompt(_dbg);
}
