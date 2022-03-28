#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h> // StrHexToUintn() function
#include <Library/UefiLib.h> // Print() function

#include <Library/IoLib.h>

// Related to input function
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ShellLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

#include <Library/TimerLib.h> // MicroSecondDelay() function

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