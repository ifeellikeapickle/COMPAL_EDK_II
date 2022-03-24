#line 1 "q:\\MdePkg\\Library\\BaseMemoryLibOptDxe\\Ia32\\CopyMem.asm"
#line 1 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdePkg\\Library\\BaseMemoryLibOptDxe\\BaseMemoryLibOptDxe\\DEBUG\\AutoGen.h"















#line 1 "q:\\MdePkg\\Include\\Base.h"



























#line 1 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"


























#pragma pack()
#line 29 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"


























#line 56 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"












#pragma warning ( disable : 4214 )




#pragma warning ( disable : 4100 )





#pragma warning ( disable : 4057 )




#pragma warning ( disable : 4127 )




#pragma warning ( disable : 4505 )




#pragma warning ( disable : 4206 )



















#line 115 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"

#line 117 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"




  
  
  

  
  
  
  typedef unsigned __int64    UINT64;
  
  
  
  typedef __int64             INT64;
  
  
  
  typedef unsigned __int32    UINT32;
  
  
  
  typedef __int32             INT32;
  
  
  
  typedef unsigned short      UINT16;
  
  
  
  
  typedef unsigned short      CHAR16;
  
  
  
  typedef short               INT16;
  
  
  
  
  typedef unsigned char       BOOLEAN;
  
  
  
  typedef unsigned char       UINT8;
  
  
  
  typedef char                CHAR8;
  
  
  
  typedef signed char         INT8;















































#line 219 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"





typedef UINT32  UINTN;




typedef INT32   INTN;








































  
  
  
  











#line 286 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"







#line 294 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"















#line 310 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"

#line 312 "q:\\MdePkg\\Include\\Ia32\\ProcessorBind.h"

#line 29 "q:\\MdePkg\\Include\\Base.h"





#pragma warning ( disable : 4200 )
#line 36 "q:\\MdePkg\\Include\\Base.h"


















extern UINT8 _VerifySizeofBOOLEAN[(sizeof(BOOLEAN) == (1)) / (sizeof(BOOLEAN) == (1))];
extern UINT8 _VerifySizeofINT8[(sizeof(INT8) == (1)) / (sizeof(INT8) == (1))];
extern UINT8 _VerifySizeofUINT8[(sizeof(UINT8) == (1)) / (sizeof(UINT8) == (1))];
extern UINT8 _VerifySizeofINT16[(sizeof(INT16) == (2)) / (sizeof(INT16) == (2))];
extern UINT8 _VerifySizeofUINT16[(sizeof(UINT16) == (2)) / (sizeof(UINT16) == (2))];
extern UINT8 _VerifySizeofINT32[(sizeof(INT32) == (4)) / (sizeof(INT32) == (4))];
extern UINT8 _VerifySizeofUINT32[(sizeof(UINT32) == (4)) / (sizeof(UINT32) == (4))];
extern UINT8 _VerifySizeofINT64[(sizeof(INT64) == (8)) / (sizeof(INT64) == (8))];
extern UINT8 _VerifySizeofUINT64[(sizeof(UINT64) == (8)) / (sizeof(UINT64) == (8))];
extern UINT8 _VerifySizeofCHAR8[(sizeof(CHAR8) == (1)) / (sizeof(CHAR8) == (1))];
extern UINT8 _VerifySizeofCHAR16[(sizeof(CHAR16) == (2)) / (sizeof(CHAR16) == (2))];







  
  
  
  
  
  







#line 86 "q:\\MdePkg\\Include\\Base.h"























#line 110 "q:\\MdePkg\\Include\\Base.h"
  
#line 112 "q:\\MdePkg\\Include\\Base.h"








  
#line 122 "q:\\MdePkg\\Include\\Base.h"





typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} GUID;




typedef UINT64 PHYSICAL_ADDRESS;




typedef struct _LIST_ENTRY LIST_ENTRY;




struct _LIST_ENTRY {
  LIST_ENTRY  *ForwardLink;
  LIST_ENTRY  *BackLink;
};










































































































































































































































































































































#line 482 "q:\\MdePkg\\Include\\Base.h"


















#line 501 "q:\\MdePkg\\Include\\Base.h"




typedef CHAR8 *VA_LIST;






























































#line 569 "q:\\MdePkg\\Include\\Base.h"




typedef UINTN  *BASE_LIST;

















































#line 624 "q:\\MdePkg\\Include\\Base.h"

















































































































typedef UINTN RETURN_STATUS;
































































































































































































































































































  #pragma intrinsic(_ReturnAddress)
  











  
























#line 1065 "q:\\MdePkg\\Include\\Base.h"

#line 1067 "q:\\MdePkg\\Include\\Base.h"


#line 17 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdePkg\\Library\\BaseMemoryLibOptDxe\\BaseMemoryLibOptDxe\\DEBUG\\AutoGen.h"

extern GUID  gEfiCallerIdGuid;
extern CHAR8 *gEfiCallerBaseName;







#line 28 "q:\\Build\\NT32IA32\\DEBUG_DEVTLS\\IA32\\MdePkg\\Library\\BaseMemoryLibOptDxe\\BaseMemoryLibOptDxe\\DEBUG\\AutoGen.h"
#line 1 "q:\\MdePkg\\Library\\BaseMemoryLibOptDxe\\Ia32\\CopyMem.asm"
;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http:
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   CopyMem.asm
;
; Abstract:
;
;   CopyMem function
;
; Notes:
;
;------------------------------------------------------------------------------

    .686
    .model  flat,C
    .xmm
    .code

;------------------------------------------------------------------------------
;  void *
;  InternalMemCopyMem (
;     void   *Destination,
;     void   *Source,
;     UINTN  Count
;    );
;------------------------------------------------------------------------------
InternalMemCopyMem  PROC    USES    esi edi
    mov     esi, [esp + 16]             ; esi <- Source
    mov     edi, [esp + 12]             ; edi <- Destination
    mov     edx, [esp + 20]             ; edx <- Count
    lea     eax, [esi + edx - 1]        ; eax <- End of Source
    cmp     esi, edi
    jae     @F
    cmp     eax, edi                    ; Overlapped?
    jae     @CopyBackward               ; Copy backward if overlapped
@@:
    xor     ecx, ecx
    sub     ecx, edi
    and     ecx, 15                     ; ecx + edi aligns on 16-byte boundary
    jz      @F
    cmp     ecx, edx
    cmova   ecx, edx
    sub     edx, ecx                    ; edx <- remaining bytes to copy
    rep     movsb
@@:
    mov     ecx, edx
    and     edx, 15
    shr     ecx, 4                      ; ecx <- # of DQwords to copy
    jz      @CopyBytes
    add     esp, -16
    movdqu  [esp], xmm0                 ; save xmm0
@@:
    movdqu  xmm0, [esi]                 ; esi may not be 16-bytes aligned
    movntdq [edi], xmm0                 ; edi should be 16-bytes aligned
    add     esi, 16
    add     edi, 16
    loop    @B
    mfence
    movdqu  xmm0, [esp]                 ; restore xmm0
    add     esp, 16                     ; stack cleanup
    jmp     @CopyBytes
@CopyBackward:
    mov     esi, eax                    ; esi <- Last byte in Source
    lea     edi, [edi + edx - 1]        ; edi <- Last byte in Destination
    std
@CopyBytes:
    mov     ecx, edx
    rep     movsb
    cld
    mov     eax, [esp + 12]             ; eax <- Destination as return value
    ret
InternalMemCopyMem  ENDP

    END
