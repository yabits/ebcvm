#include "ebcvm.h"

typedef enum cmd_type {
  CONTINUE,
  REG,
  EXAMINE,
  HELP,
  QUIT,
} cmd_type;

typedef struct cmds {
  cmd_type type;
  int value;
} cmds;

static void print_reg(dbg *_dbg) {
  char *regs[] = {
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

static void print_help() {
  fprintf(stdout, "List of commands\n");
  fprintf(stdout, "continue -- continue program\n");
  fprintf(stdout, "reg -- show registers\n");
  fprintf(stdout, "examine -- show memory\n");
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
  } else if (!strcmp(str, "help\n") || !strcmp(str, "h\n")) {
    _cmds->type = HELP;
  } else if (!strcmp(str, "quit\n") || !strcmp(str, "q\n")) {
    _cmds->type = QUIT;
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
