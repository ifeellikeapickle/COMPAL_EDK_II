#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h> //StrHexToUintn() function
#include <Library/UefiLib.h> //Print() function

#include "ECCMD.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS              Status;
     
    Status = GetInputParameters (ImageHandle, SystemTable);

    return Status;
}