/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"

char *bin_path;

static void init() {
  FLAGS_debug = false;
  FLAGS_mem = AUTO_MEM_SIZE;
  FLAGS_stack = STACK_SIZE;
  FLAGS_heap = HEAP_SIZE;
  FLAGS_step = false;
  FLAGS_reloc = true;
}

static void print_usage(const char *argv0) {
  fprintf(stderr, "Usage: %s [OPTION] FILE\n", argv0);
  fprintf(stderr, "EFI Byte Code Interpreter\n");
  fprintf(stderr, "[OPTION]\n");
  fprintf(stderr, "  --debug={0,1}\t\tdebug mode\n");
  fprintf(stderr, "  --mem=INT\t\tsize of memory\n");
  fprintf(stderr, "  --stack=INT\t\tsize of stack\n");
  fprintf(stderr, "  --heap=INT\t\tsize of heap\n");
  fprintf(stderr, "  --step={0,1}\t\tstep execution\n");
  fprintf(stderr, "  --reloc={0,1}\t\trelocate sections\n");
  fprintf(stderr, "  --help\t\tshow this help\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    exit(0);
  }

  init();

  for (int i = 1; i < argc; i++) {
    int n;
    char junk;
    if (!strcmp(argv[i], "--help")) {
      print_usage(argv[0]);
      exit(0);
    } else if (i == argc - 1) {
      bin_path = argv[i];
    } else if (sscanf(argv[i], "--mem=%d%c", &n, &junk) == 1 && n > 0) {
      FLAGS_mem = n;
    } else if (sscanf(argv[i], "--stack=%d%c", &n, &junk) == 1 && n > 0) {
      FLAGS_stack = n;
    } else if (sscanf(argv[i], "--heap=%d%c", &n, &junk) == 1 && n > 0) {
      FLAGS_heap = n;
    } else if (sscanf(argv[i], "--step=%d%c", &n, &junk) == 1 &&
        (n == 0 || n == 1)) {
      FLAGS_step = n;
    } else if (sscanf(argv[i], "--debug=%d%c", &n, &junk) == 1 &&
        (n == 0 || n == 1)) {
      FLAGS_debug = n;
    } else if (sscanf(argv[i], "--reloc=%d%c", &n, &junk) == 1 &&
        (n == 0 || n == 1)) {
      FLAGS_reloc = n;
    } else {
      error("invalid flag '%s'\n", argv[i]);
    }
  }

  if (FLAGS_mem != AUTO_MEM_SIZE && FLAGS_mem < FLAGS_heap + FLAGS_stack)
    error("memory size is too small");

  vm *_vm = init_vm();

  if (FLAGS_debug)
    _dbg = init_dbg(_vm);
  
  _vm = load_exe(bin_path, _vm);

  exec_vm(_vm);

  fini_vm(_vm);

  if (FLAGS_debug)
    fini_dbg(_dbg);

  return 0;
}
