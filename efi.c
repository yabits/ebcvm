#include "ebcvm.h"
#include "efi.h"

#define ConOut_OutputString_MAGIC 0x928ae98de3f89c23

/* FIXME: below native code emulation supports only 64-bit machine */
static void conout_output_string(vm *_vm) {
  uint64_t stack_top = _vm->regs->regs[R0];
  uint64_t ret_addr = read_mem64(_vm->mem, stack_top);
  uint64_t this = read_mem64(_vm->mem, stack_top + 8);
  uint64_t string = read_mem64(_vm->mem, stack_top + 16);

  for (uint64_t p = string; read_mem16(_vm->mem, p) != '\0'; p += 2)
    fputc((char)read_mem16(_vm->mem, p), stdout);

  _vm->regs->regs[R7] = EFI_SUCCESS;
  /* XXX: POPn */
  _vm->regs->regs[R0] += ARCH_BYTES;
  _vm->regs->regs[IP] = ret_addr;
}

static size_t calc_offset(size_t size) {
  return size + size % ARCH_BYTES;
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

static void set_efi_conout(uint64_t conout, vm *_vm) {
  uint64_t offset = 0;
  offset += calc_offset(sizeof(VOID_PTR)); /* Reset */
  write_mem64(_vm->mem, conout + offset, ConOut_OutputString_MAGIC);
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
  size_t size = boot_addr - addr;

  /* setup pointers */
  uint64_t addrs[] = {
  0, conin_addr, conout_addr, stderr_addr, runtime_addr, boot_addr, 0,
  };
  set_efi_system_table(table_addr, addrs, _vm);
  set_efi_conout(conout_addr, _vm);

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

void handle_excall(uint64_t addr, vm *_vm) {
  if (addr == ConOut_OutputString_MAGIC)
    conout_output_string(_vm);
  else
    raise_except(UNDEF, "invalid excall");
}
