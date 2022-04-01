#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/IoLib.h>

EFI_STATUS
UefiMain (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
   UINT8    A;
   UINT8    B;
   UINT8    C;
   UINT8    D;

   A = 0x0F;
   B = 0xF0;
   C = A + B;
   D = A | B;
   return EFI_SUCCESS;
}