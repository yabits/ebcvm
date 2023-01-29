/* SPDX-License-Identifier: MIT */

#include "ebcvm.h"

#ifdef __GNUC__
__attribute__((noreturn))
#endif
void error(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  exit(1);
}

uint64_t uintn(uint64_t value) {
#if ARCH_BYTES == 4
  return (uint32_t)value;
#elif ARCH_BYTES == 8
  return (uint64_t)value;
#else
#error unsupported architecture
#endif
}
