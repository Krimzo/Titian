#pragma once

#include "scripting/script.h"
#include "scene/scene.h"
#include "chaiscript.h"


namespace titian {
	inline const chaiscript::ModulePtr INTER_SCRIPT_MODULE = chaiscript::ModulePtr(new chaiscript::Module());

	inline const std::unordered_set<std::string> INTER_SCRIPT_KEYWORDS
	{
		"attr",
		"auto",
		"break",
		"continue",
		"def",
		"else",
		"for",
		"fun",
		"global",
		"if",
		"return",
		"try",
		"while",
		"var",
	};

	inline std::unordered_map<std::string, std::string> INTER_SCRIPT_IDENTIFIERS
	{};
}

namespace titian {
	class InterScript : public Script
	{
	public:
		std::string source = {};

		InterScript();

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
