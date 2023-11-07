#pragma once

// Image
#include "media/image/color.h"
#include "media/image/image.h"

// Audio
#include "media/audio/audio.h"

// Video
#include "media/video/video_reader.h"
#include "media/video/video_writer.h"


namespace kl {
    class HRChecker {};

    class HRPackage {
    public:
        const HRESULT result = {};
        const std::source_location location = {};

        HRPackage(const HRESULT result, std::source_location location = std::source_location::current())
            : result(result), location(location)
        {}
    };

    inline void operator>>(const HRPackage& package, const HRChecker& _)
    {
        if (FAILED(package.result)) {
            const std::string message = std::format("{}:{}:{}: HRESULT failed with error code: {}",
                package.location.file_name(), package.location.line(), package.location.column(),
                package.result);
            throw std::runtime_error(message);
        }
    }

    inline const HRChecker hr_checker = {};
}
