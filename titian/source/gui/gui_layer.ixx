export module gui_layer;

export import application_layer;
export import render_layer;

export namespace titian {
	class GUILayer : public Layer
	{
	public:
		kl::Object<ApplicationLayer> app_layer = nullptr;

		kl::Float4 special_color = kl::Color(235, 170, 15);
		kl::Float4 alternate_color = kl::Color(125, 190, 190);
		
		GUILayer(kl::Object<ApplicationLayer>& app_layer)
		{
			this->app_layer = app_layer;

			kl::Window* window = &app_layer->window;
			kl::GPU* gpu = &app_layer->gpu;

			ImGui::CreateContext();
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			ImGui::StyleColorsDark();
			load_custom_theme();
			load_custom_font();

			ImGui_ImplWin32_Init(static_cast<HWND>(*window));
			ImGui_ImplDX11_Init(gpu->device().Get(), gpu->context().Get());
		}

		~GUILayer() override
		{}

		bool update() override
		{
			kl::GPU* gpu = &app_layer->gpu;

			gpu->swap_buffers(true);
			return true;
		}

	private:
		void load_custom_theme() const
		{
		    ImGuiStyle& style = ImGui::GetStyle();

		    style.Colors[ImGuiCol_Text]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		    
		    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		    style.Colors[ImGuiCol_ChildBg]  = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		    style.Colors[ImGuiCol_PopupBg]  = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

		    style.Colors[ImGuiCol_Border]       = ImVec4(0.45f, 0.45f, 0.45f, 0.50f);
		    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.60f, 0.60f, 0.60f, 0.40f);

		    style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

		    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

		    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		    style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		    style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);

		    style.Colors[ImGuiCol_CheckMark] = (const ImVec4&) special_color;

		    style.Colors[ImGuiCol_SliderGrab]       = (const ImVec4&) special_color;
		    style.Colors[ImGuiCol_SliderGrabActive] = (const ImVec4&) special_color;

		    style.Colors[ImGuiCol_Button]        = style.Colors[ImGuiCol_FrameBg];
		    style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_FrameBgHovered];
		    style.Colors[ImGuiCol_ButtonActive]  = style.Colors[ImGuiCol_FrameBgActive];

		    style.Colors[ImGuiCol_Header]        = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		    style.Colors[ImGuiCol_HeaderActive]  = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);

		    style.Colors[ImGuiCol_Separator]        = style.Colors[ImGuiCol_Border];
		    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		    style.Colors[ImGuiCol_SeparatorActive]  = (const ImVec4&) special_color;

		    style.Colors[ImGuiCol_ResizeGrip]        = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.67f);
		    style.Colors[ImGuiCol_ResizeGripActive]  = (const ImVec4&) special_color;

		    style.Colors[ImGuiCol_Tab]                = ImVec4(0.08f, 0.08f, 0.08f, 0.83f);
		    style.Colors[ImGuiCol_TabHovered]         = ImVec4(0.35f, 0.35f, 0.35f, 0.83f);
		    style.Colors[ImGuiCol_TabActive]          = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		    style.Colors[ImGuiCol_TabUnfocused]       = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

		    style.Colors[ImGuiCol_DockingPreview] = (const ImVec4&) special_color;
		    style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

		    style.Colors[ImGuiCol_PlotLines]            = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		    style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		    style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

		    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.42f, 0.69f, 0.69f, 0.32f);

		    style.Colors[ImGuiCol_DragDropTarget] = (const ImVec4&) special_color;

		    style.Colors[ImGuiCol_NavHighlight]          = (const ImVec4&) special_color;
		    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		    style.WindowPadding = ImVec2(8.00f, 8.00f);
		    style.FramePadding = ImVec2(5.00f, 2.00f);
		    style.CellPadding = ImVec2(6.00f, 6.00f);
		    style.ItemSpacing = ImVec2(6.00f, 6.00f);
		    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
		    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		    style.IndentSpacing = 25;
		    style.ScrollbarSize = 15;
		    style.GrabMinSize = 10;
		    style.WindowBorderSize = 1;
		    style.ChildBorderSize = 1;
		    style.PopupBorderSize = 1;
		    style.FrameBorderSize = 1;
		    style.TabBorderSize = 1;
		    style.WindowRounding = 7;
		    style.ChildRounding = 4;
		    style.FrameRounding = 3;
		    style.PopupRounding = 4;
		    style.ScrollbarRounding = 9;
		    style.GrabRounding = 3;
		    style.LogSliderDeadzone = 4;
		    style.TabRounding = 4;
		}

		void load_custom_font() const
		{
		    const ImGuiIO& io = ImGui::GetIO();
		    io.Fonts->AddFontFromFileTTF("builtin/fonts/Roboto.ttf", 16);
		}
	};
}