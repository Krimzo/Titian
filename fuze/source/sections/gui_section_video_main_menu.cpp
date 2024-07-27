#include "fuze.h"


titian::GUISectionVideoMainMenu::GUISectionVideoMainMenu()
	: GUISection("GUISectionVideoMainMenu")
{}

void titian::GUISectionVideoMainMenu::render_gui()
{
    AppLayer* app_layer = Layers::get<AppLayer>();
    VideoLayer* video_layer = Layers::get<VideoLayer>();

    if (m_testing_exit) {
        if (im::Begin("Exit?", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            im::Text("Are you sure you want to exit?");
            if (im::Button("No", ImVec2(50.0f, 0.0f))) {
                m_testing_exit = false;
            }
            im::SameLine();
            if (im::Button("Yes", ImVec2(40.0f, 0.0f))) {
                app_layer->window.close();
            }
        }
        im::End();
    }

    im::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
    im::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4) ImColor(0, 0, 0));

    if (im::BeginMainMenuBar()) {
        // File
        if (im::BeginMenu("File")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::BeginMenu("Import")) {
                if (im::MenuItem("Image")) {
                    if (Optional file = kl::choose_file(false, { { "Image File",  FILE_EXTENSION_JPG }, { "Image File",  FILE_EXTENSION_PNG }, { "Image File",  FILE_EXTENSION_BMP } })) {
                        video_layer->load_image(file.value());
                    }
                }
                if (im::MenuItem("Audio")) {
                    if (Optional file = kl::choose_file(false, { { "Audio File",  ".mp3" }, { "Audio File",  ".wav" } })) {
                        video_layer->load_audio(file.value());
                    }
                }
                if (im::MenuItem("Video")) {
                    if (Optional file = kl::choose_file(false, { { "Video File",  ".mp4" } })) {
                        video_layer->load_video(file.value());
                    }
                }
                im::EndMenu();
            }
            if (im::BeginMenu("Export")) {
                if (im::MenuItem("Frame")) {
                    int index = 0;
                    if (Optional file = kl::choose_file(true, { { "Image File",  FILE_EXTENSION_JPG }, { "Image File",  FILE_EXTENSION_PNG }, { "Image File",  FILE_EXTENSION_BMP } }, &index)) {
                        video_layer->store_frame();
                        RAWImage image{};
                        image.resize(video_layer->viewport_size);
                        video_layer->out_frame.retrieve(image);
                        
                        RAWImageType image_type = RAWImageType::JPG;
                        if (index == 1) {
                            image_type = RAWImageType::PNG;
                        }
                        else if (index == 2) {
                            image_type = RAWImageType::BMP;
                        }
                        image.save_to_file(file.value(), image_type);
                    }
                }
                im::EndMenu();
            }
            im::Separator();
            if (im::MenuItem("Exit")) {
                m_testing_exit = true;
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        // Edit
        if (im::BeginMenu("Edit")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::MenuItem("Undo")) {
            }
            if (im::MenuItem("Redo")) {
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        // View
        if (im::BeginMenu("View")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            if (im::BeginMenu("Timeline")) {
                im::Checkbox("Timeline Seconds", &video_layer->timeline_seconds);
                im::Checkbox("Timeline 10s Seconds", &video_layer->timeline_10seconds);
                im::Checkbox("Timeline Minutes", &video_layer->timeline_minutes);
				im::EndMenu();
            }
            im::PopStyleVar();
            im::EndMenu();
        }

        // About
        if (im::BeginMenu("About")) {
            im::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
            im::Text("Fuze 0.01v");
            im::PopStyleVar();
            im::EndMenu();
        }

        im::EndMainMenuBar();
    }

    im::PopStyleColor();
    im::PopStyleVar(2);
}
