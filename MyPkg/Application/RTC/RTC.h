#include <Library/IoLib.h>

//gBS
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/EfiShellParameters.h>
#include <Library/ShellLib.h>

#define RTC_BANK0_ADDRESS_PORT  0x70
#define RTC_BANK0_DATA_PORT     0x71
#define RTC_BANK1_ADDRESS_PORT  0x72
#define RTC_BANK1_DATA_PORT     0x73

#define RTC_SECOND_OFFSET       0x00
#define RTC_MINUTE_OFFSET       0x02
#define RTC_HOUR_OFFSET         0x04
#define RTC_DAY_OFFSET          0x06
#define RTC_DATE_OFFSET         0x07
#define RTC_MONTH_OFFSET        0x08
#define RTC_YEAR_OFFSET         0x09

VOID
PrintRTCRegister (
    IN  UINT8   Bank
)
{
    unsigned int    Index;
    UINT8           Register;
    UINT8           RtcAddressPort = 0x00;
    UINT8           RtcDataPort = 0x00;
    UINT8           Second;
    UINT8           Minute;
    UINT8           Hour;
    UINT8           DayReg;
    CHAR8           *Day;
    UINT8           Date;
    UINT8           Month;
    UINT16          Year;

    if (Bank == 0) {
        RtcAddressPort = RTC_BANK0_ADDRESS_PORT;
        RtcDataPort = RTC_BANK0_DATA_PORT;
    }
    else if (Bank == 1) {
        RtcAddressPort = RTC_BANK1_ADDRESS_PORT;
        RtcDataPort = RTC_BANK1_DATA_PORT;
    }
    else {
        Print (L"Bank Number is incorrect!\n");
        Print (L"Please enter Bank Number (0 or 1) followed by RTC.efi!\n");
    }

    if (RtcAddressPort == 0x70 || RtcAddressPort == 0x72) {

        //Print the first row.
        Print(L"   ");
        for(Index = 0x00; Index <= 0x0F; Index ++){
            Print(L"%02x ", Index);
        }
        Print(L"\n");

        for (Index = 0x00; Index <= 0xFF; Index ++) {

            //Print the first column.
            if (Index % 0x10 == 0x00) {
                Print (L"%02x ", Index);
            }

            IoWrite8 (RtcAddressPort, (UINT8)Index);
            Register = IoRead8 (RtcDataPort);
            Print (L"%02x ", Register);

            //Next line when this line is full.
            if (Index % 0x10 == 0x0F) {
                Print (L"\n");
            }
        }

        //Read the time register.
        IoWrite8 (RtcAddressPort, RTC_SECOND_OFFSET);
        Second = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_MINUTE_OFFSET);
        Minute = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_HOUR_OFFSET);
        Hour = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_DAY_OFFSET);
        DayReg = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_DATE_OFFSET);
        Date = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_MONTH_OFFSET);
        Month = IoRead8 (RtcDataPort);
        IoWrite8 (RtcAddressPort, RTC_YEAR_OFFSET);
        Year = IoRead8 (RtcDataPort);

        switch (DayReg) {
            case 0x01:
                Day = (CHAR8 *)L"MON";
                break;
            case 0x02:
                Day = (CHAR8 *)L"TUE";
                break;
            case 0x03:
                Day = (CHAR8 *)L"WED";
                break;
            case 0x04:
                Day = (CHAR8 *)L"THU";
                break;
            case 0x05:
                Day = (CHAR8 *)L"FRI";
                break;
            case 0x06:
                Day = (CHAR8 *)L"SAT";
                break;
            case 0x07:
                Day = (CHAR8 *)L"SUN";
                break;
            default:
                Day = (CHAR8 *)L"";
                break;
        }

        Print (L"\nCurrent Time: 20%02x/%02x/%02x, %s, %02x:%02x:%02x\n", Year, Month, Date, Day, Hour, Minute, Second);
    }
}

EFI_STATUS
GetInputParameters (
    IN  EFI_HANDLE        ImageHandle,
    IN  EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_SHELL_PARAMETERS_PROTOCOL *gEfiShellParametersProtocol;

    UINTN			Argc;
	CHAR16			**Argv;
    EFI_STATUS      Status;
    EFI_GUID  mEfiShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

    UINT8           Bank;

	gEfiShellParametersProtocol = NULL;

	if (gEfiShellParametersProtocol != NULL){
		Print (L"gEfiShellParametersProtocol is initialized\n");
        Argc = gEfiShellParametersProtocol->Argc;
        Argv = gEfiShellParametersProtocol->Argv;

	}else {
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
		      Print(L"\nSorry, Getting Shell params did NOT work or in the EFI Shell 1.0: \n");
			  return EFI_UNSUPPORTED;
		}else{
		    Argc = gEfiShellParametersProtocol->Argc;
			Argv = gEfiShellParametersProtocol->Argv;
		}
	}
    //
    // use shell 2.0 interface
    //

   if (Argc == 2) {
       Bank = (UINT8)StrHexToUintn(Argv[1]);
       PrintRTCRegister (Bank);
   }
   else {
       Print (L"Please enter Bank Number (0 or 1) followed by RTC.efi!\n");
   }
   
	return EFI_SUCCESS;
}