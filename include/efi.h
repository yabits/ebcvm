#ifndef EFI_H_
#define EFI_H_

#include "ebcvm.h"

#define IN
#define OUT

#define TRUE true
#define FALSE false

#define EFI_SUCCESS 0x0000000000000000
#define EFI_ERROR   0x8000000000000000

typedef uint8_t   BOOLEAN;
typedef int8_t    INT8;
typedef uint8_t   UINT8;
typedef uint8_t   CHAR8;
typedef int16_t   INT16;
typedef uint16_t  UINT16;
typedef uint16_t  CHAR16;
typedef int32_t   INT32;
typedef uint32_t  UINT32;
typedef int64_t   INT64;
typedef uint64_t  UINT64;
#if ARCH_BYTES == 4
typedef int32_t   INTN;
typedef uint32_t  UINTN;
#elif ARCH_BYTES == 8
typedef int64_t   INTN;
typedef uint64_t  UINTN;
#else
#error unsupported architecture
#endif
typedef UINTN   VOID_PTR;
typedef UINTN   EFI_STATUS;
typedef UINTN   EFI_HANDLE;

typedef struct EFI_MAIN_PARAMETERS {
  EFI_HANDLE        ImageHandle;
  VOID_PTR          SystemTable; 
} EFI_MAIN_PARAMETERS;

typedef struct EFI_TABLE_HEADER {
  UINT64            Signature;
  UINT32            Revision;
  UINT32            HeaderSize;
  UINT32            CRC32;
  UINT32            Reserved;
} EFI_TABLE_HEADER;

typedef struct EFI_SYSTEM_TABLE {
  EFI_TABLE_HEADER  Hdr;
  VOID_PTR          FirmwareVendor;
  UINT32            FirmwareRevision;
  EFI_HANDLE        ConsoleInHandle;
  VOID_PTR          ConIn;
  EFI_HANDLE        ConsoleOutHandle;
  VOID_PTR          ConOut;
  EFI_HANDLE        StandardErrorHandle;
  VOID_PTR          StdErr;
  VOID_PTR          RuntimeServices;
  VOID_PTR          BootServices;
  UINTN             NumberOfTableEntries;
  VOID_PTR          ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct EFI_RUNTIME_SERVICES {
  EFI_TABLE_HEADER  Hdr;
  VOID_PTR          GetTime;
  VOID_PTR          SetTime;
  VOID_PTR          GetWakeupTime;
  VOID_PTR          SetWakeupTime;
  VOID_PTR          SetVirtualAddressMap;
  VOID_PTR          ConvertPointer;
  VOID_PTR          GetVariable;
  VOID_PTR          GetNextVariableName;
  VOID_PTR          SetVariable;
  VOID_PTR          GetNextHighMonotonicCount;
  VOID_PTR          ResetSystem;
  VOID_PTR          UpdateCapsule;
  VOID_PTR          QueryCapsuleCapabilities;
  VOID_PTR          QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

typedef struct EFI_SIMPLE_TEXT_OUT_PROTOCOL {
  VOID_PTR          Reset;
  VOID_PTR          OutputString;
  VOID_PTR          TestString;
  VOID_PTR          QueryMode;
  VOID_PTR          SetMode;
  VOID_PTR          SetAttribute;
  VOID_PTR          ClearScreen;
  VOID_PTR          SetCursorPosition;
  VOID_PTR          EnableCursor;
  VOID_PTR          Mode;
} EFI_SIMPLE_TEXT_OUT_PROTOCOL;

typedef struct EFI_SIMPLE_TEXT_IN_PROTOCOL {
  VOID_PTR          Reset;
  VOID_PTR          ReadKeyStroke;
  VOID_PTR          WaitForKey;
} EFI_SIMPLE_TEXT_IN_PROTOCOL;

typedef struct EFI_BOOT_SERVICES {
  EFI_TABLE_HEADER  Hdr;
  VOID_PTR          RaiseTPL;
  VOID_PTR          RestoreTPL;
  VOID_PTR          AllocatePages;
  VOID_PTR          FreePages;
  VOID_PTR          GetMemoryMap;
  VOID_PTR          AllocatePool;
  VOID_PTR          FreePool;
  VOID_PTR          CreateEvent;
  VOID_PTR          SetTimer;
  VOID_PTR          WaitForEvent;
  VOID_PTR          SignalEvent;
  VOID_PTR          CloseEvent;
  VOID_PTR          CheckEvent;
  VOID_PTR          InstallProtocolInterface;
  VOID_PTR          ReinstallProtocolInterface;
  VOID_PTR          UninstallProtocolInterface;
  VOID_PTR          HandleProtocol;
  VOID_PTR          PCHandleProtocol;
  VOID_PTR          RegisterProtocolNotify;
  VOID_PTR          LocateHandle;
  VOID_PTR          LocateDevicePath;
  VOID_PTR          InstallConfigurationTable;
  VOID_PTR          LoadImage;
  VOID_PTR          StartImage;
  VOID_PTR          Exit;
  VOID_PTR          UnloadImage;
  VOID_PTR          ExitBootServices;
  VOID_PTR          GetNextMonotonicCount;
  VOID_PTR          Stall;
  VOID_PTR          SetWatchdogTimer;
  VOID_PTR          ConnectController;
  VOID_PTR          DisconnectController;
  VOID_PTR          OpenProtocolInformation;
  VOID_PTR          ProtocolPerHandle;
  VOID_PTR          LocateHandleBuffer;
  VOID_PTR          LocateProtocol;
  VOID_PTR          InstallMultipleProtocolInterfaces;
  VOID_PTR          UninstallMultipleProtocolInterfaces;
  VOID_PTR          CalculateCrc32;
  VOID_PTR          CopyMem;
  VOID_PTR          SetMem;
  VOID_PTR          CreateEventEx;
} EFI_BOOT_SERVICES;

typedef struct EFI_INPUT_KEY {
  UINT16            ScanCode;
  CHAR16            UnicodeChar;
} EFI_INPUT_KEY;

#endif /* EFI_H_ */
