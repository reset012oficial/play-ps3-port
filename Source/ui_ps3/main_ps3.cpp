#define _FILE_OFFSET_BITS 64

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cerrno>
#include <cstdarg>
#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <limits>
#include <filesystem>
#include <exception>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <ppu-types.h>
#include <io/pad.h>
#include <sys/process.h>
#include <sysutil/sysutil.h>

#include "AppConfig.h"
#include "PS2VM.h"
#include "PS2VM_Preferences.h"
#include "gs/GSH_Null.h"
#include "SH_Null.h"
#include "PH_Generic.h"
#include "ControllerInfo.h"
#include "ee/PS2OS.h"

namespace fs = std::filesystem;

namespace PlayPS3
{
static constexpr uint32_t ISO_SECTOR_SIZE = 2048;
static bool g_systemExitRequested = false;

class Logger
{
public:
    static void Log(const char* format, ...)
    {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        std::printf("%s", buffer);
        FILE* logFile = std::fopen("/dev_hdd0/play_ps3_log.txt", "a");
        if (logFile)
        {
            std::fputs(buffer, logFile);
            std::fclose(logFile);
        }
    }
    static void Clear()
    {
        FILE* logFile = std::fopen("/dev_hdd0/play_ps3_log.txt", "w");
        if (logFile)
        {
            std::fputs("=== INICIO DO LOG PLAY! PS3 v2.3.1 ===\n", logFile);
            std::fclose(logFile);
        }
    }
};

static void SysutilCallback(uint64_t status, uint64_t param, void* userdata)
{
    (void)param; (void)userdata;
    if (status == SYSUTIL_EXIT_GAME)
    {
        Logger::Log("[PS3] Pedido de saida via botao PS recebido.\n");
        g_systemExitRequested = true;
    }
}

class Platform
{
public:
    static bool s_padInitialized;
    static bool IsIsoFile(const char* name)
    {
        if (!name) return false;
        size_t len = std::strlen(name);
        if (len < 4) return false;
        const char* ext = name + len - 4;
        return (std::strcmp(ext, ".iso") == 0 || std::strcmp(ext, ".ISO") == 0);
    }
    static FILE* OpenIsoFile(const std::string& path, uint64_t* outSize)
    {
        if (path.empty() || outSize == nullptr) return nullptr;
        *outSize = 0;
        FILE* file = std::fopen(path.c_str(), "rb");
        if (!file) return nullptr;
        if (::fseeko(file, 0, SEEK_END) != 0) { std::fclose(file); return nullptr; }
        off_t fileSize = ::ftello(file);
        if (fileSize < 0 || ::fseeko(file, 0, SEEK_SET) != 0) { std::fclose(file); return nullptr; }
        *outSize = static_cast<uint64_t>(fileSize);
        if (*outSize < ISO_SECTOR_SIZE) { std::fclose(file); *outSize = 0; return nullptr; }
        std::setvbuf(file, nullptr, _IOFBF, 128 * 1024);
        return file;
    }
    static bool ReadIso(FILE* file, uint64_t offset, void* buffer, size_t size, uint64_t isoSize)
    {
        if (!file || !buffer || size == 0) return false;
        if (offset > isoSize || static_cast<uint64_t>(size) > isoSize - offset) return false;
        if (::fseeko(file, static_cast<off_t>(offset), SEEK_SET) != 0) return false;
        size_t totalRead = 0;
        while (totalRead < size)
        {
            size_t amount = std::fread(static_cast<unsigned char*>(buffer) + totalRead, 1, size - totalRead, file);
            if (amount == 0) return false;
            totalRead += amount;
        }
        return true;
    }
    static bool ReadIsoSector(FILE* file, uint64_t sector, void* buffer, uint64_t isoSize)
    {
        if (sector > UINT64_MAX / ISO_SECTOR_SIZE) return false;
        return ReadIso(file, sector * ISO_SECTOR_SIZE, buffer, ISO_SECTOR_SIZE, isoSize);
    }
    static void CloseIsoFile(FILE* file) { if (file) std::fclose(file); }
    static void EnsureDirectoriesExist()
    {
        mkdir("/dev_hdd0/PS2ISO", 0777);
        mkdir("/dev_hdd0/game/PLAYPS300", 0777);
        mkdir("/dev_hdd0/game/PLAYPS300/USRDIR", 0777);
        mkdir("/dev_hdd0/game/PLAYPS300/USRDIR/bios", 0777);
        mkdir("/dev_hdd0/game/PLAYPS300/USRDIR/saves", 0777);
    }
    static void CheckBiosPresence()
    {
        const char* biosPaths[] = { "/dev_hdd0/game/PLAYPS300/USRDIR/bios", "/dev_hdd0/PS2ISO/bios" };
        bool found = false;
        for (const auto& dirPath : biosPaths)
        {
            DIR* dir = opendir(dirPath);
            if (!dir) continue;
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr)
            {
                if (entry->d_name[0] != '.') found = true;
            }
            closedir(dir);
        }
        if (!found) Logger::Log("[PS3] AVISO: Nenhum ficheiro de BIOS encontrado.\n");
    }
    static std::string FindFirstIsoGame()
    {
        const char* searchPaths[] = { "/dev_hdd0/PS2ISO", "/dev_usb000/PS2ISO", "/dev_usb001/PS2ISO", "/dev_usb002/PS2ISO", "/dev_usb003/PS2ISO" };
        for (const auto& basePath : searchPaths)
        {
            DIR* dir = opendir(basePath);
            if (!dir) continue;
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr)
            {
                if (IsIsoFile(entry->d_name))
                {
                    std::string fullPath = std::string(basePath) + "/" + entry->d_name;
                    struct stat st;
                    if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode))
                    {
                        closedir(dir);
                        return fullPath;
                    }
                }
            }
            closedir(dir);
        }
        return "";
    }
};

bool Platform::s_padInitialized = false;

class CPH_Ps3 : public CPH_Generic
{
public:
    static CPadHandler::FactoryFunction GetFactoryFunction() { return []() { return new CPH_Ps3(); }; }
    void Update(uint8* data) override
    {
        std::memset(&m_currentPad, 0, sizeof(m_currentPad));
        bool exitRequested = false;
        padInfo padinfo;
        ioPadGetInfo(&padinfo);
        int activePort = -1;
        for (int i = 0; i < 7; i++) { if (padinfo.status[i]) { activePort = i; break; } }
        if (activePort != -1 && ioPadGetData(activePort, &m_currentPad) == 0 && m_currentPad.len > 0)
        {
            using B = PS2::CControllerInfo;
            SetButtonState(B::DPAD_UP, m_currentPad.BTN_UP != 0);
            SetButtonState(B::DPAD_DOWN, m_currentPad.BTN_DOWN != 0);
            SetButtonState(B::DPAD_LEFT, m_currentPad.BTN_LEFT != 0);
            SetButtonState(B::DPAD_RIGHT, m_currentPad.BTN_RIGHT != 0);
            SetButtonState(B::SELECT, m_currentPad.BTN_SELECT != 0);
            SetButtonState(B::START, m_currentPad.BTN_START != 0);
            SetButtonState(B::SQUARE, m_currentPad.BTN_SQUARE != 0);
            SetButtonState(B::TRIANGLE, m_currentPad.BTN_TRIANGLE != 0);
            SetButtonState(B::CIRCLE, m_currentPad.BTN_CIRCLE != 0);
            SetButtonState(B::CROSS, m_currentPad.BTN_CROSS != 0);
            SetButtonState(B::L1, m_currentPad.BTN_L1 != 0);
            SetButtonState(B::L2, m_currentPad.BTN_L2 != 0);
            SetButtonState(B::L3, m_currentPad.BTN_L3 != 0);
            SetButtonState(B::R1, m_currentPad.BTN_R1 != 0);
            SetButtonState(B::R2, m_currentPad.BTN_R2 != 0);
            SetButtonState(B::R3, m_currentPad.BTN_R3 != 0);
            SetAxisState(B::ANALOG_LEFT_X, ToAxis(m_currentPad.ANA_L_H));
            SetAxisState(B::ANALOG_LEFT_Y, ToAxis(m_currentPad.ANA_L_V));
            SetAxisState(B::ANALOG_RIGHT_X, ToAxis(m_currentPad.ANA_R_H));
            SetAxisState(B::ANALOG_RIGHT_Y, ToAxis(m_currentPad.ANA_R_V));
            exitRequested = m_currentPad.BTN_START != 0 && m_currentPad.BTN_SELECT != 0 && m_currentPad.BTN_L1 != 0 && m_currentPad.BTN_R1 != 0;
        }
        m_exitRequested.store(exitRequested, std::memory_order_release);
        CPH_Generic::Update(data);
    }
    bool CheckExitCombination() const { return m_exitRequested.load(std::memory_order_acquire); }
private:
    padData m_currentPad = {};
    std::atomic<bool> m_exitRequested{false};
    static float ToAxis(unsigned char value) { return (static_cast<float>(value) / 127.5f) - 1.0f; }
};
}

static int RunGame(const std::string& isoPath)
{
    if (isoPath.empty()) return 2;
    try
    {
        CAppConfig::GetInstance().RegisterPreferenceInteger(PREF_AUDIO_SPUBLOCKCOUNT, 22);
        CAppConfig::GetInstance().SetPreferencePath(PREF_PS2_MCD0_PATH, fs::path("/dev_hdd0/game/PLAYPS300/USRDIR/saves/mcd0.ps2"));
        CAppConfig::GetInstance().SetPreferencePath(PREF_PS2_MCD1_PATH, fs::path("/dev_hdd0/game/PLAYPS300/USRDIR/saves/mcd1.ps2"));
        auto vm = std::make_unique<CPS2VM>();
        if (!vm) return 3;
        vm->Initialize();
        vm->CreateGSHandler(CGSH_Null::GetFactoryFunction());
        vm->CreatePadHandler(PlayPS3::CPH_Ps3::GetFactoryFunction());
        vm->CreateSoundHandler(CSH_Null::GetFactoryFunction());
        CAppConfig::GetInstance().SetPreferencePath(PREF_PS2_CDROM0_PATH, fs::path(isoPath));
        CAppConfig::GetInstance().Save();
        vm->CDROM0_SyncPath();
        if (!vm->m_cdrom0) { vm->DestroyPadHandler(); vm->DestroyGSHandler(); vm->DestroySoundHandler(); vm->Destroy(); return 4; }
        vm->Reset();
        vm->m_ee->m_os->BootFromCDROM();
        vm->Resume();
        bool running = true;
        while (running && !PlayPS3::g_systemExitRequested)
        {
            sysUtilCheckCallback();
            auto* pad = static_cast<PlayPS3::CPH_Ps3*>(vm->GetPadHandler());
            if (pad && pad->CheckExitCombination()) running = false;
            usleep(16000);
        }
        vm->PauseAsync();
        usleep(200000);
        vm->DestroyPadHandler();
        vm->DestroyGSHandler();
        vm->DestroySoundHandler();
        vm->Destroy();
    }
    catch (...) { return 5; }
    return 0;
}

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;
    PlayPS3::Platform::EnsureDirectoriesExist();
    PlayPS3::Logger::Clear();
    PlayPS3::Platform::CheckBiosPresence();
    sysUtilRegisterCallback(0, PlayPS3::SysutilCallback, nullptr);
    if (ioPadInit(7) != 0) { sysProcessExit(1); return 1; }
    PlayPS3::Platform::s_padInitialized = true;
    const std::string gamePath = PlayPS3::Platform::FindFirstIsoGame();
    int result = gamePath.empty() ? 2 : RunGame(gamePath);
    if (PlayPS3::Platform::s_padInitialized) { ioPadEnd(); PlayPS3::Platform::s_padInitialized = false; }
    sysUtilUnregisterCallback(0);
    sysProcessExit(result);
    return result;
}
