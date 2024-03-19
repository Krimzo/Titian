#pragma once

#include "scripting/script.h"
#include "scene/scene.h"
#include "chaiscript.h"


namespace titian {
	inline const chaiscript::ModulePtr INTERP_SCRIPT_MODULE = chaiscript::ModulePtr(new chaiscript::Module());

	inline const std::unordered_set<std::string> INTERP_SCRIPT_KEYWORDS
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

	inline std::unordered_map<std::string, std::string> INTERP_SCRIPT_IDENTIFIERS
	{};
}

namespace titian {
	class InterpScript : public Script
	{
	public:
		std::string source = {};

		InterpScript();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_valid() const override;
		void reload() override;
		void call_start() override;
		void call_update() override;

	private:
		kl::Object<chaiscript::ChaiScript> m_engine = nullptr;
		std::function<void(Scene*)> m_start_function = {};
		std::function<void(Scene*)> m_update_function = {};
	};
}
