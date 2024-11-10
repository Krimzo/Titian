#include "titian.h"


void titian::gui_colored_text( StringRef const& message, Float4 const& color )
{
    im::TextColored( reinterpret_cast<ImVec4 const&>(color), message.data() );
}

titian::Pair<ImVec2, ImVec2> titian::gui_window_rect()
{
    ImVec2 content_region = im::GetContentRegionAvail();
    ImVec2 win_content_min = im::GetWindowPos() + im::GetWindowContentRegionMin();
    ImVec2 win_content_max = win_content_min + content_region;
    return { win_content_min, win_content_max };
}

float titian::gui_calculate_item_with( StringRef const& label )
{
    return im::CalcTextSize( label.data() ).x;
}

void titian::gui_align_horizontally( float width, float alignment )
{
    float available = im::GetContentRegionAvail().x;
    float offset = (available - width) * alignment;
    if ( offset > 0.0f )
        im::SetCursorPosX( im::GetCursorPosX() + offset );
}

titian::String titian::gui_input_continuous( String const& id )
{
    auto& buffer = _INPUT_CONTINUOUS_DATA[id];
    im::InputText( id.data(), &buffer );
    return buffer;
}

titian::Opt<titian::String> titian::gui_input_waited( String const& id, StringRef const& to_copy )
{
    auto& buffer = _INPUT_WAITED_DATA[id];
    buffer = to_copy;
    if ( im::InputText( id.data(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue ) )
    {
        String result = buffer;
        buffer.clear();
        return { result };
    }
    return {};
}
