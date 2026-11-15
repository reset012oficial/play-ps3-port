#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

class CSoundHandler
{
public:
    virtual ~CSoundHandler() = default;

    virtual void Init() {}
    virtual void Destroy() {}
    virtual void Reset() {}
    virtual void Update() {}
    virtual void SetVolume(int volume) { (void)volume; }
    virtual int GetVolume() const { return 0; }
    virtual void SetMute(bool mute) { (void)mute; }
    virtual bool GetMute() const { return false; }
};

#endif // SOUNDHANDLER_H