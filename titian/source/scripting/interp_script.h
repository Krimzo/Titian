#pragma once

#include "scripting/script.h"
#include "scene/scene.h"


namespace titian {
	inline const cs::ModulePtr INTERP_SCRIPT_MODULE = cs::ModulePtr(new cs::Module());

	inline std::set<String, std::less<>> CHAI_KEYWORDS;
	inline std::set<String, std::less<>> CHAI_TYPES;
	inline std::set<String, std::less<>> CHAI_MEMBERS;
	inline std::set<String, std::less<>> CHAI_FUNCTIONS;
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
		void call_collision(Scene* scene, Entity* attacker, Entity* target) override;
		void call_ui(Scene* scene) override;

		StringMap<cs::Boxed_Value> get_parameters();

	private:
		Ref<cs::ChaiScript> m_engine;
		Function<void(Scene*)> m_start_function;
		Function<void(Scene*)> m_update_function;
		Function<void(Scene*, Entity*, Entity*)> m_collision_function;
		Function<void(Scene*)> m_ui_function;
	};
}
