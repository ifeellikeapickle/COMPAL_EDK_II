typedef struct {
  UINT8                  OpRegionOp;
  UINT32                 NameString;
  UINT8                  RegionSpace;
  UINT8                  DWordPrefix;
  UINT32                 RegionOffset;
  UINT8                  BytePrefix;
  UINT8                  RegionLen;
} AML_OP_REGION_32_8;

EFI_STATUS
EFIAPI
GetOpRegion (
    EFI_ACPI_DESCRIPTION_HEADER    *Table
);

EFI_STATUS
EFIAPI
PrintGuid (
    IN  EFI_GUID    *Guid
);

EFI_STATUS
EFIAPI
SaveDSDTToFile (
	CHAR8	*Pointer,
	UINTN	Length
);