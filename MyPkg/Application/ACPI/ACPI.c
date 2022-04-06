#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/ShellCEntryLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h> //gST, gBS

#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi60.h>

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
    UINT16                                          XsdtSignature[20];
    EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER    *RSDP;
    EFI_ACPI_DESCRIPTION_HEADER                     *XSDT;
    //EFI_ACPI_DESCRIPTION_HEADER                     *DSDT;
    UINTN                                           Index;

    Status = EfiGetSystemConfigurationTable (&gEfiAcpi20TableGuid, (VOID **)&RSDP);

    if (RSDP == NULL) {
        Print (L"Get ACPI RSDP error.\n");
        Status = EFI_NOT_FOUND;
    } else {
        Print (L"Found ACPI RSDP: \n");

        Print (L"ACPI RSDP Address: [0x%x]\n", RSDP);

        for (Index = 0; Index < 8; Index++) {
            RsdpSignature[Index] = (UINT16)(RSDP->Signature >> (Index * 8) & 0xFF);
        }

        Print (L"   RSDP Signature: %s\n", RsdpSignature);
        Print (L"         Revision: %d\n", RSDP->Revision);
        Print (L"     RSDT Address: [0x%x], Length: %d\n", RSDP->RsdtAddress, RSDP->Length);
		Print (L"     XSDT Address: [0x%lx]\n", RSDP->XsdtAddress);

        if (RSDP->Revision >= EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {

            XSDT = (EFI_ACPI_DESCRIPTION_HEADER *)(RSDP->XsdtAddress);

            for (Index = 0; Index < 4; Index++) {
                XsdtSignature[Index] = (UINT16)(XSDT->Signature >> (Index * 8) & 0xFF);
            }
            Print (L"   XSDT Signature: %s\n", XsdtSignature);
        }
    }

    return Status;
}