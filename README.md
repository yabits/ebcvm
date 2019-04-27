# ebcvm [![CircleCI](https://circleci.com/gh/yabits/ebcvm.svg?style=shield)](https://circleci.com/gh/yabits/ebcvm)

EFI Byte Code Virtual Machine in Userspace

[ebcvm](https://github.com/yabits/ebcvm) is
a usermode EFI Byte Code (EBC) virtual machine.
It supports all EBC instructions described in UEFI Specification.
It also supports a few UEFI native code emulations and a simple debugger.

## Building ebcvm

To build ebcvm, simply run:

```
$ make
```

This will build following executables.

* ebcvm:          EBC VM
* tools/ebcdisas: Disassembler for EBC binaries
* tools/fnv1:     Fowler-Noll-Vo hash tool

To run tests, run:

```
$ make test
```

## Usage

```
Usage: ./ebcvm [OPTION] FILE
EFI Byte Code Interpreter
[OPTION]
  --debug={0,1}   debug mode
  --mem=INT   size of memory
  --stack=INT   size of stack
  --heap=INT    size of heap
  --step={0,1}    step execution
  --reloc={0,1}   relocate sections
  --help    show this help
```

## Working with ELVM

Since there is no single EBC binary available in public,
we created an EBC backend for
[ELVM](https://github.com/shinh/elvm).

To generate EBC binaries, run:

```
$ git clone https://github.com/retrage/elvm.git
$ cd elvm && git checkout retrage/ebcvm
$ make ebc
```

The ELVM generates EBC binaries from EIR and C using modified
[8cc](https://github.com/rui314/8cc)
including 8cc compiler itself.

## EFI native code emulations

ebcvm has following native code emulations:

* EFI\_BOOT\_SERVICES.AllocatePool()
* EFI\_SYSTEM\_TABLE.ConIn.ReadKeyStroke()
* EFI\_SYSTEM\_TABLE.ConOut.OutputString()

## License

ebcvm is released under the [MIT license](LICENSE).
