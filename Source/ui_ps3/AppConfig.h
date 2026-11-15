#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "Singleton.h"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class CAppConfig : public CSingleton<CAppConfig>
{
    friend class CSingleton<CAppConfig>;

    CAppConfig() = default;

public:
    virtual ~CAppConfig() = default;

    // Preference constants
    static const char* PREF_AUDIO_SPUBLOCKCOUNT;
    static const char* PREF_PS2_MCD0_PATH;
    static const char* PREF_PS2_MCD1_PATH;
    static const char* PREF_PS2_CDROM0_PATH;

    void RegisterPreferenceInteger(const char* name, int value)
    {
        (void)name; (void)value;
    }

    void SetPreferencePath(const char* name, const fs::path& path)
    {
        (void)name; (void)path;
    }

    void Save()
    {
    }
};

#endif // APPCONFIG_H