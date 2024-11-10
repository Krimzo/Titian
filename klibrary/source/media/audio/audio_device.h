#pragma once

#include "media/audio/audio.h"


namespace kl
{
struct AudioDevice
{
    UINT id = 0;

    AudioDevice( UINT id = 0 );

    bool record_audio( Audio& output, std::function<bool()> const& should_record = [] { return true; } ) const;
    bool play_audio( Audio const& audio, std::function<bool()> const& should_play = [] { return true; } ) const;

    static UINT input_devices_count();
    static UINT output_devices_count();
};
}
