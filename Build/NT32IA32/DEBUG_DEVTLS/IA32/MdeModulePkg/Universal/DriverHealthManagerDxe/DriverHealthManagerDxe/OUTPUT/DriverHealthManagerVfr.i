#line 1 "q:\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerVfr.Vfr"
#line 1 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerDxe\\DEBUG\\DriverHealthManagerDxeStrDefs.h"



























extern unsigned char DriverHealthManagerDxeStrings[];








#line 38 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerDxe\\DEBUG\\DriverHealthManagerDxeStrDefs.h"

#line 40 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerDxe\\DEBUG\\DriverHealthManagerDxeStrDefs.h"
#line 1 "q:\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerVfr.Vfr"














#line 1 "q:\\mdemodulepkg\\universal\\driverhealthmanagerdxe\\DriverHealthManagerVfr.h"
















#line 1 "q:\\MdePkg\\Include\\Guid/HiiPlatformSetupFormset.h"






























extern EFI_GUID gEfiHiiPlatformSetupFormsetGuid;
extern EFI_GUID { 0xf22fc20c, 0x8cf4, 0x45eb, { 0x8e, 0x6, 0xad, 0x4e, 0x50, 0xb9, 0x5d, 0xd3 }};
extern EFI_GUID gEfiHiiUserCredentialFormsetGuid;

#line 36 "q:\\MdePkg\\Include\\Guid/HiiPlatformSetupFormset.h"
#line 18 "q:\\mdemodulepkg\\universal\\driverhealthmanagerdxe\\DriverHealthManagerVfr.h"














#line 33 "q:\\mdemodulepkg\\universal\\driverhealthmanagerdxe\\DriverHealthManagerVfr.h"
#line 16 "q:\\MdeModulePkg\\Universal\\DriverHealthManagerDxe\\DriverHealthManagerVfr.Vfr"

formset
  guid      = { 0xcfb3b000, 0x0b63, 0x444b, { 0xb1, 0xd1, 0x12, 0xd5, 0xd9, 0x5d, 0xc4, 0xfc } },
  title     = STRING_TOKEN(0x0002),
  help      = STRING_TOKEN(0x0003),
  classguid = { 0x93039971, 0x8545, 0x4b04, { 0xb4, 0x5e, 0x32, 0xeb, 0x83, 0x26, 0x4, 0xe } },

  form formid = 0x1001,
      title  = STRING_TOKEN(0x0002);

      label 0x2000;
      label 0x2001;

      suppressif TRUE;
          text
              help  = STRING_TOKEN(0x0004),
              text  = STRING_TOKEN(0x0004),
              flags = INTERACTIVE,
              key   = 0x0001;
      endif;

  endform;
endformset;
