#ifndef __EC_COMMAND_LIB_H__
#define __EC_COMMAND_LIB_H__

#define KBC_DATA_PORT           0x60
#define KBC_COMMAND_PORT        0x64

#define ACPI_DATA_PORT          0x62
#define ACPI_COMMAND_PORT       0x66

#define COMPAL_EC_DATA_PORT     0x68
#define COMPAL_EC_COMMAND_PORT  0x6C

#define DISABLE_KB_INTERFACE    0xAD
#define ENABLE_KB_INTERFACE     0xAE
#define EEPROM_BANK_ASSIGN      0x42
#define EEPROM_READ_BYTE        0x4E

typedef struct {
    UINT8                   InputCommand;
    UINT8                   InputData[10];
    UINT8                   InputDataLength;
    UINT8                   OutputData[10];
    UINT8                   OutputDataLength;
} EC_COMMAND_STRUCT;

VOID
EFIAPI
WaitForIbf (
    IN  UINT8               CommandPort
);

VOID
EFIAPI
WaitForObf (
    IN  UINT8               CommandPort
);

BOOLEAN
EFIAPI
IsObfFull (
    IN  UINT8               CommandPort
);

VOID
EFIAPI
EepromBankAssign (
    IN  UINT8   Bank
);

UINT8
EFIAPI
EepromReadByte (
    IN  UINT8   Offset
);

VOID
EFIAPI
EcCommandByKbc (
    IN  UINT8               Command,
    IN  UINT8               Data
);

VOID
EFIAPI
EcCommand (
    IN  UINTN               Argc,
	IN  CHAR16              **Argv
);

VOID
EFIAPI
FormatRegisterTable256 (
    IN  UINT8   Table[256]
);

#endif