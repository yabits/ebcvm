#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ebcvm.h"
#include "pe.h"

static vm *do_load_exe(const char *addr, vm *_vm) {
  /* check executable */
  IMAGE_DOS_HEADER *doshdr = (IMAGE_DOS_HEADER *)addr;
  if (doshdr->e_magic != MAGIC_MZ)
    goto unsupported;

  IMAGE_NT_HEADERS *nthdr = (IMAGE_NT_HEADERS *)(addr + doshdr->e_lfanew);
  if (nthdr->Signature != MAGIC_PE)
    goto unsupported;

  IMAGE_FILE_HEADER *fhdr = &nthdr->FileHeader;
  if (fhdr->Machine != 0x0ebc)
    goto unsupported;

  if (!(fhdr->Characteristics & IMAGE_FILE_32BIT_MACHINE))
    goto unsupported;

  IMAGE_OPTIONAL_HEADER *opthdr = &nthdr->OptionalHeader;
  if (opthdr->Magic != 0x20b)
    goto unsupported;

  int entry_point = opthdr->AddressOfEntryPoint;

  IMAGE_SECTION_HEADER *sechdr;
  for (int i = 0; i < fhdr->NumberOfSections; i++) {
    sechdr = (IMAGE_SECTION_HEADER *)
      (addr + doshdr->e_lfanew
       + sizeof(IMAGE_NT_HEADERS)
       + sizeof(IMAGE_SECTION_HEADER) * i);
    /* copy section */
  }

  return _vm;

unsupported:
  error("unsupported file");
  return NULL;
}

vm *load_exe(const char *path, vm *_vm) {
  if (access(path, F_OK) == -1)
    error("file does not exists");

  int fd = open(path, O_RDONLY);
  if (fd < 0)
    error("could not open file");

  struct stat sb;
  fstat(fd, &sb);
  char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (!addr)
    error("mmap failed");

  _vm = do_load_exe(addr, _vm);

  if (munmap(addr, sb.st_size))
    error("munmap failed");

  if (close(fd))
    error("could not close file");

  return _vm;
}
