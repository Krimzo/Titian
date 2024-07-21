#include "fuze.h"


fuze::GUISectionVideoTimeline::GUISectionVideoTimeline(const LayerPackage& package)
	: GUISection("GUISectionVideoTimeline", package)
{}

void fuze::GUISectionVideoTimeline::render_gui()
{
	kl::Window* window = &app_layer->window;
	const int current_scroll = window->mouse.scroll();
	const int scroll = current_scroll - m_last_scroll;

	if (window->keyboard.shift) {
		vertical_view -= scroll;
		vertical_view = std::clamp(vertical_view, 3, (int) video_layer->tracks.size());
	}
	else if (window->keyboard.ctrl) {
		if (scroll > 0) {
			horizontal_view *= 1.5f;
		}
		else if (scroll < 0) {
			horizontal_view /= 1.5f;
		}
		horizontal_view = std::max(horizontal_view, 1.0f);
	}
	else if (window->keyboard.alt) {
		horizontal_offset += scroll * horizontal_view * 0.05f;
		horizontal_offset = std::max(horizontal_offset, 0.0f);
	}
	else if (scroll != 0) {
		vertical_offset -= scroll;
		vertical_offset = std::clamp(vertical_offset, 0, (int) video_layer->tracks.size() - 1);
	}

	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (im::Begin("Timeline", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		const float available_height = ImGui::GetContentRegionAvail().y;

		im::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 4.0f));

        if (im::BeginTable("##TracksTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthFixed);
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthStretch);

			const float header_height = available_height * 0.1f;
			im::TableNextRow(ImGuiTableRowFlags_None, header_height);
			{
				im::TableSetColumnIndex(0);
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(45, 45, 45));
			}
			{
				im::TableSetColumnIndex(1);
			}

			const float row_height = (available_height - header_height) / vertical_view;
			for (int i = vertical_offset; i < vertical_offset + vertical_view && i < (int) video_layer->tracks.size(); i++) {
				const auto& track = video_layer->tracks[i];

				im::TableNextRow(ImGuiTableRowFlags_None, row_height);

				{
					im::TableSetColumnIndex(0);
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(45, 45, 45));
					
					const String text = kl::format(i + 1, ". ", track->name);
					const ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
					
					const ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
					const float vertical_off = (row_height - text_size.y) * 0.5f;

					im::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y + vertical_off));
					im::Text(text.c_str());
				}

				{
					im::TableSetColumnIndex(1);
				}
			}

            im::EndTable();
        }

		im::PopStyleVar(1);
	}
	im::End();

	im::PopStyleVar(1);

	m_last_scroll = current_scroll;
}
