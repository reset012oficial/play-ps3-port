#ifndef PS2OS_H
#define PS2OS_H

// Minimal PS2OS stub for PS3 port
// This replaces the Play! PS2OS.h which includes ELF.h -> PtrStream.h (missing)

namespace PS2OS
{
    // Boot methods
    enum BootMethod
    {
        BOOT_NONE = 0,
        BOOT_CDROM = 1,
        BOOT_ELF = 2,
        BOOT_FAST = 3
    };

    // Initialize the PS2 OS layer
    inline void Init()
    {
    }

    // Shutdown the PS2 OS layer
    inline void Shutdown()
    {
    }

    // Boot from CDROM
    inline void BootFromCDROM()
    {
    }

    // Boot from ELF file
    inline void BootFromELF(const char* path)
    {
        (void)path;
    }

    // Fast boot (skip BIOS)
    inline void FastBoot()
    {
    }

    // Get boot method
    inline BootMethod GetBootMethod()
    {
        return BOOT_FAST;
    }

    // Set boot method
    inline void SetBootMethod(BootMethod method)
    {
        (void)method;
    }

    // Check if running
    inline bool IsRunning()
    {
        return true;
    }

    // Reset the OS
    inline void Reset()
    {
    }

    // Update OS state
    inline void Update()
    {
    }
}

#endif // PS2OS_H