#pragma once

#include "scripting/script.h"
#include "scene/scene.h"
#include "mmodule.h"


namespace titian {
	class NativeScript : public Script
	{
	public:
		template<typename Return, typename... Args>
		using Function = Return(__stdcall*)(Args...);

		std::vector<byte> data = {};
		std::string path = {};

		NativeScript();

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		bool is_valid() const override;
		void reload() override;
		void call_start() override;
		void call_update() override;

	private:
		void* m_memory_module = nullptr;
		Function<void, Scene*> m_start_function = nullptr;
		Function<void, Scene*> m_update_function = nullptr;

		void unload();

		template<typename Return, typename... Args>
		Function<Return, Args...> read_function(const std::string_view& function_name)
		{
			if (m_memory_module) {
				auto function_address = MemoryGetProcAddress(m_memory_module, function_name.data());
				return reinterpret_cast<Function<Return, Args...>>(function_address);
			}
			return nullptr;
		}
	};
}
