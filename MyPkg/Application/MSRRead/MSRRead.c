#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

//Shell Library
#include <Library/ShellLib.h>
#include <Library/UefiBootServicesTableLib.h>

//Protocol
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

#define MSR_INDEX 0x1A2

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    UINT64 msr;
    UINT32 index, eax, edx;

    //
    //EFI shell parameters protocol starts from here.
    //
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;

    UINTN  Argc;
    CHAR16 **Argv;

    EFI_STATUS                    Status;
    EFI_GUID                      mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

    gEfiShellParametersProtocol = NULL;

    if(gEfiShellParametersProtocol != NULL){
        Print (L"gEfiShellParametersProtocol is initialized.\n");
        Argc = gEfiShellParametersProtocol -> Argc;
        Argv = gEfiShellParametersProtocol -> Argv;
    }
    else{
        //Check out input parameters from command line using UEFI Shell 2.0
		Status = gBS -> OpenProtocol (
            ImageHandle,
            &mEfiShellParametersProtocolGuid,
            (VOID **)&gEfiShellParametersProtocol,
            ImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        ); 
		if(EFI_ERROR(Status)){
            Print(L"\nSorry, getting shell parameters did NOT work or in the EFI Shell 1.0.\n");
	 		return EFI_UNSUPPORTED;
		}
        else{
	    Argc = gEfiShellParametersProtocol -> Argc;
		Argv = gEfiShellParametersProtocol -> Argv;
		}
	}
    //
    //EFI shell parameters protocol ends here.
    //

    if (Argc == 2) {
        index = (UINT32)StrHexToUintn (Argv[1]);
        msr = AsmReadMsr64 (index);
        eax = (UINT32)BitFieldRead64 (msr, 0, 31);
        edx = (UINT32)BitFieldRead64 (msr, 32, 63);
        Print(L"EDX: %08x\nEAX: %08x\n", edx, eax);
    }else {
        Print(L"Please enter the MSR index!\n");
    }

    return EFI_SUCCESS;
}