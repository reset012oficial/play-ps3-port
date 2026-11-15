#ifndef PS2VM_H
#define PS2VM_H

#include <memory>
#include <functional>

class CGSHandler;
class CPadHandler;
class CSoundHandler;
class CAppConfig;

class CPS2VM
{
public:
    struct EE;
    struct OS;

    CPS2VM() = default;
    virtual ~CPS2VM() = default;

    virtual bool Initialize()
    {
        return true;
    }

    virtual void CreateGSHandler(std::function<CGSHandler*()> factory)
    {
        (void)factory;
    }

    virtual void CreatePadHandler(std::function<CPadHandler*()> factory)
    {
        (void)factory;
    }

    virtual void CreateSoundHandler(std::function<CSoundHandler*()> factory)
    {
        (void)factory;
    }

    virtual void CDROM0_SyncPath()
    {
        m_cdrom0 = true;
    }

    virtual void Reset()
    {
    }

    virtual void Resume()
    {
    }

    virtual void PauseAsync()
    {
    }

    virtual void DestroyPadHandler()
    {
    }

    virtual void DestroyGSHandler()
    {
    }

    virtual void DestroySoundHandler()
    {
    }

    virtual void Destroy()
    {
    }

    virtual CPadHandler* GetPadHandler()
    {
        return nullptr;
    }

    bool m_cdrom0 = false;

    EE* m_ee = nullptr;

    struct EE
    {
        OS* m_os = nullptr;
    };

    struct OS
    {
        virtual void BootFromCDROM()
        {
        }
        virtual ~OS() = default;
    };
};

#endif // PS2VM_H