#include <Library/ECCommandLib.h>

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