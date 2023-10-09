export module gui_section;

export import klib;

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
