#pragma once

#include "math/math.h"


namespace kl::console
{
void set_enabled( bool state );
void clear();

void set_cursor_enabled( bool state );
void move_cursor( Int2 position );

int width();
void set_width( int width );

int height();
void set_height( int height );

Int2 size();
void set_size( Int2 size );

void set_title( std::string_view const& text );
void set_title( std::wstring_view const& text );
void set_font( Int2 size, std::wstring_view const& font_name = L"Consolas" );

char read();
void wait( char to_wait_for, bool echo = false );
char wait_for_any( bool echo = false );

void dump( std::string_view const& data, Int2 location = {} );
void progress_bar( std::string_view const& message, int output_y, float percentage );
}

namespace kl
{
struct ConsoleInit
{
    static const HANDLE _init;
};
}
