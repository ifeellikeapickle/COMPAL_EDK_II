#line 1 "q:\\MdeModulePkg\\Application\\UiApp\\FrontPageVfr.Vfr"
#line 1 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"
















































































































































































































extern unsigned char UiAppStrings[];



























#line 238 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"

#line 240 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"
#line 1 "q:\\MdeModulePkg\\Application\\UiApp\\FrontPageVfr.Vfr"
















#line 1 "q:\\mdemodulepkg\\application\\uiapp\\FormsetGuid.h"



















#line 1 "q:\\mdemodulepkg\\application\\uiapp\\BootMaint/FormGuid.h"




































































































typedef struct {
  
  
  
  
  UINT16  BootTimeOut;
  UINT16  BootNext;

  
  
  
  UINT8   COM1BaudRate;
  UINT8   COM1DataRate;
  UINT8   COM1StopBits;
  UINT8   COM1Parity;
  UINT8   COM1TerminalType;

  
  
  
  UINT8   COM2BaudRate;
  UINT8   COM2DataRate;
  UINT8   COM2StopBits;
  UINT8   COM2Parity;
  UINT8   COM2TerminalType;

  
  
  
  UINT16  DriverAddHandleDesc[100];
  UINT16  DriverAddHandleOptionalData[100];
  UINT8   DriverAddActive;
  UINT8   DriverAddForceReconnect;

  
  
  
  UINT8   ConsoleInputCOM1;
  UINT8   ConsoleInputCOM2;
  UINT8   ConsoleOutputCOM1;
  UINT8   ConsoleOutputCOM2;
  UINT8   ConsoleErrorCOM1;
  UINT8   ConsoleErrorCOM2;

  
  
  
  UINT8   ConsoleCheck[100];
  
  
  
  
  UINT8   ConsoleInCheck[100];
  UINT8   ConsoleOutCheck[100];
  UINT8   ConsoleErrCheck[100];

  
  
  
  
  
  UINT32  BootOptionOrder[100];
  UINT32  DriverOptionOrder[100];
  
  
  
  BOOLEAN BootOptionDel[100];
  BOOLEAN DriverOptionDel[100];
  BOOLEAN BootOptionDelMark[100];
  BOOLEAN DriverOptionDelMark[100];

  
  
  
  UINT8   COMBaudRate[100];
  UINT8   COMDataRate[100];
  UINT8   COMStopBits[100];
  UINT8   COMParity[100];
  UINT8   COMTerminalType[100];
  UINT8   COMFlowControl[100];

  
  
  
  
  
  
  UINT8   DisableMap[32];

  
  
  
  UINT16  ConsoleOutMode;

  
  
  
} BMM_FAKE_NV_DATA;












typedef struct {
  UINT16  BootDescriptionData[100];
  UINT16  BootOptionalData[127];
  UINT16  DriverDescriptionData[100];
  UINT16  DriverOptionalData[127];
  BOOLEAN BootOptionChanged;
  BOOLEAN DriverOptionChanged;
  UINT8   Active;
  UINT8   ForceReconnect;
} FILE_EXPLORER_NV_DATA;

#line 223 "q:\\mdemodulepkg\\application\\uiapp\\BootMaint/FormGuid.h"

#line 21 "q:\\mdemodulepkg\\application\\uiapp\\FormsetGuid.h"






























#line 52 "q:\\mdemodulepkg\\application\\uiapp\\FormsetGuid.h"
#line 18 "q:\\MdeModulePkg\\Application\\UiApp\\FrontPageVfr.Vfr"






















formset
  guid     = { 0x9e0c30bc, 0x3f06, 0x4ba6, 0x82, 0x88, 0x9, 0x17, 0x9b, 0x85, 0x5d, 0xbe },
  title    = STRING_TOKEN(0x007B),
  help     = STRING_TOKEN(0x0002),
  classguid = { 0x9e0c30bc, 0x3f06, 0x4ba6, 0x82, 0x88, 0x9, 0x17, 0x9b, 0x85, 0x5d, 0xbe },

  form formid = 0x1000,
       title  = STRING_TOKEN(0x007B);

    banner
      title = STRING_TOKEN(0x007C),
      line  1,
      align left;

    banner
      title = STRING_TOKEN(0x007D),
      line  2,
      align left;

    banner
      title = STRING_TOKEN(0x007E),
      line  2,
      align right;

    banner
      title = STRING_TOKEN(0x0080),
      line  3,
      align left;

    banner
      title = STRING_TOKEN(0x007F),
      line  3,
      align right;
































    text
      help    = STRING_TOKEN(0x0088),
      text    = STRING_TOKEN(0x0087),
      flags   = INTERACTIVE,
      key     = 0x1000;

    label 0x1000;
    
    
    
    
    label 0xffff;
          
    goto 
      formsetguid = { 0x3ebfa8e6, 0x511d, 0x4b5b, 0xa9, 0x5f, 0xfb, 0x38, 0x26, 0xf, 0x1c, 0x27 },
      formid      = 0,
      question    = 0,
      prompt  = STRING_TOKEN(0x008F),
      help    = STRING_TOKEN(0x0090),
      flags   = INTERACTIVE,
      key     = 0x8567;

               
    goto 
      formsetguid = { 0x847bc3fe, 0xb974, 0x446d, 0x94, 0x49, 0x5a, 0xd5, 0x41, 0x2e, 0x99, 0x3b }  ,
      formid      = 0,
      question    = 0,
      prompt      = STRING_TOKEN(0x008B),
      help        = STRING_TOKEN(0x008C),
      flags       = INTERACTIVE,
      key         = 0x1064;


    goto
      formsetguid = { 0x642237c7, 0x35d4, 0x472d, {0x83, 0x65, 0x12, 0xe0, 0xcc, 0xf2, 0x7a, 0x22} },
      formid      = 0,
      question    = 0,
      prompt  = STRING_TOKEN(0x008D),
      help    = STRING_TOKEN(0x008E),
      flags   = INTERACTIVE,
      key     = 0x9876;

  endform;

endformset;
