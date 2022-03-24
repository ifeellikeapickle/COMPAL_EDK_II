//SMBIOS GUID
#include <Guid/SmBios.h>

//SMBIOS structure
#include <IndustryStandard/SmBios.h>

//Memory
#include <Library/BaseMemoryLib.h> //CompareGuid
#include <Library/MemoryAllocationLib.h>

//Shell Library
#include <Library/ShellLib.h> //ShellPkg
#include <Library/UefiBootServicesTableLib.h>

//UefiShellDebug1CommandsLib
#include <Library/BcfgCommandLib.h>
#include <Library/DebugLib.h>

#include <Library/IoLib.h>

STATIC UINT8                        mInit           = 0;
STATIC SMBIOS_TABLE_ENTRY_POINT     *mSmbiosTable   = NULL;
STATIC SMBIOS_STRUCTURE_POINTER     m_SmbiosStruct;
STATIC SMBIOS_STRUCTURE_POINTER     *mSmbiosStruct = &m_SmbiosStruct;

EFI_STATUS
LibSmbiosInit (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // Init only once
  //
  if (mInit == 1) {
    return EFI_SUCCESS;
  }
  //
  // Get SMBIOS table from System Configure table
  //
  Status = EfiGetSystemConfigurationTable (&gEfiSmbiosTableGuid, (VOID**)&mSmbiosTable);

  if (mSmbiosTable == NULL) {
    return EFI_NOT_FOUND;
  }

  if (EFI_ERROR (Status)) {
    Print(L"Get SMBIOS Table error.\n");
    return Status;
  }
  //
  // Init SMBIOS structure table address
  //
  mSmbiosStruct->Raw  = (UINT8 *) (UINTN) (mSmbiosTable->TableAddress);

  mInit               = 1;
  return EFI_SUCCESS;
}

VOID
EFIAPI
PrintSmbiosTableRegister (
    IN  UINT32  SmbiosTableAddress
)
{
    UINT8   reg8;
    UINT32  reg32;
    reg8 = IoRead8 (SmbiosTableAddress);
    reg32 = IoRead32 (SmbiosTableAddress);
    Print (L"%02x\n", reg8);
    Print (L"%08x\n", reg32);
}