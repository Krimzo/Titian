#pragma once

#include "scene/entity.h"


namespace titian {
	class Scene;
}

namespace titian {
	enum class ScriptType : int32_t
	{
		NATIVE,
		INTERP,
		NODE,
	};
}

namespace titian {
	class Script : kl::NoCopy, public Serializable
	{
	public:
		const ScriptType script_type;

		Script(ScriptType type);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		virtual bool is_valid() const = 0;
		virtual void reload() = 0;

		virtual void call_start(Scene* scene) = 0;
		virtual void call_update(Scene* scene) = 0;
		virtual void call_collision(Scene* scene, Entity* first, Entity* second) = 0;
		virtual void call_ui(Scene* scene) = 0;
	};
}
