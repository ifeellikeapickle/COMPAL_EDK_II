#line 1 "q:\\MdeModulePkg\\Universal\\DriverSampleDxe\\Inventory.vfr"
#line 1 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverSampleDxe\\DriverSampleDxe\\DEBUG\\DriverSampleStrDefs.h"




































































































































































































extern unsigned char DriverSampleStrings[];


















#line 217 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverSampleDxe\\DriverSampleDxe\\DEBUG\\DriverSampleStrDefs.h"

#line 219 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Universal\\DriverSampleDxe\\DriverSampleDxe\\DEBUG\\DriverSampleStrDefs.h"
#line 1 "q:\\MdeModulePkg\\Universal\\DriverSampleDxe\\Inventory.vfr"















#line 1 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"



























#line 1 "q:\\MdePkg\\Include\\Guid/HiiPlatformSetupFormset.h"






























extern EFI_GUID gEfiHiiPlatformSetupFormsetGuid;
extern EFI_GUID gEfiHiiDriverHealthFormsetGuid;
extern EFI_GUID gEfiHiiUserCredentialFormsetGuid;

#line 36 "q:\\MdePkg\\Include\\Guid/HiiPlatformSetupFormset.h"
#line 29 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"
#line 1 "q:\\MdePkg\\Include\\Guid/HiiFormMapMethodGuid.h"






















extern EFI_GUID gEfiHiiStandardFormGuid;

#line 26 "q:\\MdePkg\\Include\\Guid/HiiFormMapMethodGuid.h"
#line 30 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"
#line 1 "q:\\MdeModulePkg\\Include\\Guid/DriverSampleHii.h"
































extern EFI_GUID { 0xA04A27f4, 0xDF00, 0x4D42, { 0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3D }};
extern EFI_GUID { 0xb3f56470, 0x6141, 0x4621, { 0x8f, 0x19, 0x70, 0x4e, 0x57, 0x7a, 0xa9, 0xe8 }};
extern EFI_GUID { 0xF5E655D9, 0x02A6, 0x46f2, { 0x9E, 0x76, 0xB8, 0xBE, 0x8E, 0x60, 0xAB, 0x22 }};

#line 38 "q:\\MdeModulePkg\\Include\\Guid/DriverSampleHii.h"
#line 31 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"
#line 1 "q:\\MdeModulePkg\\Include\\Guid/ZeroGuid.h"






















extern EFI_GUID gZeroGuid;

#line 26 "q:\\MdeModulePkg\\Include\\Guid/ZeroGuid.h"
#line 32 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"



#pragma pack(1)
typedef struct {
  UINT16  WhatIsThePassword[20];
  UINT16  WhatIsThePassword2[20];
  UINT16  MyStringData[40];
  UINT16  PasswordClearText[20];
  UINT16  SomethingHiddenForHtml;
  UINT8   HowOldAreYouInYearsManual;
  UINT16  HowTallAreYouManual;
  UINT8   HowOldAreYouInYears;
  UINT16  HowTallAreYou;
  UINT8   MyFavoriteNumber;
  UINT8   TestLateCheck;
  UINT8   TestLateCheck2;
  UINT8   QuestionAboutTreeHugging;
  UINT8   ChooseToActivateNuclearWeaponry;
  UINT8   SuppressGrayOutSomething;
  UINT8   OrderedList[8];
  UINT16  BootOrder[8];
  UINT8   BootOrderLarge;
  UINT8   DynamicRefresh;
  UINT8   DynamicOneof;
  UINT8   DynamicOrderedList[5];
  UINT8   Reserved;
  EFI_HII_REF RefData;
  UINT8   NameValueVar0;
  UINT16  NameValueVar1;
  UINT16  NameValueVar2[20];
  UINT8   SerialPortNo;
  UINT8   SerialPortStatus;
  UINT16  SerialPortIo;
  UINT8   SerialPortIrq;
  UINT8   GetDefaultValueFromCallBack;
  UINT8   GetDefaultValueFromAccess;
  EFI_HII_TIME  Time;
  UINT8   RefreshGuidCount;
  UINT8   Match2;
} DRIVER_SAMPLE_CONFIGURATION;




typedef struct {
  UINT8         Field8;
  UINT16        Field16;
  UINT8         OrderedList[3];
} MY_EFI_VARSTORE_DATA;









#pragma pack()

#line 94 "q:\\mdemodulepkg\\universal\\driversampledxe\\NVDataStruc.h"
#line 17 "q:\\MdeModulePkg\\Universal\\DriverSampleDxe\\Inventory.vfr"

formset
  guid     = { 0xb3f56470, 0x6141, 0x4621, {0x8f, 0x19, 0x70, 0x4e, 0x57, 0x7a, 0xa9, 0xe8} },
  title    = STRING_TOKEN(0x0002),
  help     = STRING_TOKEN(0x0003),

  form formid = 1,
       title  = STRING_TOKEN(0x0004);  

  	text
      help   = STRING_TOKEN(0x0006),
      text   = STRING_TOKEN(0x0005),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x0007),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x0008),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x0009),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

    subtitle text = STRING_TOKEN(0x0012);

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000A),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000B),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000C),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000D),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000E),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x000F),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

  	text
      help   = STRING_TOKEN(0x0012),
      text   = STRING_TOKEN(0x0010),
	    text   = STRING_TOKEN(0x0012),
      flags  = 0,
      key    = 0;

    text
      help   = STRING_TOKEN(0x0021),
      text   = STRING_TOKEN(0x0021),
      flags  = INTERACTIVE,
      key    = 0x1231;

    subtitle text = STRING_TOKEN(0x0012);

    subtitle text = STRING_TOKEN(0x0011);

  endform;

endformset;
