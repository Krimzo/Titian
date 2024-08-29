#pragma once

#include "scripting/script.h"
#include "scene/scene.h"


namespace titian {
	inline std::set<String, std::less<>> LUA_KEYWORDS;
	inline std::set<String, std::less<>> LUA_TYPES;
	inline std::set<String, std::less<>> LUA_MEMBERS;
	inline std::set<String, std::less<>> LUA_FUNCTIONS;
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

		struct Parameter
		{
			InterpScript* parent = nullptr;
			String name;

			inline Parameter(InterpScript* parent, String name)
				: parent(parent), name(std::move(name))
			{}

			template<typename T>
			bool is() const
			{
				return (*parent->m_engine)[name].is<T>();
			}

			template<typename T>
			T get() const
			{
				return (*parent->m_engine)[name];
			}

			template<typename T>
			void set(const T& value)
			{
				(*parent->m_engine)[name] = value;
			}
		};

		StringMap<Parameter> get_parameters();

		inline const auto& get_engine() const { return *m_engine; };

	private:
		Ref<sl::state> m_engine;
		
		Ref<sl::function> m_start_function;
		Ref<sl::function> m_update_function;
		Ref<sl::function> m_collision_function;
		Ref<sl::function> m_ui_function;

		void load_engine_parts();
	};
}
