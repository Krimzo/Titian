#pragma once

// Image
#include "media/image/color.h"
#include "media/image/image.h"

// Audio
#include "media/audio/audio.h"
#include "media/audio/audio_device.h"

// Video
#include "media/video/video_reader.h"
#include "media/video/video_writer.h"


namespace kl_ignored {
    [[maybe_unused]] inline static const int MEDIA_DONT_CARE = []()
    {
        constexpr DWORD init_args = (COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
        CoInitializeEx(nullptr, init_args) >> kl::verify_result;
        MFStartup(MF_VERSION) >> kl::verify_result;
        return 0;
    }();
}
