# ebcvm

EFI Byte Code Virtual Machine

[ebcvm](https://github.com/yabits/ebcvm) is
an usermode EFI Byte Code (EBC) virtual machine.
ebcvm supports all EBC instructions described in UEFI Specification.
It also supports few UEFI native code emulations.
With ebcvm, you can run EBC binaries on your \*nix userspace.

## Building ebcvm

```
$ make
```

This will build following executables.

* ebcvm:          EBC VM
* tools/ebcdisas: Disassembler for EBC binaries
* tools/fnv1:     Fowler-Noll-Vo hash tool

## Running tests

```
$ make test
```

This will run tests.

## License

MIT
