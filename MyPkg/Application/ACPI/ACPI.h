#define ENCODING_OPERATION_REGION       0x805B
#define REGION_SPACE_SYSTEM_MEMORY      0x00

#pragma pack (1)
typedef struct {
    UINT16                          OpRegionOp;
    UINT32                          NameString;
    UINT8                           RegionSpace;
} OPERATION_REGION;
#pragma pack ()

EFI_STATUS
EFIAPI
PrintGuid (
    IN  EFI_GUID                    *Guid
);

EFI_STATUS
EFIAPI
SaveDSDTToFile (
	CHAR8	                        *Pointer,
	UINTN	                        Length
);