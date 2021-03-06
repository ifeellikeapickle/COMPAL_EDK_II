/**
  DO NOT EDIT
  FILE auto-generated
  Module name:
    AutoGen.h
  Abstract:       Auto-generated AutoGen.h for building module or library.
**/

#ifndef _AUTOGENH_80B92017_EC64_4923_938D_94FAEE85832E
#define _AUTOGENH_80B92017_EC64_4923_938D_94FAEE85832E

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>
#include <PiDxe.h>
#include <Library/PcdLib.h>

extern GUID  gEfiCallerIdGuid;
extern CHAR8 *gEfiCallerBaseName;


// PCD definitions
#define _PCD_TOKEN_PcdBrowserSubtitleTextColor  146U
extern const UINT8 _gPcd_FixedAtBuild_PcdBrowserSubtitleTextColor;
#define _PCD_GET_MODE_8_PcdBrowserSubtitleTextColor  _gPcd_FixedAtBuild_PcdBrowserSubtitleTextColor
//#define _PCD_SET_MODE_8_PcdBrowserSubtitleTextColor  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_SIZE_PcdBrowserSubtitleTextColor 1
#define _PCD_GET_MODE_SIZE_PcdBrowserSubtitleTextColor _PCD_SIZE_PcdBrowserSubtitleTextColor
#define _PCD_TOKEN_PcdBrowserFieldTextColor  147U
extern const UINT8 _gPcd_FixedAtBuild_PcdBrowserFieldTextColor;
#define _PCD_GET_MODE_8_PcdBrowserFieldTextColor  _gPcd_FixedAtBuild_PcdBrowserFieldTextColor
//#define _PCD_SET_MODE_8_PcdBrowserFieldTextColor  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_SIZE_PcdBrowserFieldTextColor 1
#define _PCD_GET_MODE_SIZE_PcdBrowserFieldTextColor _PCD_SIZE_PcdBrowserFieldTextColor
#define _PCD_TOKEN_PcdBrowserFieldTextHighlightColor  148U
extern const UINT8 _gPcd_FixedAtBuild_PcdBrowserFieldTextHighlightColor;
#define _PCD_GET_MODE_8_PcdBrowserFieldTextHighlightColor  _gPcd_FixedAtBuild_PcdBrowserFieldTextHighlightColor
//#define _PCD_SET_MODE_8_PcdBrowserFieldTextHighlightColor  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_SIZE_PcdBrowserFieldTextHighlightColor 1
#define _PCD_GET_MODE_SIZE_PcdBrowserFieldTextHighlightColor _PCD_SIZE_PcdBrowserFieldTextHighlightColor
#define _PCD_TOKEN_PcdBrowserFieldBackgroundHighlightColor  149U
extern const UINT8 _gPcd_FixedAtBuild_PcdBrowserFieldBackgroundHighlightColor;
#define _PCD_GET_MODE_8_PcdBrowserFieldBackgroundHighlightColor  _gPcd_FixedAtBuild_PcdBrowserFieldBackgroundHighlightColor
//#define _PCD_SET_MODE_8_PcdBrowserFieldBackgroundHighlightColor  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_SIZE_PcdBrowserFieldBackgroundHighlightColor 1
#define _PCD_GET_MODE_SIZE_PcdBrowserFieldBackgroundHighlightColor _PCD_SIZE_PcdBrowserFieldBackgroundHighlightColor
#define _PCD_TOKEN_PcdFrontPageFormSetGuid  150U
extern const UINT8 _gPcd_FixedAtBuild_PcdFrontPageFormSetGuid[];
#define _PCD_GET_MODE_PTR_PcdFrontPageFormSetGuid  (VOID *)_gPcd_FixedAtBuild_PcdFrontPageFormSetGuid
//#define _PCD_SET_MODE_PTR_PcdFrontPageFormSetGuid  ASSERT(FALSE)  // It is not allowed to set value for a FIXED_AT_BUILD PCD
#define _PCD_SIZE_PcdFrontPageFormSetGuid 16
#define _PCD_GET_MODE_SIZE_PcdFrontPageFormSetGuid _PCD_SIZE_PcdFrontPageFormSetGuid

EFI_STATUS
EFIAPI
CustomizedDisplayLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CustomizedDisplayLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );
#include "CustomizedDisplayLibStrDefs.h"


#ifdef __cplusplus
}
#endif

#endif
