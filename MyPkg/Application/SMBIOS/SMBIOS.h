//SMBIOS GUID
#include <Guid/SmBios.h>

//SMBIOS structure
#include <IndustryStandard/SmBios.h>

//Memory
#include <Library/BaseMemoryLib.h> //CompareGuid
#include <Library/MemoryAllocationLib.h>

//Shell Library
#include <Library/ShellLib.h> //ShellPkg
#include <Library/UefiBootServicesTableLib.h>

//UefiShellDebug1CommandsLib
#include <Library/BcfgCommandLib.h>
#include <Library/DebugLib.h>

#define DMI_SUCCESS                     0x00
#define DMI_INVALID_HANDLE              0x83
#define INVALID_HANDLE                  (UINT16) (-1)

STATIC UINT8                        mInit         = 0;
STATIC SMBIOS_TABLE_ENTRY_POINT     *mSmbiosTable   = NULL;
STATIC SMBIOS_STRUCTURE_POINTER     m_SmbiosStruct;
STATIC SMBIOS_STRUCTURE_POINTER     *mSmbiosStruct = &m_SmbiosStruct;

EFI_STATUS
EFIAPI
GetSystemConfigurationTable (
    IN  EFI_GUID    *TableGuid,
    IN  OUT VOID    **Table
)
{
    UINTN Index;
    ASSERT (Table != NULL);

    for (Index = 0; Index < gST->NumberOfTableEntries; Index++) {
    if (CompareGuid (TableGuid, &(gST->ConfigurationTable[Index].VendorGuid))) {
        *Table = gST->ConfigurationTable[Index].VendorTable;
        return EFI_SUCCESS;
    }
    }

    return EFI_NOT_FOUND;
}

EFI_STATUS
LibSmbiosInit (
    VOID
)
{
    EFI_STATUS  Status;

    //
    // Init only once
    //
    if (mInit == 1) {
    return EFI_SUCCESS;
    }
    //
    // Get SMBIOS table from System Configure table
    //
    Status = GetSystemConfigurationTable (&gEfiSmbiosTableGuid, (VOID**)&mSmbiosTable);

    if (mSmbiosTable == NULL) {
    return EFI_NOT_FOUND;
    }

    if (EFI_ERROR (Status)) {
    //ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_LIBSMBIOSVIEW_GET_TABLE_ERROR), gShellDebug1HiiHandle, Status);
    Print(L"Get SMBIOS Table error.\n");
    return Status;
    }
    //
    // Init SMBIOS structure table address
    //
    mSmbiosStruct->Raw  = (UINT8 *) (UINTN) (mSmbiosTable->TableAddress);

    mInit               = 1;
    return EFI_SUCCESS;
}

VOID
LibSmbiosGetEPS (
    OUT SMBIOS_TABLE_ENTRY_POINT    **EntryPointStructure
)
{
    //
    // return SMBIOS Table address
    //
    *EntryPointStructure = mSmbiosTable;
}

CHAR8*
LibGetSmbiosString (
    IN  SMBIOS_STRUCTURE_POINTER    *Smbios,
    IN  UINT16                      StringNumber
)
{
    UINT16  Index;
    CHAR8   *String;

    ASSERT (Smbios != NULL);

    //
    // Skip over formatted section
    //
    String = (CHAR8 *) (Smbios->Raw + Smbios->Hdr->Length);

    //
    // Look through unformated section
    //
    for (Index = 1; Index <= StringNumber; Index++) {
    if (StringNumber == Index) {
        return String;
    }
    //
    // Skip string
    //
    for (; *String != 0; String++);
    String++;

    if (*String == 0) {
        //
        // If double NULL then we are done.
        //  Return pointer to next structure in Smbios.
        //  if you pass in a -1 you will always get here
        //
        Smbios->Raw = (UINT8 *)++String;
        return NULL;
    }
    }

    return NULL;
}

EFI_STATUS
LibGetSmbiosStructure (
    IN  OUT UINT16      *Handle,
    OUT     UINT8       **Buffer,
    OUT     UINT16      *Length
)
{
    SMBIOS_STRUCTURE_POINTER  Smbios;
    SMBIOS_STRUCTURE_POINTER  SmbiosEnd;
    UINT8                     *Raw;

    if (*Handle == INVALID_HANDLE) {
    *Handle = mSmbiosStruct->Hdr->Handle;
    return DMI_INVALID_HANDLE;
    }

    if ((Buffer == NULL) || (Length == NULL)) {
    //ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_SMBIOSVIEW_LIBSMBIOSVIEW_NO_BUFF_LEN_SPEC), gShellDebug1HiiHandle);
    Print (L"Get SMBIOS Structure, no buffer or length specified!\n");
    return DMI_INVALID_HANDLE;
    }

    *Length       = 0;
    Smbios.Hdr    = mSmbiosStruct->Hdr;
    SmbiosEnd.Raw = Smbios.Raw + mSmbiosTable->TableLength;
    while (Smbios.Raw < SmbiosEnd.Raw) {
    if (Smbios.Hdr->Handle == *Handle) {
        Raw = Smbios.Raw;
        //
        // Walk to next structure
        //
        LibGetSmbiosString (&Smbios, (UINT16) (-1));
        //
        // Length = Next structure head - this structure head
        //
        *Length = (UINT16) (Smbios.Raw - Raw);
        *Buffer = Raw;
        //
        // update with the next structure handle.
        //
        if (Smbios.Raw < SmbiosEnd.Raw) {
        *Handle = Smbios.Hdr->Handle;
        } else {
        *Handle = INVALID_HANDLE;
        }
        return DMI_SUCCESS;
    }
    //
    // Walk to next structure
    //
    LibGetSmbiosString (&Smbios, (UINT16) (-1));
    }

    *Handle = INVALID_HANDLE;
    return DMI_INVALID_HANDLE;
}

VOID
PrintSmbiosTableEntryPointStruct (
    IN  SMBIOS_TABLE_ENTRY_POINT    *SmbiosTableEPS
)
{
    Print (L"-----SMBIOS Table Entry Point Structure-----\n");
    Print (L"Anchor String                     : %a\n", SmbiosTableEPS->AnchorString);
    Print (L"Entry Point Structure Checksum    : %02x\n", SmbiosTableEPS->EntryPointStructureChecksum);
    Print (L"Entry Point Length                : %d\n", SmbiosTableEPS->EntryPointLength);
    Print (L"Major Version                     : %d\n", SmbiosTableEPS->MajorVersion);
    Print (L"Minor Version                     : %d\n", SmbiosTableEPS->MinorVersion);
    Print (L"Max Structure Size                : %d\n", SmbiosTableEPS->MaxStructureSize);
    Print (L"Entry Point Revision              : %d\n", SmbiosTableEPS->EntryPointRevision);
    Print (L"Formatted Area                    : %x\n", SmbiosTableEPS->FormattedArea);
    Print (L"Intermediate Anchor String        : %a\n", SmbiosTableEPS->IntermediateAnchorString);
    Print (L"Intermediate Checksum             : %02x\n", SmbiosTableEPS->IntermediateChecksum);
    Print (L"Table Length                      : %d\n", SmbiosTableEPS->TableLength);
    Print (L"Table Address                     : %08x\n", SmbiosTableEPS->TableAddress);
    Print (L"Number Of SMBIOS Structures       : %d\n", SmbiosTableEPS->NumberOfSmbiosStructures);
    Print (L"SMBIOS BCD Revision               : %d\n", SmbiosTableEPS->SmbiosBcdRevision);
}

VOID
PrintTypeRegister(
    IN  UINT8   InputType
)
{
    UINT16                      StructHandle;
    UINT8                       *StructBuffer;
    UINT16                      StructLength;

    UINT8                       Type;
    UINT8                       TypeLength;
    UINT16                      TypeHandle;

    UINTN                       Index;
    UINTN                       Offset;

    BOOLEAN                     Found = FALSE;

    SMBIOS_STRUCTURE_POINTER    SmbiosStructPtr;


    StructHandle  = INVALID_HANDLE;
    LibGetSmbiosStructure (&StructHandle, NULL, NULL);

    for (Index = 0; Index < mSmbiosTable->NumberOfSmbiosStructures; Index ++) {
        if (StructHandle == INVALID_HANDLE) {
            break;
        }
        if (LibGetSmbiosStructure (&StructHandle, &StructBuffer, &StructLength) != DMI_SUCCESS) {
            break;
        }
        
        SmbiosStructPtr.Raw = StructBuffer;
        Type = SmbiosStructPtr.Hdr->Type;
        TypeLength = SmbiosStructPtr.Hdr->Length;
        TypeHandle = SmbiosStructPtr.Hdr->Handle;

        if (InputType == Type) {

            Found = TRUE;

            // Print the first line
            Print (L"---------------------------------------------------\n");
            Print (L"Type: %d\n", Type);
            Print (L"   ");
            for (Offset = 0x00; Offset <= 0x0F; Offset ++) {
                Print (L" %02x", Offset);
            }
            Print (L"\n");

            for (Offset = 0x00; Offset < TypeLength; Offset ++) {
                if (Offset % 0x10 == 0x00) {
                    Print (L" %02x", Offset);
                }
                Print (L" %02x", *(StructBuffer + Offset));
                if (Offset % 0x10 == 0x0F) {
                    Print (L"\n");
                }
                if (Offset == TypeLength - 1) {
                    Print (L"\n");
                }
            }
        }
    }

    if (Found == FALSE) {
        Print (L"Type not found.\n");
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
    if (Argc == 1) {
        Print (L"     [File Name] [Type]\n");
        Print (L"e.g.  SMBIOS.efi      7\n");
    } else if (Argc == 2) {
        PrintTypeRegister ((UINT8)StrDecimalToUintn(Argv[1]));
    } else if (Argc > 2) {
        Print (L"Please enter 1 value.\n");
    }
   
	return EFI_SUCCESS;
}