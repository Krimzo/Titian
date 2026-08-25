#include "titian.h"


void titian::ui_separator()
{
    im::Separator();
}

void titian::ui_same_line()
{
    im::SameLine();
}

void titian::ui_set_next_width( float width )
{
    im::SetNextItemWidth( width );
}

titian::Float2 titian::ui_cursor_pos()
{
    return { im::GetCursorPosX(), im::GetCursorPosY() };
}

void titian::ui_set_cursor_pos( Float2 pos )
{
    im::SetCursorPos( { pos.x, pos.y } );
}

void titian::ui_window( StringRef const& name, Func<void()> const& func )
{
    if ( im::Begin( name.data(), nullptr, NULL ) )
        func();
    im::End();
}

void titian::ui_button( StringRef const& name, Func<void()> const& func )
{
    if ( im::Button( name.data() ) )
        func();
}

void titian::ui_bool( StringRef const& name, bool value, Func<void( bool )> const& func )
{
    if ( im::Checkbox( name.data(), &value ) )
        func( value );
}

void titian::ui_int( StringRef const& name, int value, Func<void( int )> const& func )
{
    if ( im::DragInt( name.data(), &value ) )
        func( value );
}

void titian::ui_int2( StringRef const& name, Int2 value, Func<void( Int2 )> const& func )
{
    if ( im::DragInt2( name.data(), &value.x ) )
        func( value );
}

void titian::ui_float( StringRef const& name, float value, Func<void( float )> const& func )
{
    if ( im::DragFloat( name.data(), &value ) )
        func( value );
}

void titian::ui_float2( StringRef const& name, Float2 value, Func<void( Float2 )> const& func )
{
    if ( im::DragFloat2( name.data(), &value.x ) )
        func( value );
}

void titian::ui_float3( StringRef const& name, Float3 value, Func<void( Float3 )> const& func )
{
    if ( im::DragFloat3( name.data(), &value.x ) )
        func( value );
}

void titian::ui_float4( StringRef const& name, Float4 value, Func<void( Float4 )> const& func )
{
    if ( im::DragFloat4( name.data(), &value.x ) )
        func( value );
}

void titian::ui_color( StringRef const& name, RGB value, Func<void( RGB )> const& func )
{
    Float4 temp_val = value;
    if ( im::ColorEdit4( name.data(), &temp_val.x ) )
        func( temp_val );
}

void titian::ui_string( StringRef const& name, String value, Func<void( String )> const& func )
{
    if ( im::InputText( name.data(), &value ) )
        func( value );
}

void titian::ui_text( StringRef const& data )
{
    im::Text( data.data() );
}

void titian::ui_colored_text( Float4 const& color, StringRef const& data )
{
    im::TextColored( ImVec4{ color.x, color.y, color.z, color.w }, data.data() );
}
