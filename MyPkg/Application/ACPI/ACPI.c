#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/ShellCEntryLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h> // gST, gBS

// ACPI
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>

//#include <Protocol/SimpleFileSystem.h>
#include "ACPI.h"

VOID
EFIAPI
GetOpRegion (
    EFI_ACPI_DESCRIPTION_HEADER     *Table
)
{
    AML_OP_REGION_32_8              *OpRegion;

    for (OpRegion  = (AML_OP_REGION_32_8 *) (Table + 1);
        OpRegion <= (AML_OP_REGION_32_8 *) ((UINT8 *) Table + Table->Length);
        OpRegion  = (AML_OP_REGION_32_8 *) ((UINT8 *) OpRegion + 1)) {

        if (OpRegion->NameString == 0x564E4153) {
            Print (L"SANV found\n");
            Print (L" Region Space: %02x\n", OpRegion->RegionSpace);
            Print (L" DWord Prefix: %02x\n", OpRegion->DWordPrefix);
            Print (L"  Byte Prefix: %02x\n", OpRegion->BytePrefix);
            Print (L"Region Offset: %08x\n", OpRegion->RegionOffset);
            Print (L"Region Length: %d\n", OpRegion->RegionLen);
        } else if (OpRegion->NameString == 0x53564E47) {
            Print (L"GNVS found\n");
            Print (L" Region Space: %02x\n", OpRegion->RegionSpace);
            Print (L" DWord Prefix: %02x\n", OpRegion->DWordPrefix);
            Print (L"  Byte Prefix: %02x\n", OpRegion->BytePrefix);
            Print (L"Region Offset: %08x\n", OpRegion->RegionOffset);
            Print (L"Region Length: %d\n", OpRegion->RegionLen);
        }
    }
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
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                                      Status;
    UINTN                                           Index;
    UINTN                                           EntryCount;
    //UINT8                                           Encoding1;
    //UINT8                                           Encoding2;
    UINT16                                          RsdpSignature[20];
    UINT16                                          RsdpOemId[20];
    UINT16                                          XsdtSignature[20];
    UINT16                                          FadtSignature[20];
    UINT16                                          DsdtSignature[20];
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

                    GetOpRegion (Dsdt);
                    /*
                    for (Index = 0x00; Index < Fadt->Header.Length; Index++, Fadt++) {
                        Register = (UINT64)Fadt & 0xFF;
                        Print (L" %02x", Register);
                        if (Index % 0x10 == 0x0F) {
                            Print (L"\n");
                        }
                    }
                    */
                }
            }
        }
    }
    return Status;
}