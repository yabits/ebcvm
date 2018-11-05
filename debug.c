#include "ebcvm.h"

typedef enum cmd_type {
  CONTINUE,
  HELP,
  QUIT,
} cmd_type;

typedef struct cmds {
  cmd_type type;
} cmds;

static void print_help() {
  fprintf(stdout, "List of commands\n");
  fprintf(stdout, "continue -- continue program\n");
  fprintf(stdout, "quit -- quit program\n");
  fprintf(stdout, "help -- show this help\n");
}

static cmds *parse_cmd(const char *_cmd, const char *_args) {
  cmds *_cmds = malloc(sizeof(cmds));

  if (!strcmp(_cmd, "continue") || !strcmp(_cmd, "c")) {
    _cmds->type = CONTINUE;
  } else if (!strcmp(_cmd, "help") || !strcmp(_cmd, "h")) {
    _cmds->type = HELP;
  } else if (!strcmp(_cmd, "quit") || !strcmp(_cmd, "q")) {
    _cmds->type = QUIT;
  }

  return _cmds;
}

static int exec_cmd(dbg *_dbg, cmds *_cmds) {
  int res;
  switch (_cmds->type) {
    case CONTINUE:
      res = 0;
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

    char _cmd[64];
    char _args[128];
    if (sscanf(str, "%63s %127[^\n]", _cmd, _args) != 2) {
      fprintf(stderr, "syntax error\n");
      continue;
    }

    cmds *_cmds = parse_cmd(_cmd, _args);

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
    "UNDEF",
  };
  
  fprintf(stdout, "exception %s: %s\n", exceptions[_except], str);

  prompt(_dbg);
}
