#include "titian.h"


void titian::gui_colored_text(const StringView& message, const Float4& color)
{
    im::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
}

titian::Pair<ImVec2, ImVec2> titian::gui_window_rect()
{
    const ImVec2 content_region = im::GetContentRegionAvail();
    const ImVec2 win_content_min = im::GetWindowPos() + im::GetWindowContentRegionMin();
    const ImVec2 win_content_max = win_content_min + content_region;
    return { win_content_min, win_content_max };
}

float titian::gui_calculate_item_with(const StringView& label)
{
    return im::CalcTextSize(label.data()).x;
}

void titian::gui_align_horizontally(const float width, const float alignment)
{
    const float available = im::GetContentRegionAvail().x;
    const float offset = (available - width) * alignment;
    if (offset > 0.0f) {
        im::SetCursorPosX(im::GetCursorPosX() + offset);
    }
}

titian::String titian::gui_input_continuous(const String& id)
{
    auto& buffer = _INPUT_CONTINUOUS_DATA[id];
    im::InputText(id.data(), &buffer);
    return buffer;
}

titian::Optional<titian::String> titian::gui_input_waited(const String& id, const StringView& to_copy)
{
    auto& buffer = _INPUT_WAITED_DATA[id];
    buffer = to_copy;
    if (im::InputText(id.data(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
        const String result = buffer;
        buffer.clear();
        return { result };
    }
    return {};
}
