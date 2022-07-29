/* SPDX-License-Identifier: MIT */

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

  IMAGE_OPTIONAL_HEADER *opthdr = &nthdr->OptionalHeader;
  if (opthdr->Magic != 0x20b)
    goto unsupported;

  if (fhdr->NumberOfSections < 1)
    goto unsupported;
  
  uint32_t entry_point = opthdr->AddressOfEntryPoint;
  uint64_t image_base = opthdr->ImageBase;

  uint64_t align = (uint64_t)opthdr->SectionAlignment;

  if (FLAGS_reloc)
    image_base = ((((uint64_t)FLAGS_heap + (uint64_t)FLAGS_stack) / align)
                  + 1 ) * align;

  _vm->memmap_size = fhdr->NumberOfSections + 2;
  _vm->memmap = malloc(sizeof(memmap) * _vm->memmap_size);
  if (!_vm->memmap)
    goto fail;

  _vm->memmap[0].mem_type = MEM_HEAP;
  _vm->memmap[0].addr = 0;
  _vm->memmap[0].size = FLAGS_heap;

  _vm->memmap[1].mem_type = MEM_STACK;
  _vm->memmap[1].addr = ((FLAGS_heap / align) + 1) * align;
  _vm->memmap[1].size = FLAGS_stack;

  /* enumerate sections */
  IMAGE_SECTION_HEADER *sechdr;
  for (int i = 0; i < fhdr->NumberOfSections; i++) {
    sechdr = (IMAGE_SECTION_HEADER *)
      (addr + doshdr->e_lfanew + sizeof(IMAGE_NT_HEADERS)
       + sizeof(IMAGE_DATA_DIRECTORY) * opthdr->NumberOfRvaAndSizes
       + sizeof(IMAGE_SECTION_HEADER) * i);
    if (!memcmp(sechdr->Name, ".text", 5))
      _vm->memmap[i + 2].mem_type = MEM_TEXT;
    else if (!memcmp(sechdr->Name, ".data", 5))
      _vm->memmap[i + 2].mem_type = MEM_DATA;
    else if (!memcmp(sechdr->Name, ".rodata", 7))
      _vm->memmap[i + 2].mem_type = MEM_RODATA;
    else if (!memcmp(sechdr->Name, ".bss", 4))
      _vm->memmap[i + 2].mem_type = MEM_BSS;
    else
      _vm->memmap[i + 2].mem_type = MEM_UNKNOWN;
    _vm->memmap[i + 2].addr = image_base + sechdr->VirtualAddress;
    _vm->memmap[i + 2].size = sechdr->Misc.VirtualSize;
  }

  uint16_t last = _vm->memmap_size - 1;
  uint64_t tail = _vm->memmap[last].addr + _vm->memmap[last].size;
  /* realloc memory if AUTO_MEM_SIZE and mem size if too small */
  if (!FLAGS_mem && _vm->mem->size < tail) {
    /* XXX: tail + EFI section */
    size_t size = ((tail / align) + 2) * align;
    _vm->mem = realloc_mem(_vm->mem, size);
  }

  /* load sections */
  for (int i = 0; i < fhdr->NumberOfSections; i++) {
    sechdr = (IMAGE_SECTION_HEADER *)
      (addr + doshdr->e_lfanew + sizeof(IMAGE_NT_HEADERS)
       + sizeof(IMAGE_DATA_DIRECTORY) * opthdr->NumberOfRvaAndSizes
       + sizeof(IMAGE_SECTION_HEADER) * i);
    if (image_base + sechdr->VirtualAddress > _vm->mem->size)
      raise_except(MEMORY, "out of memory", __FILE__, __LINE__);
    void *dst = _vm->mem->body + image_base + sechdr->VirtualAddress;
    void *src = (void *)addr + sechdr->PointerToRawData;
    size_t n = sechdr->Misc.VirtualSize;
    memcpy(dst, src, n);
  }

  /* setup registers */
  _vm->regs->regs[IP] = image_base + entry_point;
  _vm->regs->regs[R0] = (((FLAGS_heap + FLAGS_stack) / align) + 1) * align;
  write_mem64(_vm->mem, _vm->regs->regs[R0], STACK_MAGIC);

  /* calculate efi entry point address */
  uint64_t efi_addr = ((tail / align) + 1 ) * align;

  _vm = load_efi(efi_addr, _vm);

  return _vm;

unsupported:
  error("unsupported file");
  return NULL;

fail:
  error("could not load file");
  return NULL;
}

vm *load_exe(const char *path, vm *_vm) {
  int fd = open(path, O_RDONLY);
  if (fd < 0)
    error("could not open file");

  struct stat sb;
  if (stat(path, &sb))
    error("stat failed");

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
