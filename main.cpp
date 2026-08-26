#include "titian.h"
#include "fuze.h"
#include "sandbox.h"

using namespace titian;

inline constexpr str EXE_PLAY = "play";
inline constexpr str EXE_EDIT = "edit";
inline constexpr str EXE_SANDBOX = "sandbox";
inline constexpr str EXE_UNPACK = "unpack";
inline constexpr str EXE_PACK = "pack";
inline constexpr str EXE_VIDEO = "video";

int main(int argc, str* argv)
{
    if (!kl::IS_DEBUG && fs::path(argv[0]).is_absolute())
        kl::console::set_enabled(false);

    String exe_type{EXE_PLAY};
    if (argc >= 2)
    {
        exe_type = argv[1];
        Logger::log("Using argument exe type: ", exe_type);
    }
    else if (_conf_data.contains(CONF_EXE_TYPE))
    {
        exe_type = _conf_data.at(CONF_EXE_TYPE)->get_string().value_or(exe_type);
        Logger::log("Using .json exe type: ", exe_type);
    }
    else
    {
        Logger::log("Exe type defaulting to ", exe_type);
    }

    if (exe_type == EXE_PLAY)
        return titian_entry(argc, argv, EntryType::GAME);

    if (exe_type == EXE_EDIT)
        return titian_entry(argc, argv, EntryType::GAME_EDITOR);

    if (exe_type == EXE_SANDBOX)
        return sandbox_entry(argc, argv);

    if (exe_type == EXE_UNPACK)
        return packager_entry(argc, argv, false);

    if (exe_type == EXE_PACK)
        return packager_entry(argc, argv, true);

    if (exe_type == EXE_VIDEO)
        return fuze_entry(argc, argv);

    Logger::log("Invalid exe type: ", exe_type);
    Logger::log("Exe Types: [", EXE_PLAY, ", ", EXE_EDIT, ", ", EXE_SANDBOX, ", ", EXE_UNPACK, ", ", EXE_PACK, ", ",
                EXE_VIDEO, "]");
    return 1;
}

static struct Ignored
{
    ~Ignored()
    {
        kl::console::set_enabled(true);
    }
} _ignored{};
