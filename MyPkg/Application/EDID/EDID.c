#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include "EDID.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_EDID_DISCOVERED_PROTOCOL    *edid_dp;

    edid_dp = GetEdidProtocol ();
    PrintEdidReg (edid_dp);
    PrintEdidInfo (edid_dp);

    return Status;
}