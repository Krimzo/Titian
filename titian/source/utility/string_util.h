#pragma once

#include "standard.h"


namespace titian
{
String get_default_script();
String get_default_material_shader();
String get_default_camera_shader();

String format_byte_size( uint64_t byte_size );
String format_path( fs::path const& path );

String ltrim( StringRef const& source, str t = " \t\n\r\f\v" );
String rtrim( StringRef const& source, str t = " \t\n\r\f\v" );
String trim( StringRef const& source, str t = " \t\n\r\f\v" );

bool str_find( StringRef const& haystack, StringRef const& needle );
}
