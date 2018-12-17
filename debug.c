#include "ebcvm.h"

typedef enum cmd_type {
  INVALID,
  CONTINUE,
  REG,
  EXAMINE,
  DISASSEMBLE,
  MEMMAP,
  BACKTRACE,
  HELP,
  QUIT,
} cmd_type;

typedef struct cmds {
  cmd_type type;
  uint64_t addr;
  size_t size;
} cmds;

static void print_reg(dbg *_dbg) {
  const char *regs[] = {
  "FLAGS", "IP", "", "", "", "", "", "",
  "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
  };
  for (int i = 0; i < 16; i++) {
    if (!strcmp(regs[i], ""))
      continue;
    fprintf(stdout, "%s\t0x%lx\n", regs[i], _dbg->_vm->regs->regs[i]);
  }
}

static void print_mem(dbg *_dbg, uint64_t addr, size_t size) {
  if (_dbg->_vm->mem->size < addr + size)
    fprintf(stderr, "out of memory\n");
  for (int i = 0; i < size; i++) {
    if (i % 8 == 0 && i > 0)
      fprintf(stdout, "\n");
    if (i % 8 == 0)
      fprintf(stdout, "0x%016lx:", addr + i);
    fprintf(stdout, " %02x", read_mem8(_dbg->_vm->mem, addr + i));
  }
  fprintf(stdout, "\n");
}

static void print_disas(dbg *_dbg, uint64_t addr, size_t size) {
  if (_dbg->_vm->mem->size < addr + size)
    fprintf(stderr, "out of memory\n");
  uint64_t prev_ip = _dbg->_vm->regs->regs[IP];
  _dbg->_vm->regs->regs[IP] = addr;
  while (_dbg->_vm->regs->regs[IP] < addr + size) {
    size_t op_len = dump_inst(_dbg->_vm);
    _dbg->_vm->regs->regs[IP] += op_len;
  }
  _dbg->_vm->regs->regs[IP] = prev_ip;
}

static void print_memmap(dbg *_dbg) {
  const char *mem_types[] = {
  "heap", "stack", "text", "data", "rodata", "bss", "efi", "unknown",
  };
  for (int i = 0; i < _dbg->_vm->memmap_size; i++) {
    fprintf(stdout, "%s\t0x%016lx - 0x%016lx\n",
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
    fprintf(stdout, "frame #%d: 0x%016lx 0x%016lx\n",
            i, p, read_mem64(_dbg->_vm->mem, p));
    i++;
  }
}

static void print_help() {
  fprintf(stdout, "List of commands\n");
  fprintf(stdout, "continue -- continue program\n");
  fprintf(stdout, "reg -- show registers\n");
  fprintf(stdout, "examine -- show memory\n");
  fprintf(stdout, "disassemble -- disassemble memory\n");
  fprintf(stdout, "memmap -- show memory map\n");
  fprintf(stdout, "backtrace -- show backtrace\n");
  fprintf(stdout, "quit -- quit program\n");
  fprintf(stdout, "help -- show this help\n");
}

static cmds *parse_cmd(const char *str) {
  cmds *_cmds = malloc(sizeof(cmds));

  uint64_t addr = 0;
  size_t size = 8;
  if (!strcmp(str, "continue\n") || !strcmp(str, "c\n")) {
    _cmds->type = CONTINUE;
  } else if (!strcmp(str, "reg\n") || !strcmp(str, "r\n")) {
    _cmds->type = REG;
  } else if ((sscanf(str, "examine 0x%lx\n", &addr) == 1)
          || (sscanf(str, "x 0x%lx\n", &addr) == 1)
          || (sscanf(str, "examine/%zd 0x%lx\n", &size, &addr) == 2)
          || (sscanf(str, "x/%zd 0x%lx\n", &size, &addr) == 2)) {
    _cmds->type = EXAMINE;
    _cmds->addr = addr;
    _cmds->size = size;
  } else if ((sscanf(str, "disassemble 0x%lx\n", &addr) == 1)
          || (sscanf(str, "disas 0x%lx\n", &addr) == 1)
          || (sscanf(str, "disassemble/%zd 0x%lx\n", &size, &addr) == 2)
          || (sscanf(str, "disas/%zd 0x%lx\n", &size, &addr) == 2)) {
    _cmds->type = DISASSEMBLE;
    _cmds->addr = addr;
    _cmds->size = size;
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
      print_mem(_dbg, _cmds->addr, _cmds->size);
      break;
    case DISASSEMBLE:
      print_disas(_dbg, _cmds->addr, _cmds->size);
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
  fprintf(stdout, "IP = %lx\n", _dbg->_vm->regs->regs[IP]);
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

void handle_except(dbg *_dbg, except _except, const char *str, const char *file, int line) {
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
    "MEMORY",
    "UNDEF",
  };
  
  fprintf(stdout, "exception %s: %s %s at %d\n",
      exceptions[_except], str, file, line);

  prompt(_dbg);
}
