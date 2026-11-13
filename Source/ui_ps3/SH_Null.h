#ifndef SH_NULL_H
#define SH_NULL_H

#include "SoundHandler.h"

class CSH_Null : public CSoundHandler
{
public:
    static CSoundHandler* GetFactoryFunction()
    {
        return new CSH_Null();
    }

    CSH_Null() = default;
    virtual ~CSH_Null() = default;

    virtual void Init() override {}
    virtual void Destroy() override {}
    virtual void Reset() override {}
    virtual void Update() override {}
    virtual void SetVolume(int volume) override { (void)volume; }
    virtual int GetVolume() const override { return 0; }
    virtual void SetMute(bool mute) override { (void)mute; }
    virtual bool GetMute() const override { return false; }
};

#endif // SH_NULL_H