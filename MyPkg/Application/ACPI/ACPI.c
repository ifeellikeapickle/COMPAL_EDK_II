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
    EFI_STATUS                  Status;
    EFI_CONFIGURATION_TABLE		*AcpiTable = NULL;
    //UINTN                       Index;

    Status = EfiGetSystemConfigurationTable (&gEfiAcpi20TableGuid, (VOID **)&AcpiTable);

    if (AcpiTable == NULL) {
        Print (L"Get ACPI Table error.\n");
        Status = EFI_NOT_FOUND;
    } else {
        Print (L"Found ACPI table: \n");
		Print (L"[0x%x]\n", AcpiTable);
        Status = PrintGuid (&(AcpiTable->VendorGuid));
    }

    return Status;
}