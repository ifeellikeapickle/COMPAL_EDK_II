#include  <Library/ShellCEntryLib.h>
#include  <Protocol/EdidDiscovered.h>

#define REVERSE(Word) ((Word << 8) | (Word >> 8))

extern EFI_BOOT_SERVICES         *gBS;
extern EFI_SYSTEM_TABLE			 *gST;
extern EFI_RUNTIME_SERVICES 	 *gRT;

extern EFI_HANDLE				 gImageHandle;

EFI_GUID gEfiEdidDiscoveredProtocolGuid = { 0x1C0C34F6, 0xD380, 0x41FA, { 0xA0, 0x49, 0x8A, 0xD0, 0x6C, 0x1A, 0x66, 0xAA }};

typedef struct {
    UINT8  Header[8];                        //00h-07h //EDID header "00 FF FF FF FF FF FF 00"
    UINT16 ManufactureName;                  //08h-09h //EISA 3-character ID
    UINT16 ProductCode;                      //0Ah-0Bh //Vendor assigned code
    UINT32 SerialNumber;                     //0Ch-0Fh //32-bit serial number
    UINT8  WeekOfManufacture;                //10h-10h //Week number
    UINT8  YearOfManufacture;                //11h-11h //Year
    UINT8  EdidVersion;                      //12h-12h //EDID Structure Version
    UINT8  EdidRevision;                     //13h-13h //EDID Structure Revision
    UINT8  VideoInputDefinition;             //14h-14h
    UINT8  MaxHorizontalImageSize;           //15h-15h //cm
    UINT8  MaxVerticalImageSize;             //16h-16h //cm
    UINT8  DisplayGamma;                     //17h-17h
    UINT8  DpmSupport;                       //18h-18h
    UINT8  RedGreenLowBits;                  //19h-19h //Rx1 Rx0 Ry1 Ry0 Gx1 Gx0 Gy1Gy0
    UINT8  BlueWhiteLowBits;                 //1Ah-1Ah //Bx1 Bx0 By1 By0 Wx1 Wx0 Wy1 Wy0
    UINT8  RedX;                             //1Bh-1Bh //Red-x Bits 9 - 2
    UINT8  RedY;                             //1Ch-1Ch //Red-y Bits 9 - 2
    UINT8  GreenX;                           //1Dh-1Dh //Green-x Bits 9 - 2
    UINT8  GreenY;                           //1Eh-1Eh //Green-y Bits 9 - 2
    UINT8  BlueX;                            //1Fh-1Fh //Blue-x Bits 9 - 2
    UINT8  BlueY;                            //20h-20h //Blue-y Bits 9 - 2
    UINT8  WhiteX;                           //21h-21h //White-x Bits 9 - 2
    UINT8  WhiteY;                           //22h-22h //White-x Bits 9 - 2
    UINT8  EstablishedTimings[3];            //23h-25h
    UINT8  StandardTimingIdentification[16]; //26h-35h
    UINT8  DescriptionBlock1[18];            //36h-47h
    UINT8  DescriptionBlock2[18];            //48h-59h
    UINT8  DescriptionBlock3[18];            //5Ah-6Bh
    UINT8  DescriptionBlock4[18];            //6Ch-7Dh
    UINT8  ExtensionFlag;                    //Number of (optional) 128-byte EDID extension blocks
    UINT8  Checksum;
} EDID_DATA_BLOCK;

EFI_EDID_DISCOVERED_PROTOCOL *
GetEdidProtocol (VOID)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      *EDIDHandleBuffer;
    UINTN                           EDIDHandleCount;
    UINTN                           Index;
    EFI_EDID_DISCOVERED_PROTOCOL    *EdidDP = NULL;

    //Get protocol
    Status = gBS->LocateHandleBuffer (ByProtocol, 
                                      &gEfiEdidDiscoveredProtocolGuid,
                                      NULL,
                                      &EDIDHandleCount,
                                      &EDIDHandleBuffer);
    if (EFI_ERROR (Status)) {
        Print(L"ERROR: Can't get EDID Discovered Protocol.\n");
    } else {
        Print(L"EDID count = %d\n", EDIDHandleCount);
    }

    for(Index = 0; Index < EDIDHandleCount; Index ++) {

        //Open protocol
        Status = gBS->OpenProtocol (EDIDHandleBuffer[Index],
                                    &gEfiEdidDiscoveredProtocolGuid,
                                    (VOID **)&EdidDP,
                                    gImageHandle,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                    );
        if (EFI_ERROR (Status)) {
            Print(L"ERROR: Open EDID Discovered Protocol failed.\n");
        } else {
            Print(L"Size of EDID = %d\n", EdidDP->SizeOfEdid); //SizeOfEdid is UINT32 type
        }

        //Close protocol
        Status = gBS->CloseProtocol (EDIDHandleBuffer[Index],
                                    &gEfiEdidDiscoveredProtocolGuid,
                                    gImageHandle,
                                    NULL);
        if (EFI_ERROR (Status)) {
            Print(L"ERROR: Close EDID device handle failed.\n");
        }
    }

    return EdidDP;
}

VOID
PrintEdidReg (
    IN  EFI_EDID_DISCOVERED_PROTOCOL    *EdidDiscoveredProtocol
)
{
    UINTN   Index;
    //Print the first row
    Print(L"   ");
    for (Index = 0x00; Index <= 0x0F; Index ++){
        Print(L"%02x ", Index);
    }
    Print(L"\n");

    //Print registers
    for (Index = 0x00; Index <= 0x7F; Index ++) {
        if (Index % 0x10 == 0x00) {
            Print(L"%02x ", Index);
        }
        Print(L"%02x ", EdidDiscoveredProtocol->Edid[Index]);
        if (Index % 0x10 == 0x0F) {
            Print(L"\n");
        }
    }
}


CHAR16 *
ManufacturerAbbrev(
    UINT16 ManufactureCode
)
{
    STATIC  CHAR16  MName[4];

    MName[0] = (CHAR16)((ManufactureCode >> 10) & 0x1F) + 64;
    MName[1] = (CHAR16)((ManufactureCode >> 5) & 0x1F) + 64;
    MName[2] = (CHAR16)(ManufactureCode & 0x1F) + 64;
    MName[3] = (CHAR16)'\0';
 
    return MName;
}

VOID
PrintEdidInfo (
    IN  EFI_EDID_DISCOVERED_PROTOCOL    *EdidDiscoveredProtocol
)
{
    EDID_DATA_BLOCK     *EdidDataBlock = (EDID_DATA_BLOCK *)(EdidDiscoveredProtocol->Edid);
    UINT16              mcode = EdidDataBlock->ManufactureName;
    UINT8               video_input = EdidDataBlock->VideoInputDefinition;
    UINT8               feature_support = EdidDataBlock->DpmSupport;
    CHAR16              *arl;
    CHAR16              *arp;
    //UINT8               tmp;

    mcode = REVERSE (mcode);

    Print(L"                                   Version: %d\n", EdidDataBlock->EdidVersion);
    Print(L"                                  Revision: %d\n", EdidDataBlock->EdidRevision);
    Print(L"                         Manufacturer Name: %s\n", ManufacturerAbbrev (mcode));
    Print(L"                              Product Code: %04X\n", EdidDataBlock->ProductCode);
    Print(L"                             Serial Number: %08X\n", EdidDataBlock->SerialNumber);
    Print(L"                       Week of Manufacture: %d\n", EdidDataBlock->WeekOfManufacture);
    Print(L"                       Year of Manufacture: %d\n", EdidDataBlock->YearOfManufacture + 1990);

    Print(L"                    Video Signal Interface: ");
    if (video_input >> 7 == 0x00) {
        Print(L"Analog\n");
    } else if (video_input >> 7 == 0x01) {
        Print(L"Digital\n");
        Print(L"                           Color Bit Depth: ");
        if ((video_input & 0xF0) == 0x80) {
            Print(L"Color Bit Depth is undefined\n");
        } else if ((video_input & 0xF0) == 0x90) {
            Print(L"6 Bits per Primary Color\n");
        } else if ((video_input & 0xF0) == 0xA0) {
            Print(L"8 Bits per Primary Color\n");
        } else if ((video_input & 0xF0) == 0xB0) {
            Print(L"10 Bits per Primary Color\n");
        } else if ((video_input & 0xF0) == 0xC0) {
            Print(L"12 Bits per Primary Color\n");
        } else if ((video_input & 0xF0) == 0xD0) {
            Print(L"14 Bits per Primary Color\n");
        } else if ((video_input & 0xF0) == 0xE0) {
            Print(L"16 Bits per Primary Color\n");
        } else {
            Print(L"ERROR!\n");
        }
        Print(L"Digital Video Interface Standard Supported: ");
        if ((video_input & 0x8F) == 0x80) {
            Print(L"Digital Interface is not defined\n");
        } else if ((video_input & 0x8F) == 0x81) {
            Print(L"DVI is supported\n");
        } else if ((video_input & 0x8F) == 0x82) {
            Print(L"HDMI-a is supported\n");
        } else if ((video_input & 0x8F) == 0x83) {
            Print(L"HDMI-b is supported\n");
        } else if ((video_input & 0x8F) == 0x84) {
            Print(L"MDDI is supported\n");
        } else if ((video_input & 0x8F) == 0x85) {
            Print(L"DisplayPort is supported\n");
        } else {
            Print(L"ERROR!\n");
        }
    }

    if ((EdidDataBlock->MaxHorizontalImageSize != 0x00) && (EdidDataBlock->MaxVerticalImageSize != 0x00)) {
        Print(L"                    Horizontal Screen Size: %d cm\n", EdidDataBlock->MaxHorizontalImageSize);
        Print(L"                      Vertical Screen Size: %d cm\n", EdidDataBlock->MaxVerticalImageSize);
    } else if (EdidDataBlock->MaxHorizontalImageSize == 0x00) {
        switch (EdidDataBlock->MaxVerticalImageSize) {
            case 0x4F:
                arp = L"9:16";
                break;
            case 0x3D:
                arp = L"10:16";
                break;
            case 0x22:
                arp = L"3:4";
                break;
            case 0x1A:
                arp = L"4:5";
                break;
            default:
                arp = L"Cannot identify!";
                break;
        }
        Print(L"                   Aspect Ratio (Portrait): %s\n", *arp);
    } else if (EdidDataBlock->MaxVerticalImageSize == 0x00) {
        switch (EdidDataBlock->MaxHorizontalImageSize) {
            case 0x4F:
                arl = L"16:9";
                break;
            case 0x3D:
                arl = L"16:10";
                break;
            case 0x22:
                arl = L"4:3";
                break;
            case 0x1A:
                arl = L"5:4";
                break;
            default:
                arl = L"Cannot identify!";
                break;
        }
        Print(L"                  Aspect Ratio (Landscape): %s\n", *arl);
    }

    if (EdidDataBlock->DisplayGamma != 0xFF) {
        Print(L"                                     GAMMA: %d\n", (EdidDataBlock->DisplayGamma + 100) / 100);
    }

    Print(L"                  Display Power Management: ");
    if (feature_support & 0x80) {
        Print(L"Standby Mode is supported.\n");
    } else if (!(feature_support & 0x80)) {
        Print(L"Standby Mode is not supported.\n");
    }
    if (feature_support & 0x40) {
        Print(L"                                            ");
        Print(L"Suspend Mode is supported.\n");
    } else if (!(feature_support & 0x40)) {
        Print(L"                                            ");
        Print(L"Suspend Mode is not supported.\n");
    }
    if (feature_support & 0x20) {
        Print(L"                                            ");
        Print(L"Active Off = Very Low Power is supported.\n");
    } else if (!(feature_support & 0x20)) {
        Print(L"                                            ");
        Print(L"Active Off = Very Low Power is not supported.\n");
    }

    Print(L"         Supported Color Encoding Format/s: ");
    if ((feature_support & 0x18) == 0x00) {
        Print(L"RGB 4:4:4\n");
    } else if ((feature_support & 0x18) == 0x08) {
        Print(L"RGB 4:4:4 & YCrCb 4:4:4\n");
    } else if ((feature_support & 0x18) == 0x10) {
        Print(L"RGB 4:4:4 & YCrCb 4:2:2\n");
    } else if ((feature_support & 0x18) == 0x18) {
        Print(L"RGB 4:4:4 & YCrCb 4:4:4 & YCrCb 4:2:2\n");
    }

    Print(L"                     Other Feature Support: ");
    if (feature_support & 0x04) {
        Print(L"sRGB Standard is the default color space.\n");
    } else if (!(feature_support & 0x04)) {
        Print(L"sRGB Standard is not the default color space.\n");
    }
    if (feature_support & 0x02) {
        Print(L"                                            ");
        Print(L"Preferred Timing Mode includes the native pixel format and preferred refresh rate of the display device.\n");
    } else if (!(feature_support & 0x02)) {
        Print(L"                                            ");
        Print(L"Preferred Timing Mode does not include the native pixel format and preferred refresh rate of the display device.\n");
    }
    if (feature_support & 0x01) {
        Print(L"                                            ");
        Print(L"Display is continuous frequency.\n");
    } else if (!(feature_support & 0x01)) {
        Print(L"                                            ");
        Print(L"Display is non-continuous frequency.\n");
    }

    //PrintDetailedTimingBlock((UINT8 *)&(EdidDataBlock->DescriptionBlock1[0]));

    /*
    Print(L"           Video Setup: ");
    if ((video_input >> 4) && 0x01 == 0x00) {
        Print(L"Blank Level = Blank Level\n");
    } else if ((video_input >> 4) && 0x01 == 0x01) {
        Print(L"Blank-to-Black setup or pedestal\n");
    }

    Print(L" Synchronization Types: ");
    if ((video_input >> 3) && 0x01 == 0x00) {
        Print(L"Separate Sync H & V Signals are not supported\n");
    } else if ((video_input >> 3) && 0x01 == 0x01) {
        Print(L"Separate Sync H & V Signals are supported\n");
    }
    if ((video_input >> 2) && 0x01 == 0x00) {
        Print(L"                        ");
        Print(L"Composite Sync Signal on Horizontal is not supported\n");
    } else if ((video_input >> 2) && 0x01 == 0x01) {
        Print(L"                        ");
        Print(L"Composite Sync Signal on Horizontal is supported\n");
    }
    if ((video_input >> 1) && 0x01 == 0x00) {
        Print(L"                        ");
        Print(L"Composite Sync Signal on Green Video is not supported\n");
    } else if ((video_input >> 1) && 0x01 == 0x01) {
        Print(L"                        ");
        Print(L"Composite Sync Signal on Green Video is supported\n");
    }

    Print(L"            Serrations: ");
    if (video_input && 0x01 == 0x00) {
        Print(L"Serration on the Vertical Sync is not supported\n");
    } else if (video_input && 0x01 == 0x01) {
        Print(L"Serration on the Vertical Sync is supported\n");
    }
    */
}