#include "fuze.h"


titian::GUISectionVideoTimeline::GUISectionVideoTimeline()
	: GUISection("GUISectionVideoTimeline")
{}

void titian::GUISectionVideoTimeline::render_gui()
{
	AppLayer* app_layer = Layers::get<AppLayer>();
	VideoLayer* video_layer = Layers::get<VideoLayer>();

	kl::Window* window = &app_layer->window;
	const int current_scroll = window->mouse.scroll();
	const int scroll = current_scroll - m_last_scroll;

	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (im::Begin("Timeline", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		const float available_height = im::GetContentRegionAvail().y;
		ImDrawList* draw_list = im::GetWindowDrawList();

		if (im::IsWindowFocused()) {
			if (im::IsKeyPressed(ImGuiKey_Space)) {
				if (!video_layer->playing) {
					video_layer->playing = true;
					video_layer->last_time = video_layer->current_time;
					Layers::get<AppLayer>()->timer.restart();
				}
				else {
					video_layer->playing = false;
					video_layer->current_time = video_layer->last_time;
					Layers::get<AppLayer>()->timer.stop();
				}
			}
			if (!video_layer->playing && im::IsKeyPressed(ImGuiKey_Home)) {
				const float start_time = video_layer->start_time();
				video_layer->current_time = (video_layer->current_time > start_time) ? start_time : 0.0f;
			}
			if (!video_layer->playing && im::IsKeyPressed(ImGuiKey_End)) {
				video_layer->current_time = video_layer->end_time();
			}
		}

		if (im::IsWindowHovered()) {
			if (im::IsKeyDown(ImGuiKey_LeftAlt)) {
				vertical_view -= scroll;
				vertical_view = std::max(vertical_view, 3);
			}
			else if (im::IsKeyDown(ImGuiKey_LeftCtrl)) {
				if (scroll < 0) {
					horizontal_view *= 1.02f;
				}
				else if (scroll > 0) {
					horizontal_view /= 1.02f;
				}
				horizontal_view = std::max(horizontal_view, 1.0f);
			}
			else if (im::IsKeyDown(ImGuiKey_LeftShift)) {
				horizontal_offset += scroll * horizontal_view * 0.02f;
				horizontal_offset = std::max(horizontal_offset, 0.0f);
			}
			else if (scroll != 0) {
				vertical_offset -= scroll;
				vertical_offset = std::clamp(vertical_offset, 0, (int) video_layer->tracks.size() - 1);
			}
		}

		const ImVec2 cell_padding = ImVec2(4.0f, 4.0f);
		im::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

        if (im::BeginTable("##TracksTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthFixed);
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthStretch);

			ImVec2 total_min{};
			ImVec2 total_max{};

			/* HEADER */
			const float header_height = available_height * 0.1f;
			im::TableNextRow(ImGuiTableRowFlags_None, header_height);
			{
				im::TableSetColumnIndex(0);
				im::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(45, 45, 45));

				const String text = kl::format(std::fixed, video_layer->current_time);
				const ImVec2 text_size = im::CalcTextSize(text.c_str());

				const ImVec2 cursor_pos = im::GetCursorScreenPos();
				const float vertical_off = (header_height - text_size.y) * 0.5f;

				im::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y - cell_padding.y + vertical_off));
				im::Text(text.c_str());
			}
			{
				im::TableSetColumnIndex(1);

				const ImVec2 col_min = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Min;
				const ImVec2 col_max = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Max;
				const float col_width = col_max.x - col_min.x;
				const float col_middle = (col_min.y + col_max.y) * 0.5f;
				
				const ImVec2 mouse_pos = im::GetMousePos();
				if (!video_layer->playing && !m_moving_media && ImRect(col_min, col_max).Contains(mouse_pos) && im::IsMouseDown(ImGuiMouseButton_Left)) {
					video_layer->current_time = horizontal_offset + horizontal_view * kl::wrap(mouse_pos.x, col_min.x, col_max.x);
				}

				draw_list->AddLine(ImVec2(col_min.x, col_middle), ImVec2(col_max.x, col_middle), ImColor(75, 75, 75), 1.0f);

				for (int i = (int) horizontal_offset; i < int(horizontal_offset + horizontal_view); i++) {
					const float x = col_min.x + col_width * kl::wrap(float(i), horizontal_offset, horizontal_offset + horizontal_view);
					if (x <= col_min.x || x > col_max.x)
						continue;

					float height;
					ImColor color;
					Optional<String> text;
					if (video_layer->timeline_minutes && i % 60 == 0) {
						height = 1.0f;
						color = { 210, 130, 130 };
						text = std::to_string(i / 60);
					}
					else if (video_layer->timeline_10seconds && i % 10 == 0) {
						height = 0.8f;
						color = { 165, 210, 130 };
						const int res = video_layer->timeline_minutes ? (i - ((i / 60) * 60)) : i;
						text = std::to_string(res / 10);
					}
					else if (video_layer->timeline_seconds) {
						height = 0.6f;
						color = { 130, 165, 210 };
					}
					else {
						continue;
					}

					const float height_delta = (col_max.y - col_min.y) * (1.0f - height) * 0.5f;
					draw_list->AddLine(ImVec2(x, col_min.y + height_delta), ImVec2(x, col_max.y - height_delta), color, 1.0f);

					if (text) {
						draw_list->AddText(ImVec2(x, col_min.y + height_delta), color, text.value().c_str());
					}
				}

				total_min = col_min;
				total_max = col_max;
			}

			/* TRACKS */
			const float row_height = (available_height - header_height) / vertical_view;
			for (int i = vertical_offset; i < vertical_offset + vertical_view && i < (int) video_layer->tracks.size(); i++) {
				Ref track = video_layer->tracks[i];

				im::TableNextRow(ImGuiTableRowFlags_None, row_height);
				{
					im::TableSetColumnIndex(0);
					im::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(45, 45, 45, 255)); // crashes if i pass ImColor(45, 45, 45) in release ???
					
					const String text = kl::format(i + 1, ". ", track->name);
					const ImVec2 text_size = im::CalcTextSize(text.c_str());
					
					const ImVec2 cursor_pos = im::GetCursorScreenPos();
					const float vertical_off = (row_height - text_size.y) * 0.5f;

					im::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y - cell_padding.y + vertical_off));
					im::Text(text.c_str());
				}
				{
					im::TableSetColumnIndex(1);

					const ImVec2 col_min = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Min;
					const ImVec2 col_max = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Max;
					const float col_width = col_max.x - col_min.x;
					const float col_middle = (col_min.y + col_max.y) * 0.5f;

					if (m_moving_media && ImRect(col_min, col_max).Contains(im::GetMousePos()) && im::IsMouseReleased(ImGuiMouseButton_Left)) {
						auto& info = m_moving_media.value();
						const float mouse_delta = im::GetMousePos().x - info.old_mouse.x;
						const float offset_delta = (mouse_delta / col_width) * horizontal_view;
						const float offset = (horizontal_offset - info.horiz_offset) + (info.offset + offset_delta);
						video_layer->tracks[info.track_index]->remove_media(info.media);
						track->insert_media(offset, info.media);
					}

					const float min_time = horizontal_offset;
					const float max_time = horizontal_offset + horizontal_view;
					for (const auto& [offset, media] : track->media) {
						const float media_start = offset;
						const float media_end = offset + media->duration;
						if (media_end < min_time || media_start > max_time)
							continue;

						const float left_x = kl::unwrap(kl::wrap(media_start, min_time, max_time), col_min.x, col_max.x);
						const float right_x = kl::unwrap(kl::wrap(media_end, min_time, max_time), col_min.x, col_max.x);
						if (!m_moving_media && im::IsWindowFocused() && im::IsMouseClicked(ImGuiMouseButton_Left)) {
							const ImVec2 mouse_pos = im::GetMousePos();
							if (ImRect(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y)).Contains(mouse_pos)) {
								MovingMediaInfo info{};
								info.track_index = i;
								info.media = media;
								info.horiz_offset = horizontal_offset;
								info.offset = offset;
								info.old_mouse = mouse_pos;
								info.mouse_offset = mouse_pos - ImVec2(left_x, col_min.y);
								m_moving_media = info;
							}
						}
						
						if (m_moving_media && m_moving_media.value().media == media) {
							const float width = col_width * kl::wrap(media->duration, 0.0f, horizontal_view);
							const float height = col_max.y - col_min.y;
							const ImVec2 top_left = im::GetMousePos() - m_moving_media.value().mouse_offset;
							const ImVec2 bottom_right = top_left + ImVec2(width, height);
							draw_list->AddRectFilled(top_left, bottom_right, color_classify(media->type), 5.0f);
							draw_list->AddText(top_left, ImColor(30, 30, 30), media->name.c_str());
						}
						else {
							draw_list->AddRectFilled(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y), color_classify(media->type), 5.0f);
							draw_list->AddText(ImVec2(left_x, col_min.y), ImColor(30, 30, 30), media->name.c_str());
						}
					}

					if (col_max.y > total_max.y) {
						total_max = col_max;
					}
				}
			}
			if (im::IsMouseReleased(ImGuiMouseButton_Left)) {
				m_moving_media = std::nullopt;
			}

			/* POINTER */
			{
				const float x = total_min.x + (total_max.x - total_min.x) *
					kl::wrap(video_layer->current_time, horizontal_offset, horizontal_offset + horizontal_view);
				if (x > total_min.x && x < total_max.x) {
					const ImColor color{ 255, 255, 255 };
					draw_list->AddLine(ImVec2(x, total_min.y), ImVec2(x, total_max.y), color, 1.0f);
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

ImColor titian::GUISectionVideoTimeline::color_classify(MediaType type)
{
	switch (type)
	{
	case MediaType::IMAGE: return ImColor(215, 180, 125);
	case MediaType::AUDIO: return ImColor(125, 180, 215);
	case MediaType::VIDEO: return ImColor(135, 215, 135);
	}
	return ImColor();
}
