#include <Uefi.h>
#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ShellLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

#include <Library/ECCommandLib.h>

EFI_STATUS
UefiMain(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;

    UINTN			Argc;
	CHAR16			**Argv;
    EFI_STATUS      Status;

    EFI_GUID  mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

	gEfiShellParametersProtocol = NULL;

	if (gEfiShellParametersProtocol != NULL) {
		Print (L"gEfiShellParametersProtocol is initialized\n");
        Argc = gEfiShellParametersProtocol->Argc;
        Argv = gEfiShellParametersProtocol->Argv;

	} else {
        // check out input parameters from command line using UEFI Shell 2.0
		Status = gBS->OpenProtocol(
            ImageHandle,
            &mEfiShellParametersProtocolGuid,
            (VOID **)&gEfiShellParametersProtocol,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        ); 
		if (EFI_ERROR(Status)) {
		      Print(L"\nSorry, Getting Shell parameters did NOT work or in the EFI Shell 1.0: \n");
			  return EFI_UNSUPPORTED;
		} else {
		    Argc = gEfiShellParametersProtocol->Argc;
			Argv = gEfiShellParametersProtocol->Argv;
		}
	}
    //
    // use shell 2.0 interface
    //

    EcCommand (Argc, Argv);

    return EFI_SUCCESS;
}