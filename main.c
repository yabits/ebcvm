#include "ebcvm.h"

char *bin_path;

static void init() {
  FLAGS_mem = MEM_SIZE;
  FLAGS_step = false;
}

static void print_usage(const char *argv0) {
  fprintf(stderr, "Usage: %s FILE [OPTION]...\n", argv0);
  fprintf(stderr, "EFI Byte Code Interpreter\n");
  fprintf(stderr, "[OPTION]\n");
  fprintf(stderr, "  --mem=INT\t\tsize of memory\n");
  fprintf(stderr, "  --step={0,1}\t\tstep execution\n");
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
    } else if (i == 1) {
      bin_path = argv[i];
    } else if (sscanf(argv[i], "--mem=%d%c", &n, &junk) == 1) {
      FLAGS_mem = n;
    } else if (sscanf(argv[i], "--step=%d%c", &n, &junk) == 1 &&
        (n == 0 || n == 1)) {
      FLAGS_step = n;
    } else {
      error("invalid flag '%s'\n", argv[i]);
    }
  }

  vm *_vm = init_vm();
  
  _vm = load_exe(bin_path, _vm);

  exec_vm(_vm);

  return 0;
}
