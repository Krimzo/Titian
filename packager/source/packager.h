#pragma once

#include "serial/binary_serializer.h"
#include "serial/text_serializer.h"


namespace titian
{
int packager_entry( int argc, str* argv, bool pack );

StringSet list_files( StringRef const& input );
bool create_package( StringRef const& input, StringRef const& output_file );
bool open_package( StringRef const& input_file, StringRef const& output_dir );
}
