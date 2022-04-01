#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/IoLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/ShellLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

#include <Library/ECCommandLib.h>

VOID
WaitForIbf (
    IN  UINT8   CommandPort
)
{
    UINT8   Ibf;
    UINTN   Time = 100;
    
    while (Time--) {
        gBS->Stall (100);
        Ibf = IoRead8 (CommandPort) & 0x02;
        if (Ibf == 0x00) {
            break;
        }
    }
}

VOID
WaitForObf (
    IN  UINT8   CommandPort
)
{
    UINT8   Obf;
    UINTN   Time = 100;

    while (Time--) {
        gBS->Stall (100);
        Obf = IoRead8 (CommandPort) & 0x01;
        if (Obf == 0x01) {
            break;
        }
    }
}

BOOLEAN
IsObfFull (
    IN  UINT8   CommandPort
)
{
    UINT8       Obf;
    BOOLEAN     IsItFull;

    Obf = IoRead8 (CommandPort) & 0x01;
    if (Obf == 0x01) {
        IsItFull = TRUE;
    } else {
        IsItFull = FALSE;
    }

    return IsItFull;
}

VOID
EcCommandByKbc (
    IN  UINT8   Command,
    IN  UINT8   Data
)
{
    UINT8   OutputArray[10];
    UINT8   OutputData;
    UINT8   OutputLength;
    UINT8   Index = 0;
    UINT8   RegisterTable[256];

    // Disable Keyboard Interface
    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, DISABLE_KB_COMMAND);

    if (Command == 0x41 || Command == 0x52) {

        WaitForIbf (KBC_COMMAND_PORT);
        IoWrite8 (KBC_COMMAND_PORT, Command);

        WaitForIbf (KBC_COMMAND_PORT);
        IoWrite8 (KBC_DATA_PORT, Data);

        Index = 0;
        WaitForObf (KBC_COMMAND_PORT);
        while (IsObfFull (KBC_COMMAND_PORT)) {
            OutputData = IoRead8 (KBC_DATA_PORT);
            OutputArray[Index] = OutputData;
            Index++;
            WaitForObf (KBC_COMMAND_PORT);
        }
        OutputLength = Index;

        for (Index = 0; Index < OutputLength; Index++) {
            Print (L" %02x", OutputArray[Index]);
            Print (L" (%c)\n", OutputArray[Index]);
        }
        Print (L"%a\n", OutputArray);

    } else if (Command == 0x42) {

        if (Data < 0x00 || Data > 0x06) {
            Print (L"Please enter Bank Number between 0 and 6\n");
        } else {

            WaitForIbf (KBC_COMMAND_PORT);
            IoWrite8 (KBC_COMMAND_PORT, Command);

            WaitForIbf (KBC_COMMAND_PORT);
            IoWrite8 (KBC_DATA_PORT, Data);

            for (Index = 0x00; Index <= 0xFF; Index++) {

                WaitForIbf (KBC_COMMAND_PORT);
                IoWrite8 (KBC_COMMAND_PORT, EEPROM_READ_COMMAND);

                WaitForIbf (KBC_COMMAND_PORT);
                IoWrite8 (KBC_DATA_PORT, Index);

                WaitForObf (KBC_COMMAND_PORT);
                OutputData = IoRead8 (KBC_DATA_PORT);
                RegisterTable[Index] = OutputData;

                if (Index == 0xFF) {
                    break;
                }
            }

            FormatRegisterTable256 (RegisterTable);
        }
    } else {
        Print (L"Command or data is not defined\n");
    }

    // Enable Keyboard Interface
    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, ENABLE_KB_COMMAND);
}

VOID
EcCommand (
    IN  UINTN       Argc,
	IN  CHAR16      **Argv
)
{
    EC_COMMAND_STRUCT   *EcCommandStructure;
    UINT8               InputCommand;
    UINT8               InputData[10];

    EcCommandStructure = NULL;

    if (Argc < 3 || Argc > 3) {
        Print (L"     [File Name] [Command] [Data]\ne.g.   ECCMD.efi        41     A1\n");
    } else if (Argc == 3) {

        EcCommandStructure->InputCommand = (UINT8)StrHexToUintn(Argv[1]);
        InputCommand = EcCommandStructure->InputCommand;

        EcCommandStructure->InputData[0] = (UINT8)StrHexToUintn(Argv[2]);
        InputData[0] = EcCommandStructure->InputData[0];

        EcCommandByKbc (InputCommand, InputData[0]);
    }
}

VOID
FormatRegisterTable256 (
    IN  UINT8   Table[]
)
{
    UINT8   Index;
    UINT8   Index1;
    UINT8   Index2;
    UINT8   Index3;

    Print (L"   ");
    for (Index = 0x00; Index <= 0x0F; Index++) {
        Print (L" %02x", Index);
        if (Index == 0x0F) {
            Print (L"\n");
        }
    }

    for (Index1 = 0x00; Index1 <= 0xF0; Index1 += 0x10) {
        Print (L" %02x", Index1);
        for (Index2 = Index1; Index2 <= Index1 + 0x0F; Index2++) {
            Print (L" %02x", Table[Index2]);
            if (Index2 == 0xFF) {
                break;
            }
        }
        for (Index3 = Index1; Index3 <= Index1 + 0x0F; Index3++) {
            Print (L" %1c", Table[Index3]);
            if (Index3 == Index1 + 0x0F) {
                Print (L"\n");
            }
            if (Index3 == 0xFF) {
                break;
            }
        }
        if (Index3 == 0xFF) {
            break;
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

    EFI_GUID  mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

	gEfiShellParametersProtocol = NULL;

	if (gEfiShellParametersProtocol != NULL) {
		Print (L"gEfiShellParametersProtocol is initialized\n");
        Argc = gEfiShellParametersProtocol->Argc;
        Argv = gEfiShellParametersProtocol->Argv;

	} else {
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
		      Print(L"\nSorry, Getting Shell parameters did NOT work or in the EFI Shell 1.0: \n");
			  return EFI_UNSUPPORTED;
		} else {
		    Argc = gEfiShellParametersProtocol->Argc;
			Argv = gEfiShellParametersProtocol->Argv;
		}
	}
    //
    // use shell 2.0 interface
    //

    EcCommand (Argc, Argv);
   
	return EFI_SUCCESS;
}