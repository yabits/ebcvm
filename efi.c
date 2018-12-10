#include "ebcvm.h"
#include "efi.h"

#define ConOut_OutputString_MAGIC 0xdbec42e5063aa942
#define ConIn_ReadKeyStroke_MAGIC 0xa1b4e93c32d682d0
#define BS_AllocatePool_MAGIC 0xd61390cb796062bb

static size_t calc_offset(size_t size) {
  return size + size % ARCH_BYTES;
}

static void bs_allocate_pool(vm *_vm) {
  uint64_t stack_top = _vm->regs->regs[R0];
  uint64_t ret_addr = read_mem64(_vm->mem, stack_top);
  uint64_t pool_type = read_mem64(_vm->mem, stack_top + 8);
  uint64_t size = read_mem64(_vm->mem, stack_top + 16);
  uint64_t buffer = read_mem64(_vm->mem, stack_top + 24);

  if (size > FLAGS_heap)
    raise_except(MEMORY, "out of memory");

  /* FIXME: Add sane memory allocator */
  uint64_t heap_addr = 0xffffffffffffffff;
  for (int i = 0; i < _vm->memmap_size; i++) {
    if (_vm->memmap[i].mem_type == MEM_HEAP)
      heap_addr = _vm->memmap[i].addr;
  }
  if (heap_addr == 0xffffffffffffffff)
    raise_except(MEMORY, "heap not found");

  write_mem64(_vm->mem, buffer, heap_addr);

  _vm->regs->regs[R7] = EFI_SUCCESS;
  /* XXX: POPn */
  _vm->regs->regs[R0] += ARCH_BYTES;
  _vm->regs->regs[IP] = ret_addr;
}

static void conin_read_key_stroke(vm *_vm) {
  uint64_t stack_top = _vm->regs->regs[R0];
  uint64_t ret_addr = read_mem64(_vm->mem, stack_top);
  uint64_t this = read_mem64(_vm->mem, stack_top + 8);
  uint64_t key = read_mem64(_vm->mem, stack_top + 16);

  int c = fgetc(stdin);
  if (c == EOF)
    c = 0x00;
  /* FIXME: encode ASCII to UTF-8 */
  uint64_t offset = 0;
  write_mem16(_vm->mem, key + offset, (UINT16)0x00);
  offset += calc_offset(sizeof(UINT16));
  write_mem16(_vm->mem, key + offset, (CHAR16)c);

  _vm->regs->regs[R7] = EFI_SUCCESS;
  /* XXX: POPn */
  _vm->regs->regs[R0] += ARCH_BYTES;
  _vm->regs->regs[IP] = ret_addr;
}


/* FIXME: below native code emulation supports only 64-bit machine */
static void conout_output_string(vm *_vm) {
  uint64_t stack_top = _vm->regs->regs[R0];
  uint64_t ret_addr = read_mem64(_vm->mem, stack_top);
  uint64_t this = read_mem64(_vm->mem, stack_top + 8);
  uint64_t string = read_mem64(_vm->mem, stack_top + 16);

  for (uint64_t p = string; read_mem16(_vm->mem, p) != 0xffff; p += 2)
    fputc((char)read_mem16(_vm->mem, p), stdout);

  _vm->regs->regs[R7] = EFI_SUCCESS;
  /* XXX: POPn */
  _vm->regs->regs[R0] += ARCH_BYTES;
  _vm->regs->regs[IP] = ret_addr;
}

static void set_efi_system_table(uint64_t table, uint64_t addrs[], vm *_vm) {
  uint64_t offset = 0;
  offset += calc_offset(sizeof(EFI_TABLE_HEADER));  /* Hdr */
  write_mem64(_vm->mem, table + offset, addrs[0]);
  offset += calc_offset(sizeof(VOID_PTR));          /* FirmwareVendor */
  offset += calc_offset(sizeof(UINT32));            /* FirmwareRevision */
  offset += calc_offset(sizeof(EFI_HANDLE));        /* ConsoleInHandle */
  write_mem64(_vm->mem, table + offset, addrs[1]);
  offset += calc_offset(sizeof(VOID_PTR));          /* ConIn */
  offset += calc_offset(sizeof(EFI_HANDLE));        /* ConsoleOutHandle */
  write_mem64(_vm->mem, table + offset, addrs[2]);
  offset += calc_offset(sizeof(VOID_PTR));          /* ConOut */
  offset += calc_offset(sizeof(EFI_HANDLE));        /* StandardErrorHandle */
  write_mem64(_vm->mem, table + offset, addrs[3]);
  offset += calc_offset(sizeof(VOID_PTR));          /* StdErr */
  write_mem64(_vm->mem, table + offset, addrs[4]);
  offset += calc_offset(sizeof(VOID_PTR));          /* RuntimeServices */
  write_mem64(_vm->mem, table + offset, addrs[5]);
  offset += calc_offset(sizeof(VOID_PTR));          /* BootServices */
  /* FIXME: NumberOfTableEntries */
  /* FIXME: ConfigurationTable */
}

static void set_efi_conin(uint64_t conin, vm *_vm) {
  uint64_t offset = 0;
  offset += calc_offset(sizeof(VOID_PTR)); /* Reset */
  write_mem64(_vm->mem, conin + offset, ConIn_ReadKeyStroke_MAGIC);
}

static void set_efi_conout(uint64_t conout, vm *_vm) {
  uint64_t offset = 0;
  offset += calc_offset(sizeof(VOID_PTR)); /* Reset */
  write_mem64(_vm->mem, conout + offset, ConOut_OutputString_MAGIC);
}

static void set_efi_bs(uint64_t bs, vm *_vm) {
  uint64_t offset = 0;
  offset += calc_offset(sizeof(EFI_TABLE_HEADER)); /* Hdr */
  offset += calc_offset(sizeof(VOID_PTR)); /* RaiseTPL */
  offset += calc_offset(sizeof(VOID_PTR)); /* RestoreTPL */
  offset += calc_offset(sizeof(VOID_PTR)); /* AllocatePages */
  offset += calc_offset(sizeof(VOID_PTR)); /* FreePages */
  offset += calc_offset(sizeof(VOID_PTR)); /* GetMemoryMap */
  write_mem64(_vm->mem, bs + offset, BS_AllocatePool_MAGIC);
}

vm *load_efi(uint64_t addr, vm *_vm) {
  /* calculate addresses */
  uint64_t params_addr = addr;
  uint64_t table_addr = params_addr + sizeof(EFI_MAIN_PARAMETERS);
  /* FIXME: FirmwareVendor */
  uint64_t conin_addr = table_addr + sizeof(EFI_SYSTEM_TABLE);
  uint64_t conout_addr = conin_addr + sizeof(EFI_SIMPLE_TEXT_IN_PROTOCOL);
  uint64_t stderr_addr = conout_addr + sizeof(EFI_SIMPLE_TEXT_OUT_PROTOCOL);
  uint64_t runtime_addr = stderr_addr + sizeof(EFI_SIMPLE_TEXT_OUT_PROTOCOL);
  uint64_t boot_addr = runtime_addr + sizeof(EFI_RUNTIME_SERVICES);
  /* FIXME: ConfigurationTable */
  size_t size = boot_addr + sizeof(EFI_BOOT_SERVICES) - addr;

  /* setup pointers */
  uint64_t addrs[] = {
  0, conin_addr, conout_addr, stderr_addr, runtime_addr, boot_addr, 0,
  };
  set_efi_system_table(table_addr, addrs, _vm);
  set_efi_conin(conin_addr, _vm);
  set_efi_conout(conout_addr, _vm);
  set_efi_bs(boot_addr, _vm);

  _vm->memmap_size += 1;
  _vm->memmap = realloc(_vm->memmap, sizeof(memmap) * _vm->memmap_size);
  if (!_vm->memmap)
    goto fail;

  _vm->memmap[_vm->memmap_size - 1].mem_type = MEM_EFI;
  _vm->memmap[_vm->memmap_size - 1].addr = addr;
  _vm->memmap[_vm->memmap_size - 1].size = size;

  /* XXX: PUSH64 SystemTable */
  _vm->regs->regs[R0] -= 8;
  write_mem64(_vm->mem, _vm->regs->regs[R0], table_addr);
  /* XXX: PUSH64 ImageHandle */
  _vm->regs->regs[R0] -= 8;
  write_mem64(_vm->mem, _vm->regs->regs[R0], 0x0000000000000000);
  /* XXX: PUSH64 RET_MAGIC */
  _vm->regs->regs[R0] -= 16;
  write_mem64(_vm->mem, _vm->regs->regs[R0], RET_MAGIC);

  return _vm;

fail:
  error("could not load efi");
  return NULL;
}

void handle_excall(uint64_t code, vm *_vm) {
  switch (code) {
    case ConIn_ReadKeyStroke_MAGIC:
      conin_read_key_stroke(_vm);
      break;
    case ConOut_OutputString_MAGIC:
      conout_output_string(_vm);
      break;
    case BS_AllocatePool_MAGIC:
      bs_allocate_pool(_vm);
      break;
    default:
      raise_except(UNDEF, "invalid excall");
  }
}
