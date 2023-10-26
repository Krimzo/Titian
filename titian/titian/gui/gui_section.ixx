export module gui_section;

export import gui_helper;

export namespace titian {
	class GUISection
	{
	public:
		GUISection()
		{}

		virtual ~GUISection()
		{}

		virtual void render_gui() = 0;
	};
}
