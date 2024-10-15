#include "fuze.h"


titian::GUISectionFuzeTimeline::GUISectionFuzeTimeline()
	: GUISection("GUISectionFuzeTimeline")
{}

void titian::GUISectionFuzeTimeline::render_gui()
{
	AppLayer& app_layer = Layers::get<AppLayer>();
	VideoLayer& video_layer = Layers::get<VideoLayer>();

	kl::Window& window = app_layer.window;

	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (im::Begin("Timeline", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		const float available_height = im::GetContentRegionAvail().y;

		handle_input(window.mouse.scroll());

		constexpr ImVec2 cell_padding{ 4.0f, 4.0f };
		im::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

        if (im::BeginTable("##TracksTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthFixed);
			im::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthStretch);

			ImVec2 total_min{};
			ImVec2 total_max{};

			float header_height = 0.0f;
			render_header(cell_padding, available_height, header_height, total_min, total_max);

			const float row_height = (available_height - header_height) / vertical_view;
			for (int i = vertical_offset; i < vertical_offset + vertical_view && i < (int) video_layer.tracks.size(); i++) {
				render_track(cell_padding, row_height, i, header_height, total_min, total_max);
			}
			if (im::IsMouseReleased(ImGuiMouseButton_Left)) {
				m_moving_media = std::nullopt;
			}

			render_pointer(total_min, total_max);

            im::EndTable();
        }

		im::PopStyleVar(1);
	}
	im::End();

	im::PopStyleVar(1);
}

void titian::GUISectionFuzeTimeline::handle_input(const int scroll)
{
	AppLayer& app_layer = Layers::get<AppLayer>();
	VideoLayer& video_layer = Layers::get<VideoLayer>();
	
	im::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	if (video_layer.can_edit() && im::BeginPopupContextWindow("##TimelinePopup")) {
		if (m_editing_track && video_layer.selected_track) {
			if (auto opt_name = gui_input_waited("##RenameTrackInput", video_layer.selected_track->name)) {
				video_layer.selected_track->name = opt_name.value();
				im::CloseCurrentPopup();
			}
			if (im::Button("Move Up", { -1.0f, 0.0f })) {
				video_layer.move_track_up(*video_layer.selected_track);
				im::CloseCurrentPopup();
			}
			if (im::Button("Move Down", { -1.0f, 0.0f })) {
				video_layer.move_track_down(*video_layer.selected_track);
				im::CloseCurrentPopup();
			}
			if (im::Button("Delete Track", { -1.0f, 0.0f })) {
				video_layer.delete_track(*video_layer.selected_track);
				video_layer.selected_track = {};
				im::CloseCurrentPopup();
			}
		}
		else if (m_editing_media && video_layer.selected_media) {
			if (auto opt_name = gui_input_waited("##RenameMediaInput", video_layer.selected_media->name)) {
				video_layer.selected_media->name = opt_name.value();
				im::CloseCurrentPopup();
			}
			float offset = video_layer.get_offset(*video_layer.selected_media);
			if (im::DragFloat("Offset", &offset, 0.1f, 0.0f, 1'000'000.0f)) {
				video_layer.update_offset(video_layer.selected_media, offset);
			}
			im::DragFloat("Duration", &video_layer.selected_media->duration, 0.1f, 0.1f, 1'000'000.0f);
			if (video_layer.selected_media->type == MediaType::VIDEO
				&& video_layer.selected_media->has_audio()
				&& im::Button("Split Audio", { -1.0f, 0.0f })) {
				video_layer.split_audio(*video_layer.selected_media);
				im::CloseCurrentPopup();
			}
			if (im::Button("Delete Media", { -1.0f, 0.0f })) {
				video_layer.delete_media(*video_layer.selected_media);
				video_layer.selected_media = {};
				im::CloseCurrentPopup();
			}
		}
		else {
			if (im::Button("New Track")) {
				video_layer.tracks.emplace_back(new Track());
				im::CloseCurrentPopup();
			}
		}
		im::EndPopup();
		m_popup_was = true;
	}
	else {
		if (m_popup_was) {
			m_editing_track = false;
			m_editing_media = false;
		}
		m_popup_was = false;
	}
	im::PopStyleVar(1);

	if (im::IsWindowFocused()) {
		if (im::IsKeyPressed(ImGuiKey_Space)) {
			if (!video_layer.playing()) {
				video_layer.play();
				app_layer.timer.restart();
			}
			else {
				video_layer.stop();
				app_layer.timer.stop();
			}
		}
		if (video_layer.can_edit() && im::IsKeyPressed(ImGuiKey_Home)) {
			const float start_time = video_layer.start_time();
			video_layer.current_time = (video_layer.current_time > start_time) ? start_time : 0.0f;
		}
		if (video_layer.can_edit() && im::IsKeyPressed(ImGuiKey_End)) {
			video_layer.current_time = video_layer.end_time();
		}
	}

	if (im::IsWindowHovered()) {
		if (im::IsKeyDown(ImGuiKey_LeftAlt)) {
			vertical_view -= scroll;
			vertical_view = kl::max(vertical_view, 3);
		}
		else if (im::IsKeyDown(ImGuiKey_LeftCtrl)) {
			if (scroll < 0) {
				horizontal_view *= 1.05f;
			}
			else if (scroll > 0) {
				horizontal_view /= 1.05f;
			}
			horizontal_view = kl::max(horizontal_view, 1.0f);
		}
		else if (im::IsKeyDown(ImGuiKey_LeftShift)) {
			horizontal_offset += scroll * horizontal_view * 0.05f;
			horizontal_offset = kl::max(horizontal_offset, 0.0f);
		}
		else if (scroll != 0) {
			vertical_offset -= scroll;
			vertical_offset = kl::clamp(vertical_offset, 0, (int) video_layer.tracks.size() - 1);
		}
	}
}

void titian::GUISectionFuzeTimeline::render_header(const ImVec2 cell_padding, const float available_height, float& header_height, ImVec2& total_min, ImVec2& total_max)
{
	VideoLayer& video_layer = Layers::get<VideoLayer>();
	ImDrawList& draw_list = *im::GetWindowDrawList();

	header_height = available_height * 0.1f;
	im::TableNextRow(ImGuiTableRowFlags_None, header_height);

	im::TableSetColumnIndex(0);
	im::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(45, 45, 45));

	const String text = kl::format(std::fixed, video_layer.current_time);
	const ImVec2 text_size = im::CalcTextSize(text.data());

	const ImVec2 cursor_pos = im::GetCursorScreenPos();
	const float vertical_off = (header_height - text_size.y) * 0.5f;

	im::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y - cell_padding.y + vertical_off));
	im::Text(text.data());

	im::TableSetColumnIndex(1);
	im::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(35, 35, 35));

	const ImVec2 col_min = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Min;
	const ImVec2 col_max = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Max;
	const float col_width = col_max.x - col_min.x;
	const float col_middle = (col_min.y + col_max.y) * 0.5f;

	const ImVec2 mouse_pos = im::GetMousePos();
	if (video_layer.can_edit() && !m_moving_media && ImRect(col_min, col_max).Contains(mouse_pos) && im::IsMouseDown(ImGuiMouseButton_Left)) {
		video_layer.current_time = horizontal_offset + horizontal_view * kl::unlerp(mouse_pos.x, col_min.x, col_max.x);
	}

	draw_list.AddLine(ImVec2(col_min.x, col_middle), ImVec2(col_max.x, col_middle), ImColor(75, 75, 75), 1.0f);

	for (int i = (int) horizontal_offset; i < int(horizontal_offset + horizontal_view); i++) {
		const float x = col_min.x + col_width * kl::unlerp(float(i), horizontal_offset, horizontal_offset + horizontal_view);
		if (x <= col_min.x || x > col_max.x)
			continue;

		float height;
		ImColor color;
		Optional<String> text;
		if (video_layer.timeline_minutes && i % 60 == 0) {
			height = 1.0f;
			color = { 210, 130, 130 };
			text = std::to_string(i / 60);
		}
		else if (video_layer.timeline_10seconds && i % 10 == 0) {
			height = 0.8f;
			color = { 165, 210, 130 };
			const int res = video_layer.timeline_minutes ? (i - ((i / 60) * 60)) : i;
			text = std::to_string(res / 10);
		}
		else if (video_layer.timeline_seconds) {
			height = 0.6f;
			color = { 130, 165, 210 };
		}
		else {
			continue;
		}

		const float height_delta = (col_max.y - col_min.y) * (1.0f - height) * 0.5f;
		draw_list.AddLine(ImVec2(x, col_min.y + height_delta), ImVec2(x, col_max.y - height_delta), color, 1.0f);

		if (text) {
			draw_list.AddText(ImVec2(x, col_min.y + height_delta), color, text.value().data());
		}
	}

	total_min = col_min;
	total_max = col_max;
}

void titian::GUISectionFuzeTimeline::render_track(const ImVec2 cell_padding, const float row_height, const int i, const float header_height, const ImVec2& total_min, ImVec2& total_max)
{
	VideoLayer& video_layer = Layers::get<VideoLayer>();
	ImDrawList& draw_list = *im::GetWindowDrawList();

	Ref track = video_layer.tracks[i];

	im::TableNextRow(ImGuiTableRowFlags_None, row_height);
	im::TableSetColumnIndex(0);
	im::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImColor(45, 45, 45));

	ImRect window_rect{ im::GetWindowContentRegionMin(), im::GetWindowContentRegionMax() };
	window_rect.Translate(im::GetWindowPos());

	ImVec2 col_min = im::TableGetCellBgRect(im::GetCurrentTable(), 0).Min;
	ImVec2 col_max = im::TableGetCellBgRect(im::GetCurrentTable(), 0).Max;

	if (im::IsWindowFocused() && window_rect.Contains(im::GetMousePos())) {
		if (im::IsMouseClicked(ImGuiMouseButton_Left)) {
			if (ImRect(col_min, col_max).Contains(im::GetMousePos())) {
				video_layer.selected_media = {};
				video_layer.selected_track = track;
			}
			else if (video_layer.selected_track == track) {
				video_layer.selected_track = {};
			}
		}
		if (im::IsMouseClicked(ImGuiMouseButton_Right)) {
			if (ImRect(col_min, col_max).Contains(im::GetMousePos())) {
				video_layer.selected_media = {};
				video_layer.selected_track = track;
				m_editing_track = true;
			}
		}
	}

	if (video_layer.selected_track == track) {
		draw_list.AddRect(col_min, col_max, ImColor(220, 220, 220));
	}

	const String text = kl::format(i + 1, ". ", track->name);
	const ImVec2 text_size = im::CalcTextSize(text.data());

	const ImVec2 cursor_pos = im::GetCursorScreenPos();
	const float vertical_off = (row_height - text_size.y) * 0.5f;

	im::SetCursorScreenPos(ImVec2(cursor_pos.x, cursor_pos.y - cell_padding.y + vertical_off));
	im::Text(text.data());

	im::TableSetColumnIndex(1);

	col_min = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Min;
	col_max = im::TableGetCellBgRect(im::GetCurrentTable(), 1).Max;
	col_min += ImVec2(1.0f, 1.0f);
	const float col_width = col_max.x - col_min.x;
	const float col_middle = (col_min.y + col_max.y) * 0.5f;

	if (m_moving_media && ImRect(col_min, col_max).Contains(im::GetMousePos()) && im::IsMouseReleased(ImGuiMouseButton_Left)) {
		auto& info = m_moving_media.value();
		const float mouse_delta = im::GetMousePos().x - info.old_mouse.x;
		const float offset_delta = (mouse_delta / col_width) * horizontal_view;
		const float offset = (horizontal_offset - info.horiz_offset) + (info.offset + offset_delta);
		video_layer.tracks[info.track_index]->remove_media(*info.media);
		track->insert_media(offset, info.media);
	}

	const float min_time = horizontal_offset;
	const float max_time = horizontal_offset + horizontal_view;
	for (const auto& [offset, media] : track->media) {
		const float media_start = offset;
		const float media_end = offset + media->duration;

		const float left_x_no_clamp = kl::lerp<float, false>(kl::unlerp<float, false>(media_start, min_time, max_time), col_min.x, col_max.x);
		const float left_x = kl::lerp(kl::unlerp(media_start, min_time, max_time), col_min.x, col_max.x);
		const float right_x = kl::lerp(kl::unlerp(media_end, min_time, max_time), col_min.x, col_max.x);
		if (video_layer.can_edit() && !m_moving_media && im::IsWindowFocused() && im::IsMouseClicked(ImGuiMouseButton_Left)) {
			const ImVec2 mouse_pos = im::GetMousePos();
			if (ImRect(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y)).Contains(mouse_pos)) {
				MovingMediaInfo info{};
				info.track_index = i;
				info.media = media;
				info.horiz_offset = horizontal_offset;
				info.offset = offset;
				info.old_mouse = mouse_pos;
				info.mouse_offset = mouse_pos - ImVec2(left_x_no_clamp, col_min.y);
				m_moving_media = info;
			}
		}

		if (im::IsWindowFocused() && window_rect.Contains(im::GetMousePos())) {
			if (im::IsMouseClicked(ImGuiMouseButton_Left)) {
				if (ImRect(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y)).Contains(im::GetMousePos())) {
					video_layer.selected_track = {};
					video_layer.selected_media = media;
				}
				else if (video_layer.selected_media == media) {
					video_layer.selected_media = {};
				}
			}
			if (im::IsMouseClicked(ImGuiMouseButton_Right)) {
				if (ImRect(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y)).Contains(im::GetMousePos())) {
					video_layer.selected_track = {};
					video_layer.selected_media = media;
					m_editing_media = true;
				}
			}
		}

		if (m_moving_media && m_moving_media.value().media == media) {
			const float width = col_width * kl::unlerp<float, false>(media->duration, 0.0f, horizontal_view);
			const float height = col_max.y - col_min.y;
			const ImVec2 top_left = im::GetMousePos() - m_moving_media.value().mouse_offset;
			const ImVec2 bottom_right = top_left + ImVec2(width, height);
			const ImVec2 top_left_clamped = ImVec2(kl::clamp(top_left.x, col_min.x, col_max.x), kl::max(top_left.y, total_min.y + header_height));
			const ImVec2 bottom_right_clamped = ImVec2(kl::clamp(bottom_right.x, col_min.x, col_max.x), kl::max(bottom_right.y, total_min.y + header_height));

			if (media->type == MediaType::VIDEO) {
				draw_list.AddRectFilled(top_left_clamped, bottom_right_clamped, ImColor(40, 40, 40));
				const float section_width = col_width * kl::unlerp<float, false>(Video::BUFFERING_LENGTH, 0.0f, horizontal_view);
				const float core_width = section_width / Video::SECTION_THREAD_COUNT;
				float width_offset = 0.0f;
				for (const auto& section : media->video->buffering_sections()) {
					for (const auto& [state, core] : section) {
						float left = top_left.x + width_offset;
						float right = kl::min(left + core_width * core, bottom_right.x);
						left = kl::clamp(left, col_min.x, bottom_right_clamped.x);
						right = kl::clamp(right, col_min.x, bottom_right_clamped.x);
						const ImColor color = state ? color_classify(*media) : ImColor(205, 120, 115);
						draw_list.AddRectFilled(ImVec2(left, top_left_clamped.y), ImVec2(right, bottom_right_clamped.y), color);
						width_offset += core_width;
					}
				}
			}
			else {
				draw_list.AddRectFilled(top_left_clamped, bottom_right_clamped, color_classify(*media));
			}

			if (video_layer.selected_media == media) {
				draw_list.AddRect(top_left_clamped, bottom_right_clamped, ImColor(255, 255, 255));
			}
			draw_list.AddText(top_left_clamped, ImColor(30, 30, 30), media->name.data());
		}
		else {
			if (media->type == MediaType::VIDEO) {
				draw_list.AddRectFilled(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y), ImColor(40, 40, 40));
				const float section_width = col_width * kl::unlerp<float, false>(Video::BUFFERING_LENGTH, 0.0f, horizontal_view);
				const float core_width = section_width / Video::SECTION_THREAD_COUNT;
				float width_offset = 0.0f;
				for (const auto& section : media->video->buffering_sections()) {
					for (const auto& [state, core] : section) {
						float left = left_x_no_clamp + width_offset;
						float right = left + core_width * core;
						left = kl::clamp(left, left_x, right_x);
						right = kl::clamp(right, left_x, right_x);
						const ImColor color = state ? color_classify(*media) : ImColor(205, 120, 115);
						draw_list.AddRectFilled(ImVec2(left, col_min.y), ImVec2(right, col_max.y), color);
						width_offset += core_width;
					}
				}
			}
			else {
				draw_list.AddRectFilled(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y), color_classify(*media));
			}
			if (video_layer.selected_media == media) {
				draw_list.AddRect(ImVec2(left_x, col_min.y), ImVec2(right_x, col_max.y), ImColor(255, 255, 255));
			}
			draw_list.AddText(ImVec2(left_x, col_min.y), ImColor(30, 30, 30), media->name.data());
		}
	}

	if (col_max.y > total_max.y) {
		total_max = col_max;
	}
}

void titian::GUISectionFuzeTimeline::render_pointer(const ImVec2 total_min, const ImVec2 total_max)
{
	VideoLayer& video_layer = Layers::get<VideoLayer>();
	ImDrawList& draw_list = *im::GetWindowDrawList();

	const float x = total_min.x + (total_max.x - total_min.x) *
		kl::unlerp(video_layer.current_time, horizontal_offset, horizontal_offset + horizontal_view);
	if (x > total_min.x && x < total_max.x) {
		const ImColor color{ 255, 255, 255 };
		draw_list.AddLine(ImVec2(x, total_min.y), ImVec2(x, total_max.y), color, 1.0f);
	}
}

ImColor titian::GUISectionFuzeTimeline::color_classify(const Media& media)
{
	static constexpr ImColor IMAGE_COLOR = ImColor(215, 180, 125);
	static constexpr ImColor AUDIO_COLOR = ImColor(125, 180, 215);
	static constexpr ImColor VIDEO_COLOR = ImColor(135, 215, 135);

	switch (media.type)
	{
	case MediaType::IMAGE:
		return IMAGE_COLOR;

	case MediaType::AUDIO:
		return AUDIO_COLOR;

	case MediaType::VIDEO:
		if (media.has_audio()) {
			return ImVec4(VIDEO_COLOR) * ImVec4(0.5f, 0.5f, 0.5f, 0.5f) + ImVec4(AUDIO_COLOR) * ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
		}
		else {
			return VIDEO_COLOR;
		}
	}
	return {};
}
