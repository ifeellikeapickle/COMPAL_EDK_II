#ifndef __EC_COMMAND_LIB_H__
#define __EC_COMMAND_LIB_H__

#define KBC_DATA_PORT           0x60
#define KBC_COMMAND_PORT        0x64

#define ACPI_DATA_PORT          0x62
#define ACPI_COMMAND_PORT       0x66

#define COMPAL_EC_DATA_PORT     0x68
#define COMPAL_EC_COMMAND_PORT  0x6C

VOID
WaitForIbf (
    IN  UINT8   CommandPort
);

VOID
WaitForObf (
    IN  UINT8   CommandPort
);

BOOLEAN
IsObfFull (
    IN  UINT8   CommandPort
);

VOID
PrintIbfObf (
    VOID
);

VOID
PrintCompanyIdByCompal (
    VOID
);

VOID
PrintProjectNameByKbc (
    VOID
);

VOID
PrintProjectNameByAcpi (
    VOID
);

VOID
PrintProjectNameByCompal (
    VOID
);

VOID
PrintEcEeprom (
    IN  UINT8   Bank
);

VOID
EcCommand (
    IN  UINTN       Argc,
	IN  CHAR16      **Argv
);

EFI_STATUS
GetInputParameters (
    IN  EFI_HANDLE        ImageHandle,
    IN  EFI_SYSTEM_TABLE  *SystemTable
);

#endif