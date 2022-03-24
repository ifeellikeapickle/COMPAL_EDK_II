#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>

#include <Library/IoLib.h>

#include "SPDRead.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINT32          smb_base_addr;
    UINT8           slave_addr;

    smb_base_addr = GetSMBusBaseAddress ();
    Print (L"SMBus Base Address: %08x\n", smb_base_addr);

    //The first bit 1 indicates that the SMBus is IO mapped.
    smb_base_addr = smb_base_addr & 0xFFFFFFFE;
    Print (L"IO Space Base: %08x\n", smb_base_addr);

    for (slave_addr = 0xA0; slave_addr <= 0xA6; slave_addr += 0x02) {
        if (CheckSmbusDeviceExist (smb_base_addr, slave_addr) == TRUE) {
            PrintSpdRegister (smb_base_addr, slave_addr);
        }
        else {
            Print (L"Slave Address %02x does not exist!!\n", slave_addr);
        }
    }

    return Status;
}