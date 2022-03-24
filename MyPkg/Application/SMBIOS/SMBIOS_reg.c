#include <Uefi.h>
#include <Base.h>
//StrHexToUintn function is in the BaseLib.
#include <Library/BaseLib.h>
#include <Library/UefiLib.h> //EfiGetSystemConfigurationTable ()

#include "SMBIOS_reg.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                  Status;

    Status = LibSmbiosInit ();
    Print (L"Anchor String: %a\n", mSmbiosTable->AnchorString);
    Print (L"Table Address: %08x\n", mSmbiosTable->TableAddress);
    PrintSmbiosTableRegister (mSmbiosTable->TableAddress);

    return Status;
}