#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/ShellCEntryLib.h>
#include <Library/BaseMemoryLib.h>

// gST, gBS
#include <Library/UefiBootServicesTableLib.h>

// ACPI
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>

#include <Protocol/SimpleFileSystem.h>

#include "ACPI.h"

EFI_STATUS
EFIAPI
SearchOperationRegion (
    IN  EFI_ACPI_DESCRIPTION_HEADER     *Table
)
{
    EFI_STATUS          Status;
    CHAR16              RegionName[20];
    OPERATION_REGION    *OperationRegion;
    

    for (OperationRegion = (OPERATION_REGION *) Table;
        OperationRegion  < (OPERATION_REGION *) ((UINT8 *)Table + Table->Length);
        OperationRegion  = (OPERATION_REGION *) ((UINT8 *)OperationRegion + 1)) {
            if (OperationRegion->OpRegionOp == ENCODING_OPERATION_REGION &&
                OperationRegion->RegionSpace == REGION_SPACE_SYSTEM_MEMORY) {
                    Print (L" %08x", OperationRegion->NameString);
                    ZeroMem (RegionName, sizeof(RegionName));
                    RegionName[0] = (OperationRegion->NameString >> 0 & 0xFF);
                    RegionName[1] = (OperationRegion->NameString >> 8 & 0xFF);
                    RegionName[2] = (OperationRegion->NameString >> 16 & 0xFF);
                    RegionName[3] = (OperationRegion->NameString >> 24 & 0xFF);
                    Print (L" (%s)\n", RegionName);
            }
    }
    Status = EFI_SUCCESS;
    return Status;
}

EFI_STATUS
EFIAPI
PrintGuid (
    IN  EFI_GUID    *Guid
)
{
    EFI_STATUS  Status;

    if (Guid == NULL) {
        Print(L"GUID is NULL!\n");
        Status = EFI_INVALID_PARAMETER;
    } else {
        Print (
        L"GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
        (UINTN)Guid->Data1,
        (UINTN)Guid->Data2,
        (UINTN)Guid->Data3,
        (UINTN)Guid->Data4[0],
        (UINTN)Guid->Data4[1],
        (UINTN)Guid->Data4[2],
        (UINTN)Guid->Data4[3],
        (UINTN)Guid->Data4[4],
        (UINTN)Guid->Data4[5],
        (UINTN)Guid->Data4[6],
        (UINTN)Guid->Data4[7]
        );
        Status = EFI_SUCCESS;
    }

    return Status;
}

EFI_STATUS
EFIAPI
SaveDsdtToFile (
	CHAR8	*Pointer,
	UINTN	Length
)
{
    EFI_STATUS          			Status;
	EFI_FILE_PROTOCOL 				*Root;
    EFI_FILE_PROTOCOL               *FileHandle = 0;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
	
	Status = gBS->LocateProtocol(
					&gEfiSimpleFileSystemProtocolGuid,
					NULL,
					(VOID **)&SimpleFileSystem);

	if (EFI_ERROR(Status)) {
			Print (L"Cannot find EFI_SIMPLE_FILE_SYSTEM_PROTOCOL\n");
			return Status;
	}

	Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
	if (EFI_ERROR(Status)) {
			Print (L"OpenVolume error\n");
			return Status;
	}
   
	Status = Root -> Open(Root,
				&FileHandle,
				(CHAR16 *) L"dsdt.aml",
				EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
				0);
			
	if (EFI_ERROR(Status) || (FileHandle == 0)) {
			Print(L"Open error\n");
			return Status;
	}
	Status = FileHandle -> Write (FileHandle, &Length, Pointer);
	
	Status  = FileHandle -> Close (FileHandle);
	
	return EFI_SUCCESS;
}

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                                      Status;
    UINTN                                           Index;
    UINTN                                           EntryCount;
    //UINTN                                           Count = 0;
    CHAR16                                          RsdpSignature[20];
    CHAR16                                          RsdpOemId[20];
    CHAR16                                          XsdtSignature[20];
    CHAR16                                          FadtSignature[20];
    CHAR16                                          DsdtSignature[20];
    //UINT8                                           *Pointer;
    UINT64                                          *EntryPtr;
    EFI_ACPI_DESCRIPTION_HEADER                     *Xsdt;
    EFI_ACPI_DESCRIPTION_HEADER                     *Dsdt;
    EFI_ACPI_DESCRIPTION_HEADER                     *Entry;
    EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE       *Fadt;
    EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER    *Rsdp;
    
    Status = EfiGetSystemConfigurationTable (&gEfiAcpi20TableGuid, (VOID **)&Rsdp);

    if (Rsdp == NULL) {
        Print (L"RSDP not found!\n");
        Status = EFI_NOT_FOUND;
    } else {
        Print (L"     RSDP Address: [0x%x]\n", Rsdp);

        ZeroMem (RsdpSignature, sizeof(RsdpSignature));
        for (Index = 0; Index < 8; Index++) {
            RsdpSignature[Index] = (Rsdp->Signature >> (Index * 8) & 0xFF);
        }
        ZeroMem (RsdpOemId, sizeof(RsdpOemId));
        for (Index = 0; Index < 6; Index++) {
            RsdpOemId[Index] = (Rsdp->OemId[Index] & 0xFF);
        }

        Print (L"   RSDP Signature: %s\n", RsdpSignature);
        Print (L"      RSDP OEM ID: %s\n", RsdpOemId);
        Print (L"    RSDP Revision: %d\n", Rsdp->Revision);
        Print (L"     RSDT Address: [0x%x], Length: %d\n", Rsdp->RsdtAddress, Rsdp->Length);
		Print (L"     XSDT Address: [0x%lx]\n", Rsdp->XsdtAddress);

        if (Rsdp->Revision >= EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {

            Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(Rsdp->XsdtAddress);

            ZeroMem (XsdtSignature, sizeof(XsdtSignature));
            for (Index = 0; Index < 4; Index++) {
                XsdtSignature[Index] = (Xsdt->Signature >> (Index * 8) & 0xFF);
            }
            Print (L"   XSDT Signature: %s\n", XsdtSignature);
            Print (L"      XSDT Length: %d\n", Xsdt->Length);

            EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (UINT64);
            Print (L" XSDT Entry Count: %d\n", EntryCount);

            EntryPtr = (UINT64 *)(Xsdt + 1); // Skip the header?
            
            for (Index = 0; Index < EntryCount; Index++, EntryPtr++) {
                Entry = (EFI_ACPI_DESCRIPTION_HEADER *)(*EntryPtr);
                //Print (L"  Entry Signature: 0x%x\n", Entry->Signature);

                if (Entry->Signature == EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
                    Fadt = (EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE *)Entry;
                    Dsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)(Fadt->Dsdt);

                    ZeroMem (FadtSignature, sizeof(FadtSignature));
                    for (Index = 0; Index < 4; Index++) {
                        FadtSignature[Index] = (Fadt->Header.Signature >> (Index * 8) & 0xFF);
                    }
                    ZeroMem (DsdtSignature, sizeof(DsdtSignature));
                    for (Index = 0; Index < 4; Index++) {
                        DsdtSignature[Index] = (Dsdt->Signature >> (Index * 8) & 0xFF);
                    }

                    Print (L"   FADT Signature: %s\n", FadtSignature);
                    Print (L"      FADT Length: %d\n", Fadt->Header.Length);
                    Print (L"    FADT Revision: %d\n", Fadt->Header.Revision);
                    Print (L"     DSDT Address: 0x%x\n", Dsdt);
                    Print (L"   DSDT Signature: %s\n", DsdtSignature);
                    Print (L"      DSDT Length: %d\n", Dsdt->Length);

                    SearchOperationRegion (Dsdt);
                }
            }
        }
    }
    return Status;
}