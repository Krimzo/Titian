#pragma once

#include "scripting/script.h"
#include "scene/scene.h"


namespace titian {
	inline const cs::ModulePtr INTERP_SCRIPT_MODULE = cs::ModulePtr(new cs::Module());

	inline const StringSet INTERP_SCRIPT_KEYWORDS
	{
		"attr",
		"auto",
		"break",
		"continue",
		"def",
		"else",
		"false",
		"for",
		"fun",
		"global",
		"if",
		"return",
		"true",
		"try",
		"var",
		"while",
	};

	inline StringMap<String> INTERP_SCRIPT_IDENTIFIERS
	{};

	inline StringMap<String> INTERP_SCRIPT_MEMBERS
	{};
}

namespace titian {
	class InterpScript : public Script
	{
	public:
		String source;

		InterpScript();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_valid() const override;
		void reload() override;

		void call_start(Scene* scene) override;
		void call_update(Scene* scene) override;
		void call_collision(Scene* scene, Entity* first, Entity* second) override;
		void call_ui(Scene* scene) override;

		StringMap<cs::Boxed_Value> get_parameters();

	private:
		Ref<cs::ChaiScript> m_engine = nullptr;
		Function<void(Scene*)> m_start_function = {};
		Function<void(Scene*)> m_update_function = {};
		Function<void(Scene*, Entity*, Entity*)> m_collision_function = {};
		Function<void(Scene*)> m_ui_function = {};
	};
}
