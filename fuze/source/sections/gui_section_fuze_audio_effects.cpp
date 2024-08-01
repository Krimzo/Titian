#include "fuze.h"


titian::GUISectionFuzeAudioEffects::GUISectionFuzeAudioEffects()
	: GUISection("GUISectionFuzeAudioEffects")
{}

void titian::GUISectionFuzeAudioEffects::render_gui()
{
	static const Vector<Pair<String, Function<Ref<AudioEffect>()>>> audio_effects = {
		{ "Volume", [] { return new AudioEffectVolume(); } },
	};

	VideoLayer* video_layer = Layers::get<VideoLayer>();
	Media* selected_media = &video_layer->selected_media;

	if (im::Begin("Audio Effects") && selected_media) {
		if (im::BeginPopupContextWindow("New Effect", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			int counter = 0;
			im::Text("New Effect");
			for (const auto& [title, provider] : audio_effects) {
				counter += 1;
				im::Text(kl::format(counter, '.').c_str());
				im::SameLine();
				if (im::Button(title.c_str())) {
					selected_media->audio_effects.push_back(provider());
					im::CloseCurrentPopup();
				}
			}
			im::EndPopup();
		}

		Set<int> to_delete;

		int counter = 0;
		for (auto& effect : selected_media->audio_effects) {
			if (counter > 0) {
				im::Separator();
			}

			im::Text(kl::format(counter + 1, '.').c_str());
			im::SameLine();

			im::Selectable(effect->get_name().c_str());
			if (im::BeginPopupContextItem()) {
				if (counter > 0) {
					if (im::Button("Move Up")) {
						std::swap(selected_media->audio_effects[counter], selected_media->audio_effects[(size_t) counter - 1]);
						im::CloseCurrentPopup();
					}
				}
				if (counter < (int) selected_media->audio_effects.size() - 1) {
					if (im::Button("Move Down")) {
						std::swap(selected_media->audio_effects[counter], selected_media->audio_effects[(size_t) counter + 1]);
						im::CloseCurrentPopup();
					}
				}
				if (im::Button("Delete")) {
					to_delete.insert(counter);
					im::CloseCurrentPopup();
				}
				im::EndPopup();
			}

			im::PushID(counter);
			effect->display_gui();
			im::PopID();
			counter += 1;
		}

		for (const int index : to_delete) {
			selected_media->audio_effects.erase(selected_media->audio_effects.begin() + index);
		}
	}
	im::End();
}
