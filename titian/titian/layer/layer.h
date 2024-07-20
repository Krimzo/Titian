#pragma once

#include "utility/time_util.h"


namespace titian {
	class AppLayer;
	class GameLayer;
	class EditorLayer;
	class RenderLayer;
	class GUILayer;
}

namespace titian {
	class LayerPackage
	{
	public:
		friend class BOUND_LAYERS;

		inline LayerPackage()
		{}

		inline LayerPackage(AppLayer* app_layer, GameLayer* game_layer, EditorLayer* editor_layer, RenderLayer* render_layer, GUILayer* gui_layer)
		{
			this->app_layer = app_layer;
			this->game_layer = game_layer;
			this->editor_layer = editor_layer;
			this->render_layer = render_layer;
			this->gui_layer = gui_layer;
		}

		inline void load_layers(const LayerPackage& package)
		{
			app_layer = package.app_layer;
			game_layer = package.game_layer;
			editor_layer = package.editor_layer;
			render_layer = package.render_layer;
			gui_layer = package.gui_layer;
		}

	protected:
		AppLayer* app_layer = nullptr;
		GameLayer* game_layer = nullptr;
		EditorLayer* editor_layer = nullptr;
		RenderLayer* render_layer = nullptr;
		GUILayer* gui_layer = nullptr;
	};
}

namespace titian {
	class BOUND_LAYERS {
	public:
		static inline AppLayer* app_layer = nullptr;
		static inline GameLayer* game_layer = nullptr;
		static inline EditorLayer* editor_layer = nullptr;
		static inline RenderLayer* render_layer = nullptr;
		static inline GUILayer* gui_layer = nullptr;

		static inline void bind(const LayerPackage& package)
		{
			app_layer = package.app_layer;
			game_layer = package.game_layer;
			editor_layer = package.editor_layer;
			render_layer = package.render_layer;
			gui_layer = package.gui_layer;
		}
	};
}

namespace titian {
	class LayerHolder : public LayerPackage
	{
	public:
		inline LayerHolder(const LayerPackage& package)
		{
			load_layers(package);
		}
	};
}

namespace titian {
	class Layer : public LayerPackage, public BenchmarkInfo
	{
	public:
		Layer(const String& name);
		virtual ~Layer() = default;

		Layer(const Layer&) = delete;
		Layer(Layer&&) = delete;

		void operator=(const Layer&) = delete;
		void operator=(Layer&&) = delete;

		virtual bool update() = 0;

	protected:
		void panic() const;
	};
}
