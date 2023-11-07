#include "main.h"


titian::NativeScript::NativeScript()
	: Script(Type::NATIVE)
{}

void titian::NativeScript::serialize(Serializer* serializer, const void* helper_data) const
{
	Script::serialize(serializer, helper_data);

	const uint64_t size = data.size();
	serializer->write_object<uint64_t>(size);
	serializer->write_array<byte>(data.data(), size);
	serializer->write_string(path);
}

void titian::NativeScript::deserialize(const Serializer* serializer, const void* helper_data)
{
	Script::deserialize(serializer, helper_data);

	const uint64_t size = serializer->read_object<uint64_t>();
	data.resize(size);
	serializer->read_array(data.data(), size);
	serializer->read_string(path);
	this->reload();
}

bool titian::NativeScript::is_valid() const
{
	return m_memory_module && m_start_function && m_update_function;
}

void titian::NativeScript::reload()
{
	this->unload();

	if (std::filesystem::exists(path)) {
		data = kl::read_file(path);
	}

	m_memory_module = MemoryLoadLibrary(data.data(), data.size());
	m_start_function = read_function<void, Scene*>("on_start");
	m_update_function = read_function<void, Scene*>("on_update");
}

void titian::NativeScript::call_start()
{
	Scene* scene = &GameLayer::BOUND_SELF->scene;
	if (m_start_function && scene) {
		m_start_function(scene);
	}
}

void titian::NativeScript::call_update()
{
	Scene* scene = &GameLayer::BOUND_SELF->scene;
	if (m_update_function && scene) {
		m_update_function(scene);
	}
}

void titian::NativeScript::unload()
{
	m_start_function = nullptr;
	m_update_function = nullptr;
	if (m_memory_module) {
		MemoryFreeLibrary(m_memory_module);
		m_memory_module = nullptr;
	}
}
