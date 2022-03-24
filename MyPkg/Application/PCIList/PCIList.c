#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/PciLib.h>

#include "PCIList.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    unsigned short  bus, dev, fun, off, i, count;
    UINT16          vid, did, svid, sdid;
    UINT32          class_code, link_cap, reg[16];
    UINT8           base_class_code, pcie_cap_id_off, pcie_cap_id, next_cap_ptr, aspm_sup_val;
    CHAR8           *aspm_sup;

    aspm_sup = (CHAR8 *)L"                    ";

    //Print the title.
    Print (L"  Bus#  Dev#  Fun#     VID     DID    SVID    SDID    Class Code            ASPM Support\n");
    
    for(bus = 0x00; bus <= PCI_MAX_BUS; bus++){
        for(dev = 0x00; dev <= PCI_MAX_DEVICE; dev++){
            for(fun = 0x00; fun <= PCI_MAX_FUNCTION; fun++){

                vid = PciRead16 (PCI_LIB_ADDRESS (bus, dev, fun, VENDOR_ID_OFFSET));

                if(vid != 0xFFFF && vid != 0x0000){

                    did = PciRead16 (PCI_LIB_ADDRESS (bus, dev, fun, DEVICE_ID_OFFSET));
                    svid = PciRead16 (PCI_LIB_ADDRESS (bus, dev, fun, SUBSYSTEM_VENDOR_ID_OFFSET));
                    sdid = PciRead16 (PCI_LIB_ADDRESS (bus, dev, fun, SUBSYSTEM_DEVICE_ID_OFFSET));
                    class_code = PciRead32 (PCI_LIB_ADDRESS (bus, dev, fun, CLASS_CODE_OFFSET-1));
                    class_code = BitFieldRead32 (class_code, 8, 31);
                    base_class_code = (UINT8) (BitFieldRead32 (class_code, 16, 23));
                    count = 1;

                    //Store the Configuration Space Header Register in array.
                    for(off = 0x00, i = 0; off <= 0x3C, i < 16; off+=0x04, i++){
                        reg[i] = PciRead32 (PCI_LIB_ADDRESS (bus, dev, fun, off));
                    }

                    //Looking for PCIe Cap ID offset.
                    pcie_cap_id_off = PciRead8 (PCI_LIB_ADDRESS (bus, dev, fun, CAPABILITIES_POINTER_OFFSET)); //80
                    next_cap_ptr = PciRead8 (PCI_LIB_ADDRESS (bus, dev, fun, pcie_cap_id_off + 0x01));//90 or 00 
                    pcie_cap_id = PciRead8 (PCI_LIB_ADDRESS (bus, dev, fun, pcie_cap_id_off)); //01 or 10
                    
                    while(pcie_cap_id != 0x10 && next_cap_ptr != 0x00){
                        pcie_cap_id_off = next_cap_ptr;
                        next_cap_ptr = pcie_cap_id_off + 0x01; 
                        pcie_cap_id = PciRead8 (PCI_LIB_ADDRESS (bus, dev, fun, pcie_cap_id_off));
                        next_cap_ptr = PciRead8 (PCI_LIB_ADDRESS (bus, dev, fun, next_cap_ptr));
                        //Print(L"Looking for L0s/L1 register for the %d time(s).\n", count);
                        count++;
                    }
                    
                    if(next_cap_ptr == 0x00){}
                    else if(pcie_cap_id == 0x10){
                        link_cap = PciRead32 (PCI_LIB_ADDRESS (bus, dev, fun, pcie_cap_id_off + 0x0C));
                        aspm_sup_val = (UINT8)BitFieldRead32 (link_cap, 10, 11);
                        //aspm_sup_val = BitFieldRead32 (link_cap, 10, 11);
                        //Print(L"L0s/L1 Register = %08lx\n", link_cap);

                        
                        switch (aspm_sup_val) {
                            case 0x00:
                                aspm_sup = (CHAR8 *)L"                    ";
                                break;
                            case 0x01:
                                aspm_sup = (CHAR8 *)L"       L0s Supported";
                                break;
                            case 0x02:
                                aspm_sup = (CHAR8 *)L"        L1 Supported";
                                break;
                            case 0x03:
                                aspm_sup = (CHAR8 *)L"L0s and L1 Supported";
                                break;
                            default:
                                aspm_sup = (CHAR8 *)L"                    ";
                                break;
                        }
                        
                    }
                    else{}

                    Print(L"    %02x    %02x    %02x    %04x    %04x    %04x    %04x        %06x    %s\n", bus, dev, fun, vid, did, svid, sdid, class_code, aspm_sup);
                    //Print(L"    %02x    %02x    %02x    %04x    %04x    %04x    %04x        %06x    %08x    %08x\n", bus, dev, fun, vid, did, svid, sdid, class_code, link_cap, aspm_sup_val);
                }
            }
        }
    }
    return EFI_SUCCESS;
}