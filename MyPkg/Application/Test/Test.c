#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/ShellLib.h>

#include <Library/IoLib.h>

EFI_STATUS
EFIAPI
ShellAppMain (
    IN  UINTN   Argc,
	IN  CHAR16  **Argv
)
{
    UINT8   i;
    Print (L"Argv = %d\n", Argc);

	for (i = 0; i < Argc; i++)
 	{
        Print (L"Arg[%d] = %s\n", i, *(Argv + i));
	}
	return EFI_SUCCESS;
}