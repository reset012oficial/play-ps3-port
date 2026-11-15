#ifndef SIGNAL_H
#define SIGNAL_H

// Minimal signal handler stub for PS2OS.h compatibility
// PS2OS.h includes "signal/Signal.h" which doesn't exist in Play! source

namespace Signal
{
    // Signal handling functions
    typedef void (*Handler)(int);

    inline void Install(int signal, Handler handler)
    {
        (void)signal;
        (void)handler;
    }

    inline void Uninstall(int signal)
    {
        (void)signal;
    }

    inline void Raise(int signal)
    {
        (void)signal;
    }

    inline bool IsPending(int signal)
    {
        (void)signal;
        return false;
    }

    inline void Clear(int signal)
    {
        (void)signal;
    }
}

#endif // SIGNAL_H