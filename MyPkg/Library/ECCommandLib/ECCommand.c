#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/UefiBootServicesTableLib.h>

#include <Library/IoLib.h>

#include <Library/ECCommandLib.h>

VOID
EFIAPI
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
EFIAPI
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
EFIAPI
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
EFIAPI
DisableKeyboardInterface (
    VOID
)
{
    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, DISABLE_KB_INTERFACE);
}

VOID
EFIAPI
EnableKeyboardInterface (
    VOID
)
{
    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, ENABLE_KB_INTERFACE);
}

VOID
EFIAPI
EepromAssignBank (
    IN  UINT8   Bank
)
{
    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, EEPROM_ASSIGN_BANK);

    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_DATA_PORT, Bank);
}

UINT8
EFIAPI
EepromReadByte (
    IN  UINT8   Offset
)
{
    UINT8   Data;

    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_COMMAND_PORT, EEPROM_READ_BYTE);

    WaitForIbf (KBC_COMMAND_PORT);
    IoWrite8 (KBC_DATA_PORT, Offset);

    WaitForObf (KBC_COMMAND_PORT);
    Data = IoRead8 (KBC_DATA_PORT);
    
    return Data;
}

/*
VOID
EFIAPI
EcCommandByKbc (
    IN  UINT8   Command,
    IN  UINT8   Data,
    IN  UINT8   Offset,     OPTIONAL
    IN  UINT8   WriteData   OPTIONAL
)
{
    UINT8   OutputArray[10];
    UINT8   OutputData;
    UINT8   OutputLength;
    UINT8   Index = 0;
    UINT8   RegisterTable[256];

    DisableKeyboardInterface ();

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

            EepromAssignBank (Data);

            for (Index = 0x00; Index <= 0xFF; Index++) {

                RegisterTable[Index] = EepromReadByte (Index);

                if (Index == 0xFF) {
                    break;
                }
            }

            FormatRegisterTable256 (RegisterTable);
        }
    } else {
        Print (L"Command or data is not defined\n");
    }

    EnableKeyboardInterface ();
}
*/

VOID
EFIAPI
EcCommand (
    IN  UINTN       Argc,
	IN  CHAR16      **Argv
)
{
    EC_COMMAND_STRUCT   *EcCommandStructure;
    UINT8               Command;
    UINT8               Data[10];
    UINT8               DataLength;
    UINT8               OutputData;
    UINT8               OutputArray[10];
    UINT8               OutputLength;
    UINT8               EepromTable[256];
    UINT8               Index;

    EcCommandStructure = NULL;

    if (Argc < 3 || Argc > 5) {
        Print (L"     [File Name] [Command] [Data]\ne.g.   ECCMD.efi        41     A1\n");
    } else {
        Command = (UINT8)StrHexToUintn(Argv[1]);
        for (Index = 0; Index < Argc - 2; Index++) {
            Data[Index] = (UINT8)StrHexToUintn(Argv[Index + 2]);
        }
        DataLength = (UINT8)(Argc - 2);

        DisableKeyboardInterface ();
    
        if (Command == COMPANY_ID_COMMAND || Command == PROJECT_NAME_COMMAND) {

            WaitForIbf (KBC_COMMAND_PORT);
            IoWrite8 (KBC_COMMAND_PORT, Command);

            WaitForIbf (KBC_COMMAND_PORT);
            IoWrite8 (KBC_DATA_PORT, Data[0]);

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

        } else if (Command == EEPROM_ASSIGN_BANK) {
            if (Data[0] < 0 || Data[0] > 6) {
                Print (L"Please enter Bank Number between 0 and 6\n");
            } else if (DataLength == 1) {
                EepromAssignBank (Data[0]);
                for (Index = 0x00; Index <= 0xFF; Index++) {
                    EepromTable[Index] = EepromReadByte (Index);
                    if (Index == 0xFF) {
                        break;
                    }
                }
                FormatRegisterTable256 (EepromTable);
            }
        } else {
            
        }

        EnableKeyboardInterface ();
    }
}

VOID
EFIAPI
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