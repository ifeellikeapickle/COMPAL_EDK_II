#include <Library/IoLib.h>
#include <Library/PciLib.h>

#define PCI_CONFIG_ADDRESS_PORT     0xCF8
#define PCI_CONFIG_DATA_PORT        0xCFC
#define GPIO_BASE_ADDRESS_PFA       0x8000F848
#define GPIO_CONTROL_PFA            0x8000F84C
#define GPIO_ENABLE                 0x10

VOID
EnableGpio (VOID)
{
    //UINT32      GpioBaseAddress;
    UINT32      GpioControl;
    IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GPIO_CONTROL_PFA);
    IoWrite32 (PCI_CONFIG_DATA_PORT, GPIO_ENABLE);
    IoWrite32 (PCI_CONFIG_ADDRESS_PORT, GPIO_CONTROL_PFA);
    GpioControl = IoRead32 (PCI_CONFIG_DATA_PORT);
    Print (L"GPIO Control: %08x\n", GpioControl);
}