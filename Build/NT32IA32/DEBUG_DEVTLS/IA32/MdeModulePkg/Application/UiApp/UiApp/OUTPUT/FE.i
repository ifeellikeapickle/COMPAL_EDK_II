#line 1 "q:\\MdeModulePkg\\Application\\UiApp\\BootMaint\\FE.vfr"
#line 1 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"
















































































































































































































extern unsigned char UiAppStrings[];



























#line 238 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"

#line 240 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdeModulePkg\\Application\\UiApp\\UiApp\\DEBUG\\UiAppStrDefs.h"
#line 1 "q:\\MdeModulePkg\\Application\\UiApp\\BootMaint\\FE.vfr"















#line 1 "q:\\mdemodulepkg\\application\\uiapp\\bootmaint\\FormGuid.h"




































































































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

#line 223 "q:\\mdemodulepkg\\application\\uiapp\\bootmaint\\FormGuid.h"

#line 17 "q:\\MdeModulePkg\\Application\\UiApp\\BootMaint\\FE.vfr"

formset
  guid = { 0x1f2d63e1, 0xfebd, 0x4dc7, {0x9c, 0xc5, 0xba, 0x2b, 0x1c, 0xef, 0x9c, 0x5b} },
  title = STRING_TOKEN(0x0063),
  help = STRING_TOKEN(0x0002),
  classguid = { 0x1f2d63e1, 0xfebd, 0x4dc7, {0x9c, 0xc5, 0xba, 0x2b, 0x1c, 0xef, 0x9c, 0x5b} },

  varstore FILE_EXPLORER_NV_DATA,
    varid = 0x1000,
    name = FeData,
    guid = { 0x1f2d63e1, 0xfebd, 0x4dc7, {0x9c, 0xc5, 0xba, 0x2b, 0x1c, 0xef, 0x9c, 0x5b} };

  form formid = 0x101E,
       title = STRING_TOKEN(0x0063);

       label 0x101E;
       label 0xffff;
  endform;

  form formid = 0x101F,
       title = STRING_TOKEN(0x0027);

       label 0x101F;
       label 0xffff;

       subtitle text = STRING_TOKEN(0x0002);

       string    varid    = FeData.BootDescriptionData,
                 questionid = 0x110F,
                 prompt   = STRING_TOKEN(0x002D),
                 help     = STRING_TOKEN(0x0002),
                 flags    = INTERACTIVE,
                 minsize  = 6,
                 maxsize  = 75,
       endstring;

       string    varid    = FeData.BootOptionalData,
                 questionid = 0x1110,
                 prompt   = STRING_TOKEN(0x0061),
                 help     = STRING_TOKEN(0x0002),
                 flags    = INTERACTIVE,
                 minsize  = 0,
                 maxsize  = 120,
       endstring;

       subtitle text = STRING_TOKEN(0x0002);

       text
         help   = STRING_TOKEN(0x002F),
         text   = STRING_TOKEN(0x002F),
         text   = STRING_TOKEN(0x0002),
         flags  = INTERACTIVE,
         key    = 0x1000;

       text
         help   = STRING_TOKEN(0x0030),
         text   = STRING_TOKEN(0x0030),
         text   = STRING_TOKEN(0x0002),
         flags  = INTERACTIVE,
         key    = 0x1001;

  endform;

  form formid = 0x1020,
       title = STRING_TOKEN(0x0028);

       label 0x1020;
       label 0xffff;

       subtitle text = STRING_TOKEN(0x0002);

       string    varid    = FeData.DriverDescriptionData,
                 questionid = 0x1111,
                 prompt   = STRING_TOKEN(0x002D),
                 help     = STRING_TOKEN(0x0002),
                 flags    = INTERACTIVE,
                 minsize  = 6,
                 maxsize  = 75,
       endstring;

       string    varid    = FeData.DriverOptionalData,
                 questionid = 0x1112,
                 prompt   = STRING_TOKEN(0x0061),
                 help     = STRING_TOKEN(0x0002),
                 flags    = INTERACTIVE,
                 minsize  = 0,
                 maxsize  = 120,
       endstring;

       checkbox varid    = FeData.ForceReconnect,
               prompt   = STRING_TOKEN(0x002E),
               help     = STRING_TOKEN(0x002E),
               flags    = CHECKBOX_DEFAULT,
               key      = 0,
       endcheckbox;

       subtitle text = STRING_TOKEN(0x0002);

       text
         help   = STRING_TOKEN(0x002F),
         text   = STRING_TOKEN(0x002F),
         text   = STRING_TOKEN(0x0002),
         flags  = INTERACTIVE,
         key    = 0x1002;  

       text
         help   = STRING_TOKEN(0x0030),
         text   = STRING_TOKEN(0x0030),
         text   = STRING_TOKEN(0x0002),
         flags  = INTERACTIVE,
         key    = 0x1003;

  endform;

endformset;
