#include <Uefi.h>
#include <Base.h>
//StrHexToUintn function is in the BaseLib.
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

//IO and PCI Library
#include <Library/IoLib.h>
#include <Library/PciLib.h>

//Shell Library
#include <Library/ShellLib.h>
#include <Library/UefiBootServicesTableLib.h>

//Protocol
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>

#define VENDOR_ID_OFFSET         0x00
#define PCI_CONFIG_ADDRESS_PORT  0xCF8
#define PCI_CONFIG_DATA_PORT     0xCFC
#define GET_PFA(BUS, DEVICE, FUNCTION, OFFSET) 0x80000000 | (BUS << 16) | (DEVICE << 11) | (FUNCTION << 8) | (OFFSET & 0xFC)

VOID
ReadPciRegister (
    IN INTN   Argc,
    IN CHAR16 **Argv
)
{
    unsigned int    i, j, k;
    UINT8           bus, dev, fun, reg8;
    UINT32          vid_did, reg32;

    if(Argc == 5 && StrCmp (Argv[1], L"r") == 0){

        bus = (UINT8)StrHexToUintn(Argv[2]);
        dev = (UINT8)StrHexToUintn(Argv[3]);
        fun = (UINT8)StrHexToUintn(Argv[4]);

        IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, VENDOR_ID_OFFSET));
        vid_did = IoRead32 (PCI_CONFIG_DATA_PORT);

        Print(L"VID and DID: %08x\n", vid_did);

        if(vid_did != 0xFFFFFFFF && vid_did != 0x00000000){

            //Print the 1st line.
            Print(L"   ");
            for(i = 0x00; i <= 0x0F; i += 0x01){
                Print(L"%02x ", i);
            }
            Print(L"\n");

            for(i = 0x00; i <= 0xF0; i += 0x10){
                Print(L"%02x ", i);
                for(j = 0x00; j <= 0x0C; j += 0x04){
                    IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, i + j));
                    reg32 = IoRead32 (PCI_CONFIG_DATA_PORT);
                    for(k = 0; k <= 24; k += 8){
                        reg8 = (UINT8)(BitFieldRead32 (reg32, k, k+7));
                        Print(L"%02x ", reg8);
                    }
                }
                Print(L"\n");
            }
        }
        else{
            Print(L"Device not found!\n");
        }
    }
    else{
        Print(L"Please enter the following parameters: \n1. r 2. bus 3. device 4. function\n");
    }
}

VOID
WritePciRegister (
    IN INTN   Argc,
    IN CHAR16 **Argv
)
{
    UINT8  bus, dev, fun, off, val, reg8;
    UINT32 vid_did, reg32;

    if(Argc == 7 && StrCmp (Argv[1], L"w") == 0){

        bus = (UINT8)StrHexToUintn(Argv[2]);
        dev = (UINT8)StrHexToUintn(Argv[3]);
        fun = (UINT8)StrHexToUintn(Argv[4]);
        off = (UINT8)StrHexToUintn(Argv[5]);
        val = (UINT8)StrHexToUintn(Argv[6]);

        IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, VENDOR_ID_OFFSET));
        vid_did = IoRead32 (PCI_CONFIG_DATA_PORT);

        Print(L"VID and DID: %08x\n", vid_did);

        if(vid_did != 0xFFFFFFFF && vid_did != 0x00000000){

            IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, off));
            IoWrite32 (PCI_CONFIG_DATA_PORT, val);
            IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, off));
            reg32 = IoRead32 (PCI_CONFIG_DATA_PORT);
            reg8 = (UINT8)(BitFieldRead32 (reg32, 0, 7));
            if(reg8 == val){
                Print(L"Successfully wrote %02x in offset: %02x.\n", val, off);
            }
        }
        else{
            Print(L"Device not found!\n");
        }
    }
    else{
        Print(L"Please enter the following parameters: \n1. r/w 2. bus 3. device 4. function 5. offset 6. value\n");
    }
}

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;

    UINTN                         Argc;
    CHAR16                        **Argv;

    EFI_STATUS                    Status;
    EFI_GUID                      mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

    gEfiShellParametersProtocol = NULL;

    if(gEfiShellParametersProtocol != NULL){
        Print (L"gEfiShellParametersProtocol is initialized.\n");
        Argc = gEfiShellParametersProtocol -> Argc;
        Argv = gEfiShellParametersProtocol -> Argv;
    }
    else{
        // check out input parameters from command line using UEFI Shell 2.0
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

    if(Argc == 5 && StrCmp (Argv[1], L"r") == 0){
        Print(L"ENTERED READ MODE.\n");
        ReadPciRegister (Argc, Argv);
    }
    else if(Argc == 7 && StrCmp (Argv[1], L"w") == 0){
        Print(L"ENTERED WRITE MODE.\n");
        WritePciRegister (Argc, Argv);
    }
    else{
        Print(L"Please enter the following parameters: \n1. r/w 2. bus 3. device 4. function 5. offset(OPTIONAL) 6. value(OPTIONAL)\n*Divide them by space.*\n");
    }
    return EFI_SUCCESS;
}