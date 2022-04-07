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

//#include <Protocol/SimpleFileSystem.h>
#include "ACPI.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                                      Status;
    UINT16                                          RsdpSignature[20];
    UINT16                                          RsdpOemId[20];
    UINT16                                          XsdtSignature[20];
    UINT64                                          *EntryPtr;
    EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER    *Rsdp;
    EFI_ACPI_DESCRIPTION_HEADER                     *Xsdt;
    //EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE       *Fadt;
    //EFI_ACPI_DESCRIPTION_HEADER                     *Dsdt;
    UINTN                                           Index;
    UINTN                                           EntryCount;

    Status = EfiGetSystemConfigurationTable (&gEfiAcpi20TableGuid, (VOID **)&Rsdp);

    if (Rsdp == NULL) {
        Print (L"Get ACPI RSDP error.\n");
        Status = EFI_NOT_FOUND;
    } else {
        Print (L"Found ACPI RSDP: \n");
        Print (L"     RSDP Address: [0x%x]\n", Rsdp);

        ZeroMem(RsdpSignature, sizeof(RsdpSignature));
        for (Index = 0; Index < 8; Index++) {
            RsdpSignature[Index] = (Rsdp->Signature >> (Index * 8) & 0xFF);
        }
        ZeroMem(RsdpOemId, sizeof(RsdpOemId));
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

            ZeroMem(XsdtSignature, sizeof(XsdtSignature));
            for (Index = 0; Index < 4; Index++) {
                XsdtSignature[Index] = (Xsdt->Signature >> (Index * 8) & 0xFF);
            }
            Print (L"   XSDT Signature: %s\n", XsdtSignature);
            Print (L"      XSDT Length: %d\n", Xsdt->Length);

            EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (UINT64);
            Print (L"   size of Header: %d\n", sizeof (EFI_ACPI_DESCRIPTION_HEADER));
            Print (L"   size of UINT64: %d\n", sizeof (UINT64));
            Print (L" XSDT Entry Count: %d\n", EntryCount);

            EntryPtr = (UINT64 *)(Xsdt + 1);
            /*
            for (Index = 0; Index < EntryCount; Index++, EntryPtr++) {
                
            }
            */
        }
    }

    return Status;
}