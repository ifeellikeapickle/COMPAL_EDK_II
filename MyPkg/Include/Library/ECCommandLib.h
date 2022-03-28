#ifndef __EC_COMMAND_LIB__
#define __EC_COMMAND_LIB__

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

#endif