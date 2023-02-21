#pragma once

// Image
#include "media/image/color.h"
#include "media/image/image.h"

// Audio
#include "media/audio/audio.h"

// Video
#include "media/video/video_reader.h"
#include "media/video/video_writer.h"


namespace kl::media_utility {
    void fail_check_(HRESULT hr, const std::string& error_message);
    bool succeeded_(HRESULT hr);
}
