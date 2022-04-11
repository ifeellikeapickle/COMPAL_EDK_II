#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include <Library/ShellCEntryLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h> // gST, gBS

// ACPI
#include <Guid/Acpi.h>
#include <IndustryStandard/Acpi.h>

//#include <Protocol/SimpleFileSystem.h>
#include "ACPI.h"

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS                                      Status;
    UINTN                                           Index;
    UINTN                                           EntryCount;
    UINT8                                           Encoding1;
    UINT8                                           Encoding2;
    UINT16                                          RsdpSignature[20];
    UINT16                                          RsdpOemId[20];
    UINT16                                          XsdtSignature[20];
    UINT16                                          FadtSignature[20];
    UINT16                                          DsdtSignature[20];
    UINT64                                          *EntryPtr;
    EFI_ACPI_DESCRIPTION_HEADER                     *Xsdt;
    EFI_ACPI_DESCRIPTION_HEADER                     *Dsdt;
    EFI_ACPI_DESCRIPTION_HEADER                     *Entry;
    EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE       *Fadt;
    EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER    *Rsdp;
    

    Status = EfiGetSystemConfigurationTable (&gEfiAcpi20TableGuid, (VOID **)&Rsdp);

    if (Rsdp == NULL) {
        Print (L"RSDP not found!\n");
        Status = EFI_NOT_FOUND;
    } else {
        Print (L"     RSDP Address: [0x%x]\n", Rsdp);

        ZeroMem (RsdpSignature, sizeof(RsdpSignature));
        for (Index = 0; Index < 8; Index++) {
            RsdpSignature[Index] = (Rsdp->Signature >> (Index * 8) & 0xFF);
        }
        ZeroMem (RsdpOemId, sizeof(RsdpOemId));
        for (Index = 0; Index < 6; Index++) {
            RsdpOemId[Index] = (Rsdp->OemId[Index] & 0xFF);
        }

        Print (L"   RSDP Signature: %s\n", RsdpSignature);
        Print (L"      RSDP OEM ID: %s\n", RsdpOemId);
        Print (L"    RSDP Revision: %d\n", Rsdp->Revision);
        Print (L"     RSDT Address: [0x%x], Length: %d\n", Rsdp->RsdtAddress, Rsdp->Length);
		Print (L"     XSDT Address: [0x%lx]\n", Rsdp->XsdtAddress);

        if (Rsdp->Revision >= EFI_ACPI_6_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {

            Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(Rsdp->XsdtAddress);

            ZeroMem (XsdtSignature, sizeof(XsdtSignature));
            for (Index = 0; Index < 4; Index++) {
                XsdtSignature[Index] = (Xsdt->Signature >> (Index * 8) & 0xFF);
            }
            Print (L"   XSDT Signature: %s\n", XsdtSignature);
            Print (L"      XSDT Length: %d\n", Xsdt->Length);

            EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (UINT64);
            Print (L" XSDT Entry Count: %d\n", EntryCount);

            EntryPtr = (UINT64 *)(Xsdt + 1); // Skip the header?
            
            for (Index = 0; Index < EntryCount; Index++, EntryPtr++) {
                Entry = (EFI_ACPI_DESCRIPTION_HEADER *)(*EntryPtr);
                //Print (L"  Entry Signature: 0x%x\n", Entry->Signature);

                if (Entry->Signature == EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
                    Fadt = (EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE *)Entry;
                    Dsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)(Fadt->Dsdt);

                    ZeroMem (FadtSignature, sizeof(FadtSignature));
                    for (Index = 0; Index < 4; Index++) {
                        FadtSignature[Index] = (Fadt->Header.Signature >> (Index * 8) & 0xFF);
                    }
                    ZeroMem (DsdtSignature, sizeof(DsdtSignature));
                    for (Index = 0; Index < 4; Index++) {
                        DsdtSignature[Index] = (Dsdt->Signature >> (Index * 8) & 0xFF);
                    }

                    Print (L"   FADT Signature: %s\n", FadtSignature);
                    Print (L"      FADT Length: %d\n", Fadt->Header.Length);
                    Print (L"    FADT Revision: %d\n", Fadt->Header.Revision);
                    Print (L"     DSDT Address: 0x%x\n", Dsdt);
                    Print (L"   DSDT Signature: %s\n", DsdtSignature);
                    Print (L"      DSDT Length: %d\n", Dsdt->Length);

                    for (Index = 0; Index < 10; Index++, Dsdt++) {
                        Encoding1 = (UINTN)Dsdt & 0xFF;
                        Encoding2 = (UINTN)(Dsdt+1) & 0xFF;
                        Print (L"E1 = %02x\n", Encoding1);
                        Print (L"E2 = %02x\n", Encoding2);
                    }
                    /*
                    for (Index = 0x00; Index < Fadt->Header.Length; Index++, Fadt++) {
                        Register = (UINT64)Fadt & 0xFF;
                        Print (L" %02x", Register);
                        if (Index % 0x10 == 0x0F) {
                            Print (L"\n");
                        }
                    }
                    */
                }
            }
        }
    }
    return Status;
}