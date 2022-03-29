#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

//Memory
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <CPUID.h>

VOID
PrintProcessorSignature (
    VOID
    )
{
    UINT32 eax, ebx, ecx, edx;
    CHAR8 signature[12];
    AsmCpuid (CPUID_SIGNATURE, &eax, &ebx, &ecx, &edx);
    *(UINT32*)(signature + 0) = ebx;
    *(UINT32*)(signature + 4) = edx;
    *(UINT32*)(signature + 8) = ecx;
    //Print(L"EAX: %08x\nEBX: %08x\nECX: %08x\nEDX: %08x\n", eax, ebx, ecx, edx);
    Print(L"   Signature: %a\n", signature);
}

VOID
PrintProcessorBrandString (
    VOID
    )
{
    UINT32 eax, ebx, ecx, edx;
    CHAR8 brand_string[48];

    AsmCpuid (CPUID_BRAND_STRING1, &eax, &ebx, &ecx, &edx);
    *(UINT32*)(brand_string + 0) = eax;
    *(UINT32*)(brand_string + 4) = ebx;
    *(UINT32*)(brand_string + 8) = ecx;
    *(UINT32*)(brand_string + 12) = edx;
    //Print(L"EAX: %08x\nEBX: %08x\nECX: %08x\nEDX: %08x\n", eax, ebx, ecx, edx);

    AsmCpuid (CPUID_BRAND_STRING2, &eax, &ebx, &ecx, &edx);
    *(UINT32*)(brand_string + 16) = eax;
    *(UINT32*)(brand_string + 20) = ebx;
    *(UINT32*)(brand_string + 24) = ecx;
    *(UINT32*)(brand_string + 28) = edx;
    //Print(L"EAX: %08x\nEBX: %08x\nECX: %08x\nEDX: %08x\n", eax, ebx, ecx, edx);

    AsmCpuid (CPUID_BRAND_STRING3, &eax, &ebx, &ecx, &edx);
    *(UINT32*)(brand_string + 32) = eax;
    *(UINT32*)(brand_string + 36) = ebx;
    *(UINT32*)(brand_string + 40) = ecx;
    *(UINT32*)(brand_string + 44) = edx;
    //Print(L"EAX: %08x\nEBX: %08x\nECX: %08x\nEDX: %08x\n", eax, ebx, ecx, edx);

    Print(L"Brand String: %a\n", brand_string);
}

VOID
PrintProcessorVersionInfo (
    VOID
    )
{
    UINT32 eax, ebx, ecx, edx;
    UINT8 family, model, stepping;
    AsmCpuid (CPUID_VERSION_INFO, &eax, &ebx, &ecx, &edx);
    //Print(L"EAX: %08x\nEBX: %08x\nECX: %08x\nEDX: %08x\n", eax, ebx, ecx, edx);
    family   = (UINT8) (BitFieldRead32 (eax, 8, 11));
    model    = (UINT8) (BitFieldRead32 (eax, 4, 7));
    stepping = (UINT8) (BitFieldRead32 (eax, 0, 3));
    Print(L"      Family: %1x\n       Model: %1x\n    Stepping: %1x\n", family, model, stepping);
}

VOID
PrintProcessorFeaturesInfo (
    VOID
    )
{
    UINT32  eax, ebx, ecx, edx;
    UINT32  xeax, xebx, xecx, xedx;
    UINT32  Col = 0;
    UINT8   Width = 60;
    CHAR16  features[1000];
    CHAR16  buf[80];
    CHAR16  *f = features;
    BOOLEAN FirstRow = TRUE;


    ZeroMem (features, 1000);

    AsmCpuid(CPUID_VERSION_INFO, &eax, &ebx, &ecx, &edx);
    AsmCpuid(CPUID_EXTENDED_CPU_SIG, &xeax, &xebx, &xecx, &xedx);

    // Presorted list. No sorting routine!
    if (edx & BIT22) StrCat(features, L" ACPI");             // ACPI via MSR Support
    if (ecx & BIT25) StrCat(features, L" AESNI");
    if (edx & BIT09) StrCat(features, L" APIC");
    if (ecx & BIT28) StrCat(features, L" AVX");               // Advanced Vector Extensions
    if (edx & BIT19) StrCat(features, L" CLFSH");           // CLFLUSH (Cache Line Flush) Instruction Support
    if (edx & BIT15) StrCat(features, L" CMOV");             // CMOV Instructions Extension
    if (ecx & BIT13) StrCat(features, L" CMPXCHG16B"); // CMPXCHG16B Instruction Support
    if (ecx & BIT10) StrCat(features, L" CNXT-ID");       // L1 Cache Adaptive Or Shared Mode Support
    if (edx & BIT08) StrCat(features, L" CX8");               // CMPXCHG8 Instruction Support
    if (ecx & BIT18) StrCat(features, L" DCA");               // Direct Cache Access
    if (edx & BIT02) StrCat(features, L" DE");                 // Debugging Extensions
    if (edx & BIT21) StrCat(features, L" DS");                 // Dubug Store Support
    if (ecx & BIT04) StrCat(features, L" DS-CPL");         // CPL Qual. Debug Store Support
    if (ecx & BIT02) StrCat(features, L" DTES64");         // 64-bit Debug Store Support
    if (ecx & BIT07) StrCat(features, L" EIST");
    if (ecx & BIT29) StrCat(features, L" F16C");             // 16-bit FP Conversion Instructions Support
    if (ecx & BIT12) StrCat(features, L" FMA");               // Fused Multiply-Add
    if (edx & BIT00) StrCat(features, L" FPU");               // Floating Point Unit
    if (edx & BIT24) StrCat(features, L" FXSR");             // FXSAVE/FXRSTOR Support
    if (edx & BIT28) StrCat(features, L" HTT");               // Max APIC IDs reserved field is Valid
    if (xedx & BIT29) StrCat(features, L" Intel_64_Architecture");
    if (xecx & BIT00) StrCat(features, L" LAHF/SAHF");
    //if (xedx.Bits.LM) StrCat(features, L" LM");
    if (xecx & BIT05) StrCat(features, L" LZCNT");
    if (edx & BIT14) StrCat(features, L" MCA");               // Machine Check Architecture
    if (edx & BIT07) StrCat(features, L" MCE");               // Machine Check Exception
    if (ecx & BIT03) StrCat(features, L" MONITOR");       // Monitor/Mwait Support (SSE3 supplements)
    if (edx & BIT23) StrCat(features, L" MMX");               // Multimedia Extensions
    if (ecx & BIT22) StrCat(features, L" MOVBE");           // Move Data After Swapping Bytes Instruction Support
    if (edx & BIT05) StrCat(features, L" MSR");               // Model Specific Registers
    if (edx & BIT12) StrCat(features, L" MTRR");             // Memory Type Range Registers
    //if (xedx.Bits.NX) StrCat(features, L" NX");
    if (ecx & BIT27) StrCat(features, L" OSXSAVE");       // OS has set bit to support CPU extended state management using XSAVE/XRSTOR
    if (edx & BIT06) StrCat(features, L" PAE");               // Physical Address Extensions
    if (xedx & BIT26) StrCat(features, L" 1GB_Pages");
    if (edx & BIT16) StrCat(features, L" PAT");               // Page Attribute Table
    if (edx & BIT31) StrCat(features, L" PBE");               // Pending Break Enable
    if (ecx & BIT17) StrCat(features, L" PCID");             // Process Context Identifiers
    if (ecx & BIT01) StrCat(features, L" PCLMULQDQ");   // Support Carry-Less Multiplication of Quadword instruction
    if (ecx & BIT15) StrCat(features, L" PDCM");             // Performance Capabilities
    if (edx & BIT13) StrCat(features, L" PGE");               // Page Global Enable
    if (ecx & BIT23) StrCat(features, L" POPCNT");         // Return the Count of Number of Bits Set to 1 instruction
    if (xecx & BIT08) StrCat(features, L" PREFETCHW");
    if (edx & BIT03) StrCat(features, L" PSE");               // Page Size Extensions (4MB memory pages)
    if (edx & BIT17) StrCat(features, L" PSE-36");         // 36-Bit (> 4MB) Page Size Extension
    if (edx & BIT18) StrCat(features, L" PSN");               // Processor Serial Number Support
    if (ecx & BIT30) StrCat(features, L" RDRAND");         // Read Random Number from hardware random number generator instruction Support
    if (xedx & BIT27) StrCat(features, L" RDTSCP/IA32_TSC_AUX");
    if (ecx & BIT11) StrCat(features, L" SDBG");             // Silicon Debug Support
    if (edx & BIT11) StrCat(features, L" SEP");               // SYSENTER/SYSEXIT Support
    if (ecx & BIT06) StrCat(features, L" SMX");
    if (edx & BIT27) StrCat(features, L" SS");
    if (edx & BIT25) StrCat(features, L" SSE");
    if (edx & BIT26) StrCat(features, L" SSE2");
    if (ecx & BIT00) StrCat(features, L" SSE3");
    if (ecx & BIT19) StrCat(features, L" SSE4_1");
    if (ecx & BIT20) StrCat(features, L" SSE4_2");
    if (ecx & BIT09) StrCat(features, L" SSSE3");                             // Supplemental SSE-3
    if (xedx & BIT11) StrCat(features, L" SYSCALL/SYSRET");
    if (edx & BIT04) StrCat(features, L" TSC");                                 // Time Stamp Counter
    if (ecx & BIT24) StrCat(features, L" TSC-Deadline");               // TSC Deadline Timer
    if (edx & BIT29) StrCat(features, L" TM");                                   // Automatic Clock Control (Thermal Monitor)
    if (ecx & BIT08) StrCat(features, L" TM2");                                 // Thermal Monitor 2
    if (edx & BIT01) StrCat(features, L" VME");                                 // Virtual 8086 Mode Enhancements
    if (ecx & BIT05) StrCat(features, L" VMX");                                 // Hardware Virtualization Support
    if (ecx & BIT21) StrCat(features, L" x2APIC");                           // x2APIC Support
    if (ecx & BIT26) StrCat(features, L" XSAVE");                             // Save Processor Extended States
    if (ecx & BIT14) StrCat(features, L" xTPR_Update_Control"); // Change IA32_MISC_ENABLE Support

    Print(L"    Features: ");

    ZeroMem (buf, 80);

    while(*f){
        buf[Col++] = *f++;
        if(Col > Width){
            while(Col >= 0 && buf[Col] != L' '){
                buf[Col] = CHAR_NULL;
                f--; Col--;
            }
            if(FirstRow){
                Print(L"%s\n", buf);
                FirstRow = FALSE;
            } else {
                Print(L"              %s\n", buf);
            }
            ZeroMem(buf, 80);
            Col = 0;
        }
    }
    if(Col){
        if(FirstRow){
            Print(L"%s\n", buf);
        }
        else{
            Print(L"              %s\n", buf);
        }
    }
}

EFI_STATUS
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    PrintProcessorSignature ();
    PrintProcessorBrandString ();
    PrintProcessorVersionInfo ();
    PrintProcessorFeaturesInfo ();
    return EFI_SUCCESS;
}