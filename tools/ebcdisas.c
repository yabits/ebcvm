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
  fprintf(stderr, "Usage: %s FILE [OPTION]...\n", argv0);
  fprintf(stderr, "EFI Byte Code Disassembler\n");
  fprintf(stderr, "[OPTION]\n");
  fprintf(stderr, "  --help\t\tshow this help\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    exit(0);
  }

  init();

  for (int i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "--help")) {
      print_usage(argv[0]);
      exit(0);
    } else if (i == 1) {
      bin_path = argv[i];
    } else {
      error("invalid flag '%s'\n", argv[i]);
    }
  }

  vm *_vm = init_vm();

  _vm = load_exe(bin_path, _vm);

  dump_vm(_vm);

  fini_vm(_vm);

  return 0;
}
