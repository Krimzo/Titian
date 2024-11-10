#pragma once

#include "standard.h"


namespace titian
{
void ui_separator();
void ui_same_line();
void ui_set_next_width( float width );

Float2 ui_cursor_pos();
void ui_set_cursor_pos( Float2 pos );

void ui_window( StringRef const& name, Func<void()> const& func );
void ui_button( StringRef const& name, Func<void()> const& func );

void ui_bool( StringRef const& name, bool value, Func<void( bool )> const& func );
void ui_int( StringRef const& name, int value, Func<void( int )> const& func );
void ui_int2( StringRef const& name, Int2 value, Func<void( Int2 )> const& func );
void ui_float( StringRef const& name, float value, Func<void( float )> const& func );
void ui_float2( StringRef const& name, Float2 value, Func<void( Float2 )> const& func );
void ui_float3( StringRef const& name, Float3 value, Func<void( Float3 )> const& func );
void ui_float4( StringRef const& name, Float4 value, Func<void( Float4 )> const& func );
void ui_color( StringRef const& name, RGB value, Func<void( RGB )> const& func );
void ui_string( StringRef const& name, String value, Func<void( String )> const& func );

void ui_text( StringRef const& data );
void ui_colored_text( Float4 const& color, StringRef const& data );
}
