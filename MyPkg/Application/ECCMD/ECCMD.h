#include <Library/IoLib.h>

//Related to input function
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ShellLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

//MicroSecondDelay() function
#include <Library\TimerLib.h>

#define KBC_DATA_PORT           0x60
#define KBC_COMMAND_PORT        0x64

#define ACPI_DATA_PORT          0x62
#define ACPI_COMMAND_PORT       0x66

#define COMPAL_EC_DATA_PORT     0x68
#define COMPAL_EC_COMMAND_PORT  0x6C

VOID
WaitForIbf (
    IN  UINT8   CommandPort
)
{
    while ((IoRead8 (CommandPort) & 0x02) != 0x00) {}
}

VOID
WaitForObf (
    IN  UINT8   CommandPort
)
{
    while ((IoRead8 (CommandPort) & 0x01) == 0x00) {}
}

BOOLEAN
IsObfFull (
    IN  UINT8   CommandPort
)
{
    if ((IoRead8 (CommandPort) & 0x01) == 0x01) {
        return TRUE;
    } else {
        return FALSE;
    }
}

VOID
PrintIbfObf (VOID)
{
    UINT8   Ibf;
    UINT8   Obf;

    Ibf = (IoRead8 (KBC_COMMAND_PORT) & 0x02) >> 1;
    Obf = IoRead8 (KBC_COMMAND_PORT) & 0x01;
    Print (L"IBF: %d\nOBF: %d\n", Ibf, Obf);
}

VOID
PrintCompanyIdByKbc (VOID)
{
    UINT8   CompanyId[10];
    UINTN   Index = 0;

    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, 0x41);

    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_DATA_PORT, 0xA1);

    while (IsObfFull (KBC_COMMAND_PORT)) {
        WaitForObf (KBC_COMMAND_PORT);
        CompanyId[Index] = IoRead8 (KBC_DATA_PORT);
        Print (L" %x", CompanyId[Index]);
        Print (L" (%c)\n", CompanyId[Index]);
        Index++;
    }
    Print (L"%a\n", CompanyId);
}

VOID
PrintCompanyIdByAcpi (VOID)
{
    UINT8   CompanyId2[10];
    UINTN   Index = 0;

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_COMMAND_PORT, 0x41);

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_DATA_PORT, 0xA1);

    WaitForObf (ACPI_COMMAND_PORT);
    while (IsObfFull (ACPI_COMMAND_PORT)) {
        CompanyId2[Index] = IoRead8 (ACPI_DATA_PORT);
        Print (L" %x", CompanyId2[Index]);
        Print (L" (%c)\n", CompanyId2[Index]);
        Index++;
        MicroSecondDelay (100000);
    }
    Print (L"%a\n", CompanyId2);
}

VOID
PrintProjectNameByAcpi (VOID)
{
    UINT8   ProjectName[10];
    UINTN   Index = 0;

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_COMMAND_PORT, 0x52);

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_DATA_PORT, 0xA0);

    WaitForObf (ACPI_COMMAND_PORT);

    while (IsObfFull (ACPI_COMMAND_PORT)) {
        ProjectName[Index] = IoRead8 (ACPI_DATA_PORT);
        Print (L" %x", ProjectName[Index]);
        Print (L" (%c)\n", ProjectName[Index]);
        Index++;
    }
    Print (L"%a\n", ProjectName);
}

VOID
PrintEcEeprom (
    IN  UINT8   Bank
)
{
    UINTN   Index;
    UINT8   Reg;

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_COMMAND_PORT, 0x42);

    WaitForIbf (ACPI_COMMAND_PORT);
    IoWrite8 (ACPI_DATA_PORT, Bank);

    for (Index = 0x00; Index <= 0xFF; Index ++) {
        WaitForIbf (ACPI_COMMAND_PORT);
        IoWrite8 (ACPI_COMMAND_PORT, 0x4E);
        WaitForIbf (ACPI_COMMAND_PORT);
        IoWrite8 (ACPI_DATA_PORT, (UINT8)Index);
        WaitForObf (ACPI_COMMAND_PORT);
        Reg = IoRead8 (ACPI_DATA_PORT);
        Print (L" %02x", Reg);
        if (Index % 0x10 == 0x0F) {
            Print (L"\n");
        }
    }
}

EFI_STATUS
GetInputParameters (
    IN  EFI_HANDLE        ImageHandle,
    IN  EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;

    UINTN			Argc;
	CHAR16			**Argv;
    EFI_STATUS      Status;
    UINT8           BankNumber;
    EFI_GUID  mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

	gEfiShellParametersProtocol = NULL;

	if (gEfiShellParametersProtocol != NULL){
		Print (L"gEfiShellParametersProtocol is initialized\n");
        Argc = gEfiShellParametersProtocol->Argc;
        Argv = gEfiShellParametersProtocol->Argv;

	}else {
        // check out input parameters from command line using UEFI Shell 2.0
		Status = gBS->OpenProtocol(
            ImageHandle,
            &mEfiShellParametersProtocolGuid,
            (VOID **)&gEfiShellParametersProtocol,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        ); 
		if (EFI_ERROR(Status)) {
		      Print(L"\nSorry, Getting Shell params did NOT work or in the EFI Shell 1.0: \n");
			  return EFI_UNSUPPORTED;
		}else{
		    Argc = gEfiShellParametersProtocol->Argc;
			Argv = gEfiShellParametersProtocol->Argv;
		}
	}
    //
    // use shell 2.0 interface
    //

   if (Argc == 3 && !StrCmp(Argv[1], L"41") && (!StrCmp(Argv[2], L"A1") || !StrCmp(Argv[2], L"a1"))) {
       PrintCompanyIdByAcpi ();
   } else if (Argc == 3 && !StrCmp(Argv[1], L"52") && (!StrCmp(Argv[2], L"A0") || !StrCmp(Argv[2], L"a0"))) {
       PrintProjectNameByAcpi ();
   } else if (Argc == 3 && !StrCmp(Argv[1], L"42")) {
       BankNumber = (UINT8)StrHexToUintn(Argv[2]);
       if (BankNumber < 0x00 || BankNumber > 0x06) {
           Print (L"Please enter Bank Number between 0 and 6.\n");
       } else {
           PrintEcEeprom (BankNumber);
       }
   } else if (Argc == 1) {
       Print (L"     [File Name] [Command] [Data]\ne.g.   ECCMD.efi        41     A1\n");
   } else {
       Print (L"Command or data are not defined.\n");
   }
   
	return EFI_SUCCESS;
}