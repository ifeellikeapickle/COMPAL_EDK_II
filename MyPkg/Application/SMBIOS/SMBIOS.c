#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h> // StrDecimalToUintn() function
#include <Library/UefiLib.h>

#include "SMBIOS.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                  Status;

    // Initial the SMBIOS Table
    Status = LibSmbiosInit ();
    mSmbiosStruct->Raw  = (UINT8 *) (UINTN) (mSmbiosTable->TableAddress);

    Status = GetInputParameters (ImageHandle, SystemTable);
    
    return Status;
}