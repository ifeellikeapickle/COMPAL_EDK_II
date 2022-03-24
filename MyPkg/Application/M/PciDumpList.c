
#include <Uefi.h>
#include <Base.h>

#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>

EFI_STATUS
UefiMain(IN EFI_HANDLE  ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable)
{
  INT16 b,d,f;
  UINT32 Vid,Did;
  
  Print(L"Device ID    Vendor ID    Bus#  Device#  Function#\n");
  
  for(b=0;b<0x100;b++){
    for(d=0;d<0x20;d++){
      for(f=0;f<0x08;f++){
        Vid=PciRead32(PCI_LIB_ADDRESS(b,d,f,0x00));
        
        if(Vid!=0xffffffff && Vid!=0x00){
          Did=PciRead32(PCI_LIB_ADDRESS(b,d,f,0x02));
          
          Print(L"%lx         %lx         ",Did>>16,Vid<<16>>16);
          Print(L"%02lx    %02lx       %02lx\n",b,d,f);
        }
      }
    }
  }return EFI_SUCCESS;
}
