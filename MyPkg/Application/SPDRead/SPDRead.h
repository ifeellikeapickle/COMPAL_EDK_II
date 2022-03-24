#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define PCI_MAX_BUS                 0xFF
#define PCI_MAX_DEVICE              0x1F
#define PCI_MAX_FUNCTION            0x07
#define PCI_CONFIG_ADDRESS_PORT     0xCF8
#define PCI_CONFIG_DATA_PORT        0xCFC
#define VENDOR_ID_OFFSET            0x00
#define RIVISION_ID_OFFSET          0x08
#define SMBUS_BASE_ADDRESS_OFFSET   0x20

#define HSTS_OFFSET                 0x00
#define HCTL_OFFSET                 0x02
#define HCMD_OFFSET                 0x03
#define TSA_OFFSET                  0x04
#define HD0_OFFSET                  0x05

#define BIT00                       0x01
#define BIT02                       0x04

#define GET_PFA(BUS, DEVICE, FUNCTION, OFFSET) 0x80000000 | (BUS << 16) | (DEVICE << 11) | (FUNCTION << 8) | (OFFSET & 0xFC)

UINT32
EFIAPI
GetSMBusBaseAddress (VOID)
{
    unsigned int    bus, dev, fun;
    UINT32          vid_did;
    UINT32          class_code;
    UINT32          smb_base = 0xFFFFFFFF;

    for (bus = 0x00; bus <= PCI_MAX_BUS; bus++) {
        for (dev = 0x00; dev <= PCI_MAX_DEVICE; dev++) {
            for (fun = 0x00; fun <= PCI_MAX_FUNCTION; fun++) {

                IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, VENDOR_ID_OFFSET));
                vid_did = IoRead32 (PCI_CONFIG_DATA_PORT);

                if (vid_did != 0xFFFFFFFF && vid_did != 0x00000000) {

                    IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, RIVISION_ID_OFFSET));
                    class_code = IoRead32 (PCI_CONFIG_DATA_PORT);
                    class_code = BitFieldRead32 (class_code, 8, 31);

                    if (class_code == 0x0C0500) {

                        IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GET_PFA (bus, dev, fun, SMBUS_BASE_ADDRESS_OFFSET));
                        smb_base = IoRead32 (PCI_CONFIG_DATA_PORT);
                        //Print(L"SMBus Controller Found!\n");

                    }
                }
            }
        }
    }

    return smb_base;

}

VOID
EFIAPI
PrintSpdRegister (
    IN  UINT32  SmbusBaseAddress,
    IN  UINT8   SlaveAddress
)
{
    unsigned int    Index;
    STATIC  UINT8   Register[256];

    Print (L"Slave Address: %02x\n", SlaveAddress);

    //Print the first row.
    Print(L"   ");
    for(Index = 0x00; Index <= 0x0F; Index ++){
        Print(L"%02x ", Index);
    }
    Print(L"\n");

    for (Index = 0x00; Index <= 0xFF; Index ++) {

        IoWrite8 (SmbusBaseAddress + HSTS_OFFSET, 0xFE);
        IoWrite8 (SmbusBaseAddress + TSA_OFFSET, SlaveAddress + 1);
        IoWrite8 (SmbusBaseAddress + HCMD_OFFSET, (UINT8)Index);
        IoWrite8 (SmbusBaseAddress + HCTL_OFFSET, 0x48);

        //Check the BIT[0]. 1 indicates that the PCH is running a command from the host interface.
        while ((IoRead8 (SmbusBaseAddress) & 0x01) != 0) {
            //gBS->Stall (10);
        };

        //Print the first column.
        if (Index % 0x10 == 0x00) {
            Print (L"%02x ", Index);
        }

        //Check the BIT[2], BIT[3], BIT[4]. 1 indicates error.
        if ((IoRead8 (SmbusBaseAddress) & 0x1C) == 0) {
            Register[Index] = IoRead8 (SmbusBaseAddress + 0x05);
            Print (L"%02x ", Register[Index]);
        }

        //Next line when this line is full.
        if (Index % 0x10 == 0x0F) {
            Print (L"\n");
        }

    }
}

BOOLEAN
EFIAPI
CheckSmbusDeviceExist (
    IN  UINT32  SmbusBaseAddress,
    IN  UINT8   SlaveAddress
)
{
    UINT8   HostStatus;
    UINT8   HostCommand = 0x00;

    IoWrite8 (SmbusBaseAddress + HSTS_OFFSET, 0xFE); //0xFE: Clear
    IoWrite8 (SmbusBaseAddress + TSA_OFFSET, SlaveAddress + 1); //SlaveAddress + 1: Read
    IoWrite8 (SmbusBaseAddress + HCMD_OFFSET, HostCommand);
    IoWrite8 (SmbusBaseAddress + HCTL_OFFSET, 0x48); //0x48: Read byte

    //Check the first bit of offset 0x00. 1 indicates that the PCH is running a command from the host interface.
    while (IoRead8 (SmbusBaseAddress) & BIT00) {
        //gBS->Stall (10);
    };

    HostStatus = IoRead8 (SmbusBaseAddress + HSTS_OFFSET);

    if ((HostStatus & BIT02) == 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}