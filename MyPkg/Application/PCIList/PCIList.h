#define PCI_MAX_BUS                 0xFF
#define PCI_MAX_DEVICE              0x1F
#define PCI_MAX_FUNCTION            0x07
#define VENDOR_ID_OFFSET            0x00
#define DEVICE_ID_OFFSET            0x02
#define CLASS_CODE_OFFSET           0x09
#define SUBSYSTEM_VENDOR_ID_OFFSET  0x2C
#define SUBSYSTEM_DEVICE_ID_OFFSET  0x2E
#define CAPABILITIES_POINTER_OFFSET 0x34
#define CONFIG_ADDRESS              0xCF8
#define CONFIG_DATA                 0xCFC
#define PCI_BASE_ADDR               0x80000000L

/*
#define PCI_LIB_ADDRESS(Bus,Device,Function,Register)   \
  (((Register) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))
*/

VOID
PrintPciClassCode (
    UINT8 BaseClassCode
)
{
    //char result = "";
    switch(BaseClassCode){
        case 0x00:
            Print(L"Pre 2.0 device\n");
            break;
        case 0x01:
            Print(L"Mass Storage Controller\n");
            break;
        case 0x02:
            Print(L"Network Controller\n");
            break;
        case 0x03:
            Print(L"Display Controller\n");
            break;
        case 0x04:
            Print(L"Multimedia Device\n");
            break;
        case 0x05:
            Print(L"Memory Controller\n");
            break;
        case 0x06:
            Print(L"Bridge Device\n");
            break;
        case 0x07:
            Print(L"Simple Communications Controllers\n");
            break;
        case 0x08:
            Print(L"Base System Peripherals\n");
            break;
        case 0x09:
            Print(L"Input Devices\n");
            break;
        case 0x0A:
            Print(L"Docking Stations\n");
            break;
        case 0x0B:
            Print(L"Processors\n");
            break;
        case 0x0C:
            Print(L"Serial Bus Controllers\n");
            break;
        case 0x0D:
            Print(L"Wireless Controllers\n");
            break;
        case 0x0E:
            Print(L"Intelligent IO Controllers\n");
            break;
        case 0x0F:
            Print(L"Satellite Communications Controllers\n");
            break;
        case 0x10:
            Print(L"Encryption/Decryption Controllers\n");
            break;
        case 0x11:
            Print(L"Data Acquisition & Signal Processing Controllers\n");
            break;
        case 0x12:
            Print(L"Processing Accelerators\n");
            break;
        case 0x13:
            Print(L"Non-Essential Instrumentation\n");
            break;
        case 0xFF:
            Print(L"Device does not fit in any defined classes\n");
            break;
        default:
            Print(L"\n");
            break;
    }
}