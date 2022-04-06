EFI_STATUS
EFIAPI
PrintGuid (
    IN  EFI_GUID    *Guid
)
{
    EFI_STATUS  Status;

    if (Guid == NULL) {
        Print(L"GUID is NULL!\n");
        Status = EFI_INVALID_PARAMETER;
    } else {
        Print (
        L"GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
        (UINTN)Guid->Data1,
        (UINTN)Guid->Data2,
        (UINTN)Guid->Data3,
        (UINTN)Guid->Data4[0],
        (UINTN)Guid->Data4[1],
        (UINTN)Guid->Data4[2],
        (UINTN)Guid->Data4[3],
        (UINTN)Guid->Data4[4],
        (UINTN)Guid->Data4[5],
        (UINTN)Guid->Data4[6],
        (UINTN)Guid->Data4[7]
        );
        Status = EFI_SUCCESS;
    }

    return Status;
}