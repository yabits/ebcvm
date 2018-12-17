#include <stdio.h>
#include <stdint.h>

const uint64_t offset = 0xcbf29ce484222325;
const uint64_t prime = 0x100000001b3;

int main(int argc, char *argv[]) {
  uint64_t hash = offset;

  char c;
  while ((c = fgetc(stdin)) != EOF) {
    hash = hash * prime;
    hash = hash ^ c;
  }

  printf("0x%016lx\n", hash);

  return 0;
}
