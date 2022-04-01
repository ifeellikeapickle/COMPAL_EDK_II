#ifndef __EC_COMMAND_LIB_H__
#define __EC_COMMAND_LIB_H__

#define KBC_DATA_PORT           0x60
#define KBC_COMMAND_PORT        0x64

#define ACPI_DATA_PORT          0x62
#define ACPI_COMMAND_PORT       0x66

#define COMPAL_EC_DATA_PORT     0x68
#define COMPAL_EC_COMMAND_PORT  0x6C

#define DISABLE_KB_COMMAND      0xAD
#define ENABLE_KB_COMMAND       0xAE
#define EEPROM_READ_COMMAND     0x4E

typedef struct {
    UINT8                   InputCommand;
    UINT8                   InputData[10];
    UINT8                   InputDataLength;
    UINT8                   OutputData[10];
    UINT8                   OutputDataLength;
} EC_COMMAND_STRUCT;

VOID
WaitForIbf (
    IN  UINT8               CommandPort
);

VOID
WaitForObf (
    IN  UINT8               CommandPort
);

BOOLEAN
IsObfFull (
    IN  UINT8               CommandPort
);

VOID
EcCommandByKbc (
    IN  UINT8               Command,
    IN  UINT8               Data
);

VOID
EcCommand (
    IN  UINTN               Argc,
	IN  CHAR16              **Argv
);

VOID
FormatRegisterTable256 (
    IN  UINT8   Table[256]
);

EFI_STATUS
GetInputParameters (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
);

#endif