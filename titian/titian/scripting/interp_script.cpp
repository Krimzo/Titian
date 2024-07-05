#include "main.h"

namespace cs = chaiscript;


/* Inter script */
titian::InterpScript::InterpScript()
	: Script(ScriptType::INTERP)
{
	this->source = get_default_script();
}

void titian::InterpScript::serialize(Serializer* serializer, const void* helper_data) const
{
	Script::serialize(serializer, helper_data);

	serializer->write_string(source);
}

void titian::InterpScript::deserialize(const Serializer* serializer, const void* helper_data)
{
	Script::deserialize(serializer, helper_data);

	serializer->read_string(source);
	this->reload();
}

bool titian::InterpScript::is_valid() const
{
	return m_engine && m_start_function && m_update_function;
}

void titian::InterpScript::reload()
{
	m_start_function = {};
	m_update_function = {};
	m_engine = new cs::ChaiScript();

	try {
		m_engine->add(INTERP_SCRIPT_MODULE);
		m_engine->eval(this->source);
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}

	try {
		m_start_function = m_engine->eval<std::function<void(Scene*)>>("on_start");
	}
	catch (std::exception&)
	{}

	try {
		m_update_function = m_engine->eval<std::function<void(Scene*)>>("on_update");
	}
	catch (std::exception&)
	{}

	try {
		m_collision_function = m_engine->eval<std::function<void(Scene*, Entity*, Entity*)>>("on_collision");
	}
	catch (std::exception&)
	{}

	try {
		m_ui_function = m_engine->eval<std::function<void(Scene*)>>("on_ui");
	}
	catch (std::exception&)
	{}
}

void titian::InterpScript::call_start(Scene* scene)
{
	if (!m_start_function) {
		return;
	}

	try {
		m_start_function(scene);
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}
}

void titian::InterpScript::call_update(Scene* scene)
{
	if (!m_update_function) {
		return;
	}

	try {
		m_update_function(scene);
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}
}

void titian::InterpScript::call_collision(Scene* scene, Entity* first, Entity* second)
{
	if (!m_collision_function) {
		return;
	}

	try {
		m_collision_function(scene, first, second);
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}
}

void titian::InterpScript::call_ui(Scene* scene)
{
	if (!m_ui_function) {
		return;
	}

	try {
		m_ui_function(scene);
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}
}

std::map<std::string, cs::Boxed_Value> titian::InterpScript::get_parameters()
{
	if (!m_engine) {
		return {};
	}

	std::map<std::string, cs::Boxed_Value> result;
	for (auto& [name, value] : m_engine->get_state().engine_state.m_global_objects) {
		if (name.starts_with("p_")) {
			result[name] = value;
		}
	}
	return result;
}

/* SCRIPT BINDS */
const int load_types = [&]
{
	using namespace titian;
	
	// Bootstrap
	cs::bootstrap::standard_library::vector_type<std::vector<kl::Vertex<float>>>("MeshData", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<std::vector<kl::Color>>("TextureData", *INTERP_SCRIPT_MODULE);

	cs::bootstrap::standard_library::vector_type<std::vector<Mesh*>>("MeshVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<std::vector<Texture*>>("TextureVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<std::vector<Material*>>("MaterialVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<std::vector<Entity*>>("EntityVector", *INTERP_SCRIPT_MODULE);

	cs::bootstrap::standard_library::map_type<std::map<std::string, Mesh*>>("MeshMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<std::map<std::string, Texture*>>("TextureMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<std::map<std::string, Material*>>("MaterialMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<std::map<std::string, Entity*>>("EntityMap", *INTERP_SCRIPT_MODULE);

	// Derived
	INTERP_SCRIPT_MODULE->add(cs::base_class<Entity, Camera>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Entity, Light>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, AmbientLight>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, PointLight>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, DirectionalLight>());

	// Casts
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Int2, kl::Float2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float2, kl::Int2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float2, kl::Complex<float>>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float3, kl::Color>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float3, kl::Quaternion<float>>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float4, kl::Color>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Float4, kl::Quaternion<float>>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Complex<float>, kl::Float2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Quaternion<float>, kl::Float3>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Quaternion<float>, kl::Float4>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Color, kl::Float3>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<kl::Color, kl::Float4>());

	// Int2
	cs::utility::add_class<kl::Int2>(*INTERP_SCRIPT_MODULE, "Int2",
	{
		cs::constructor<kl::Int2()>(),
		cs::constructor<kl::Int2(const kl::Int2&)>(),

		cs::constructor<kl::Int2(int)>(),
		cs::constructor<kl::Int2(int, int)>(),
	},
	{
		{ cs::fun(&kl::Int2::x), "x" },
		{ cs::fun(&kl::Int2::y), "y" },

		{ cs::fun<kl::Int2 & (kl::Int2::*)(const kl::Int2&)>(&kl::Int2::operator=), "=" },

		{ cs::fun<int& (kl::Int2::*)(int)>(&kl::Int2::operator[]), "[]"},

		{ cs::fun(&kl::Int2::operator==), "==" },
		{ cs::fun(&kl::Int2::operator!=), "!=" },

		{ cs::fun(&kl::Int2::operator+), "+" },
		{ cs::fun(&kl::Int2::operator+=), "+=" },

		{ cs::fun<kl::Int2(kl::Int2::*)(const kl::Int2&) const>(&kl::Int2::operator-), "-"},
		{ cs::fun(&kl::Int2::operator-=), "-=" },

		{ cs::fun<kl::Int2 (kl::Int2::*)(int) const>(&kl::Int2::operator*), "*" },
		{ cs::fun<void (kl::Int2::*)(int)>(&kl::Int2::operator*=), "*=" },

		{ cs::fun<kl::Int2 (kl::Int2::*)(const kl::Int2&) const>(&kl::Int2::operator*), "*" },
		{ cs::fun<void (kl::Int2::*)(const kl::Int2&)>(&kl::Int2::operator*=), "*=" },

		{ cs::fun<kl::Int2 (kl::Int2::*)(int) const>(&kl::Int2::operator/), "/" },
		{ cs::fun<void (kl::Int2::*)(int)>(&kl::Int2::operator/=), "/=" },

		{ cs::fun<kl::Int2 (kl::Int2::*)(const kl::Int2&) const>(&kl::Int2::operator/), "/" },
		{ cs::fun<void (kl::Int2::*)(const kl::Int2&)>(&kl::Int2::operator/=), "/=" },

		{ cs::fun<kl::Int2(kl::Int2::*)() const>(&kl::Int2::operator-), "-" },

		{ cs::fun([](const kl::Int2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Int2"] = "Two component integer vector.";
	INTERP_SCRIPT_MEMBERS["x"] = "x";
	INTERP_SCRIPT_MEMBERS["x"] = "y";

	// Float2
	cs::utility::add_class<kl::Float2>(*INTERP_SCRIPT_MODULE, "Float2",
	{
		cs::constructor<kl::Float2()>(),
		cs::constructor<kl::Float2(const kl::Float2&)>(),

		cs::constructor<kl::Float2(float)>(),
		cs::constructor<kl::Float2(float, float)>(),
	},
	{
		{ cs::fun(&kl::Float2::x), "x" },
		{ cs::fun(&kl::Float2::y), "y" },

		{ cs::fun<kl::Float2& (kl::Float2::*)(const kl::Float2&)>(&kl::Float2::operator=), "=" },

		{ cs::fun<float& (kl::Float2::*)(int)>(&kl::Float2::operator[]), "[]"},

		{ cs::fun(&kl::Float2::operator==), "==" },
		{ cs::fun(&kl::Float2::operator!=), "!=" },

		{ cs::fun(&kl::Float2::operator+), "+" },
		{ cs::fun(&kl::Float2::operator+=), "+=" },

		{ cs::fun<kl::Float2 (kl::Float2::*)(const kl::Float2&) const>(&kl::Float2::operator-), "-"},
		{ cs::fun(&kl::Float2::operator-=), "-=" },

		{ cs::fun<kl::Float2 (kl::Float2::*)(float) const>(&kl::Float2::operator*), "*" },
		{ cs::fun<void (kl::Float2::*)(float)>(&kl::Float2::operator*=), "*="},

		{ cs::fun<kl::Float2 (kl::Float2::*)(const kl::Float2&) const>(&kl::Float2::operator*), "*" },
		{ cs::fun<void (kl::Float2::*)(const kl::Float2&)>(&kl::Float2::operator*=), "*=" },

		{ cs::fun<kl::Float2 (kl::Float2::*)(float) const>(&kl::Float2::operator/), "/" },
		{ cs::fun<void (kl::Float2::*)(float)>(&kl::Float2::operator/=), "/=" },

		{ cs::fun<kl::Float2 (kl::Float2::*)(const kl::Float2&) const>(&kl::Float2::operator/), "/" },
		{ cs::fun<void (kl::Float2::*)(const kl::Float2&)>(&kl::Float2::operator/=), "/=" },

		{ cs::fun<kl::Float2 (kl::Float2::*)() const>(&kl::Float2::operator-), "-"},
		{ cs::fun(&kl::Float2::length), "length" },

		{ cs::fun([](const kl::Float2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float2"] = "Two component float vector.";
	INTERP_SCRIPT_IDENTIFIERS["length"] = "Returns length of this vector.";

	// Float3
	cs::utility::add_class<kl::Float3>(*INTERP_SCRIPT_MODULE, "Float3",
	{
		cs::constructor<kl::Float3()>(),
		cs::constructor<kl::Float3(const kl::Float3&)>(),

		cs::constructor<kl::Float3(float)>(),
		cs::constructor<kl::Float3(float, float, float)>(),

		cs::constructor<kl::Float3(kl::Float2, float)>(),
		cs::constructor<kl::Float3(float, kl::Float2)>(),
	},
	{
		{ cs::fun(&kl::Float3::x), "x" },
		{ cs::fun(&kl::Float3::y), "y" },
		{ cs::fun(&kl::Float3::z), "z" },

		{ cs::fun<kl::Float3& (kl::Float3::*)(const kl::Float3&)>(&kl::Float3::operator=), "=" },

		{ cs::fun<kl::Float2 & (kl::Float3::*)()>(&kl::Float3::xy), "xy"},
		{ cs::fun<kl::Float2& (kl::Float3::*)()>(&kl::Float3::yz), "yz" },

		{ cs::fun<float& (kl::Float3::*)(int)>(&kl::Float3::operator[]), "[]"},

		{ cs::fun(&kl::Float3::operator==), "==" },
		{ cs::fun(&kl::Float3::operator!=), "!=" },

		{ cs::fun(&kl::Float3::operator+), "+" },
		{ cs::fun(&kl::Float3::operator+=), "+=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)(const kl::Float3&) const>(&kl::Float3::operator-), "-" },
		{ cs::fun(&kl::Float3::operator-=), "-=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)(float) const>(&kl::Float3::operator*), "*" },
		{ cs::fun<void (kl::Float3::*)(float)>(&kl::Float3::operator*=), "*=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)(const kl::Float3&) const>(&kl::Float3::operator*), "*" },
		{ cs::fun<void (kl::Float3::*)(const kl::Float3&)>(&kl::Float3::operator*=), "*=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)(float) const>(&kl::Float3::operator/), "/" },
		{ cs::fun<void (kl::Float3::*)(float)>(&kl::Float3::operator/=), "/=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)(const kl::Float3&) const>(&kl::Float3::operator/), "/" },
		{ cs::fun<void (kl::Float3::*)(const kl::Float3&)>(&kl::Float3::operator/=), "/=" },

		{ cs::fun<kl::Float3 (kl::Float3::*)() const>(&kl::Float3::operator-), "-"},
		{ cs::fun(&kl::Float3::length), "length" },

		{ cs::fun([](const kl::Float3& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float3"] = "Three component float vector.";
	INTERP_SCRIPT_MEMBERS["z"] = "z";
	INTERP_SCRIPT_IDENTIFIERS["xy"] = "Returns x and y as a Float2&.";
	INTERP_SCRIPT_IDENTIFIERS["yz"] = "Returns y and z as a Float2&.";

	// Float4
	cs::utility::add_class<kl::Float4>(*INTERP_SCRIPT_MODULE, "Float4",
	{
		cs::constructor<kl::Float4()>(),
		cs::constructor<kl::Float4(const kl::Float4&)>(),

		cs::constructor<kl::Float4(float)>(),
		cs::constructor<kl::Float4(float, float, float, float)>(),

		cs::constructor<kl::Float4(kl::Float2, float, float)>(),
		cs::constructor<kl::Float4(float, kl::Float2, float)>(),
		cs::constructor<kl::Float4(float, float, kl::Float2)>(),
		cs::constructor<kl::Float4(kl::Float2, kl::Float2)>(),

		cs::constructor<kl::Float4(kl::Float3, float)>(),
		cs::constructor<kl::Float4(float, kl::Float3)>(),
	},
	{
		{ cs::fun(&kl::Float4::x), "x" },
		{ cs::fun(&kl::Float4::y), "y" },
		{ cs::fun(&kl::Float4::z), "z" },
		{ cs::fun(&kl::Float4::w), "w" },

		{ cs::fun<kl::Float4& (kl::Float4::*)(const kl::Float4&)>(&kl::Float4::operator=), "=" },

		{ cs::fun<kl::Float2& (kl::Float4::*)()>(&kl::Float4::xy), "xy" },
		{ cs::fun<kl::Float2& (kl::Float4::*)()>(&kl::Float4::yz), "yz" },
		{ cs::fun<kl::Float2& (kl::Float4::*)()>(&kl::Float4::zw), "zw" },

		{ cs::fun<kl::Float3& (kl::Float4::*)()>(&kl::Float4::xyz), "xyz" },
		{ cs::fun<kl::Float3& (kl::Float4::*)()>(&kl::Float4::yzw), "yzw" },

		{ cs::fun<float& (kl::Float4::*)(int)>(&kl::Float4::operator[]), "[]"},

		{ cs::fun(&kl::Float4::operator==), "==" },
		{ cs::fun(&kl::Float4::operator!=), "!=" },

		{ cs::fun(&kl::Float4::operator+), "+" },
		{ cs::fun(&kl::Float4::operator+=), "+=" },

		{ cs::fun<kl::Float4 (kl::Float4::*)(const kl::Float4&) const>(&kl::Float4::operator-), "-"},
		{ cs::fun(&kl::Float4::operator-=), "-=" },

		{ cs::fun<kl::Float4 (kl::Float4::*)(float) const>(&kl::Float4::operator*), "*" },
		{ cs::fun<void (kl::Float4::*)(float)>(&kl::Float4::operator*=), "*=" },

		{ cs::fun<kl::Float4 (kl::Float4::*)(const kl::Float4&) const>(&kl::Float4::operator*), "*" },
		{ cs::fun<void (kl::Float4::*)(const kl::Float4&)>(&kl::Float4::operator*=), "*=" },

		{ cs::fun<kl::Float4 (kl::Float4::*)(float) const>(&kl::Float4::operator/), "/" },
		{ cs::fun<void (kl::Float4::*)(float)>(&kl::Float4::operator/=), "/=" },

		{ cs::fun<kl::Float4 (kl::Float4::*)(const kl::Float4&) const>(&kl::Float4::operator/), "/" },
		{ cs::fun<void (kl::Float4::*)(const kl::Float4&)>(&kl::Float4::operator/=), "/="},

		{ cs::fun<kl::Float4 (kl::Float4::*)() const>(&kl::Float4::operator-), "-"},
		{ cs::fun(&kl::Float4::length), "length" },
		
		{ cs::fun([](const kl::Float4& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float4"] = "Four component float vector.";
	INTERP_SCRIPT_MEMBERS["w"] = "w";
	INTERP_SCRIPT_IDENTIFIERS["zw"] = "Returns z and w as a Float2&.";
	INTERP_SCRIPT_IDENTIFIERS["xyz"] = "Returns x, y and z as a Float3&.";
	INTERP_SCRIPT_IDENTIFIERS["yzw"] = "Returns y, z and w as a Float3&.";

	// Float2x2
	cs::utility::add_class<kl::Float2x2>(*INTERP_SCRIPT_MODULE, "Float2x2",
	{
		cs::constructor<kl::Float2x2()>(),
		cs::constructor<kl::Float2x2(const kl::Float2x2&)>(),
	},
	{
		{ cs::fun<kl::Float2x2& (kl::Float2x2::*)(const kl::Float2x2&)>(&kl::Float2x2::operator=), "=" },

		{ cs::fun<float& (kl::Float2x2::*)(int)>(&kl::Float2x2::operator[]), "[]"},

		{ cs::fun(&kl::Float2x2::operator==), "==" },
		{ cs::fun(&kl::Float2x2::operator!=), "!=" },

		{ cs::fun(&kl::Float2x2::operator+), "+" },
		{ cs::fun(&kl::Float2x2::operator+=), "+=" },

		{ cs::fun(&kl::Float2x2::operator-), "-" },
		{ cs::fun(&kl::Float2x2::operator-=), "-=" },

		{ cs::fun<kl::Float2x2 (kl::Float2x2::*)(float) const>(&kl::Float2x2::operator*), "*" },
		{ cs::fun<void (kl::Float2x2::*)(float)>(&kl::Float2x2::operator*=), "*=" },

		{ cs::fun<kl::Float2x2 (kl::Float2x2::*)(const kl::Float2x2&) const>(&kl::Float2x2::operator*), "*" },
		{ cs::fun<void (kl::Float2x2::*)(const kl::Float2x2&)>(&kl::Float2x2::operator*=), "*=" },

		{ cs::fun<kl::Float2 (kl::Float2x2::*)(const kl::Float2&) const>(&kl::Float2x2::operator*), "*"},

		{ cs::fun(&kl::Float2x2::determinant), "determinant" },

		{ cs::fun([](const kl::Float2x2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float2x2"] = "Two float wide square matrix.";
	INTERP_SCRIPT_IDENTIFIERS["determinant"] = "Returns the determinant of this matrix.";

	// Float3x3
	cs::utility::add_class<kl::Float3x3>(*INTERP_SCRIPT_MODULE, "Float3x3",
	{
		cs::constructor<kl::Float3x3()>(),
		cs::constructor<kl::Float3x3(const kl::Float3x3&)>(),
	},
	{
		{ cs::fun<kl::Float3x3& (kl::Float3x3::*)(const kl::Float3x3&)>(&kl::Float3x3::operator=), "=" },

		{ cs::fun<float& (kl::Float3x3::*)(int)>(&kl::Float3x3::operator[]), "[]"},

		{ cs::fun(&kl::Float3x3::operator==), "==" },
		{ cs::fun(&kl::Float3x3::operator!=), "!=" },

		{ cs::fun(&kl::Float3x3::operator+), "+" },
		{ cs::fun(&kl::Float3x3::operator+=), "+=" },

		{ cs::fun(&kl::Float3x3::operator-), "-" },
		{ cs::fun(&kl::Float3x3::operator-=), "-=" },

		{ cs::fun<kl::Float3x3 (kl::Float3x3::*)(float) const>(&kl::Float3x3::operator*), "*" },
		{ cs::fun<void (kl::Float3x3::*)(float)>(&kl::Float3x3::operator*=), "*=" },

		{ cs::fun<kl::Float3x3 (kl::Float3x3::*)(const kl::Float3x3&) const>(&kl::Float3x3::operator*), "*" },
		{ cs::fun<void (kl::Float3x3::*)(const kl::Float3x3&)>(&kl::Float3x3::operator*=), "*=" },

		{ cs::fun<kl::Float3 (kl::Float3x3::*)(const kl::Float3&) const>(&kl::Float3x3::operator*), "*"},

		{ cs::fun(&kl::Float3x3::determinant), "determinant" },

		{ cs::fun([](const kl::Float3x3& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float3x3"] = "Three float wide square matrix.";

	// Float4x4
	cs::utility::add_class<kl::Float4x4>(*INTERP_SCRIPT_MODULE, "Float4x4",
	{
		cs::constructor<kl::Float4x4()>(),
		cs::constructor<kl::Float4x4(const kl::Float4x4&)>(),
	},
	{
		{ cs::fun<kl::Float4x4& (kl::Float4x4::*)(const kl::Float4x4&)>(&kl::Float4x4::operator=), "=" },

		{ cs::fun<float& (kl::Float4x4::*)(int)>(&kl::Float4x4::operator[]), "[]"},

		{ cs::fun(&kl::Float4x4::operator==), "==" },
		{ cs::fun(&kl::Float4x4::operator!=), "!=" },

		{ cs::fun(&kl::Float4x4::operator+), "+" },
		{ cs::fun(&kl::Float4x4::operator+=), "+=" },

		{ cs::fun(&kl::Float4x4::operator-), "-" },
		{ cs::fun(&kl::Float4x4::operator-=), "-=" },

		{ cs::fun<kl::Float4x4 (kl::Float4x4::*)(float) const>(&kl::Float4x4::operator*), "*" },
		{ cs::fun<void (kl::Float4x4::*)(float)>(&kl::Float4x4::operator*=), "*=" },

		{ cs::fun<kl::Float4x4 (kl::Float4x4::*)(const kl::Float4x4&) const>(&kl::Float4x4::operator*), "*" },
		{ cs::fun<void (kl::Float4x4::*)(const kl::Float4x4&)>(&kl::Float4x4::operator*=), "*=" },

		{ cs::fun<kl::Float4 (kl::Float4x4::*)(const kl::Float4&) const>(&kl::Float4x4::operator*), "*"},

		{ cs::fun(&kl::Float4x4::determinant), "determinant" },

		{ cs::fun([](const kl::Float4x4& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float4x4"] = "Four float wide square matrix.";

	// Complex
	cs::utility::add_class<kl::Complex<float>>(*INTERP_SCRIPT_MODULE, "Complex",
	{
		cs::constructor<kl::Complex<float>()>(),
		cs::constructor<kl::Complex<float>(const kl::Complex<float>&)>(),

		cs::constructor<kl::Complex<float>(float, float)>(),
		cs::constructor<kl::Complex<float>(float)>(),
	},
	{
		{ cs::fun(&kl::Complex<float>::r), "r" },
		{ cs::fun(&kl::Complex<float>::i), "i" },

		{ cs::fun<kl::Complex<float>& (kl::Complex<float>::*)(const kl::Complex<float>&)>(&kl::Complex<float>::operator=), "=" },

		{ cs::fun<float& (kl::Complex<float>::*)(int)>(&kl::Complex<float>::operator[]), "[]"},

		{ cs::fun(&kl::Complex<float>::operator==), "==" },
		{ cs::fun(&kl::Complex<float>::operator!=), "!=" },

		{ cs::fun(&kl::Complex<float>::operator+), "+" },
		{ cs::fun(&kl::Complex<float>::operator+=), "+=" },

		{ cs::fun<kl::Complex<float> (kl::Complex<float>::*)(const kl::Complex<float>&) const>(&kl::Complex<float>::operator-), "-" },
		{ cs::fun(&kl::Complex<float>::operator-=), "-=" },

		{ cs::fun<kl::Complex<float> (kl::Complex<float>::*)(float) const>(&kl::Complex<float>::operator*), "*" },
		{ cs::fun<void (kl::Complex<float>::*)(float)>(&kl::Complex<float>::operator*=), "*=" },

		{ cs::fun<kl::Complex<float> (kl::Complex<float>::*)(const kl::Complex<float>&) const>(&kl::Complex<float>::operator*), "*" },
		{ cs::fun<void (kl::Complex<float>::*)(const kl::Complex<float>&)>(&kl::Complex<float>::operator*=), "*="},

		{ cs::fun<kl::Complex<float> (kl::Complex<float>::*)() const>(&kl::Complex<float>::operator-), "-" },
		{ cs::fun(&kl::Complex<float>::length), "length" },

		{ cs::fun([](const kl::Complex<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Complex"] = "Two component imaginary number.";
	INTERP_SCRIPT_MEMBERS["r"] = "r";
	INTERP_SCRIPT_MEMBERS["i"] = "i";

	// Quaternion
	cs::utility::add_class<kl::Quaternion<float>>(*INTERP_SCRIPT_MODULE, "Quaternion",
	{
		cs::constructor<kl::Quaternion<float>()>(),
		cs::constructor<kl::Quaternion<float>(const kl::Quaternion<float>&)>(),

		cs::constructor<kl::Quaternion<float>(float, float, float)>(),
		cs::constructor<kl::Quaternion<float>(float, float, float, float)>(),
		cs::constructor<kl::Quaternion<float>(kl::Float3, float)>(),
	},
	{
		{ cs::fun(&kl::Quaternion<float>::w), "w" },
		{ cs::fun(&kl::Quaternion<float>::x), "x" },
		{ cs::fun(&kl::Quaternion<float>::y), "y" },
		{ cs::fun(&kl::Quaternion<float>::z), "z" },

		{ cs::fun<kl::Quaternion<float>& (kl::Quaternion<float>::*)(const kl::Quaternion<float>&)>(&kl::Quaternion<float>::operator=), "=" },

		{ cs::fun<float& (kl::Quaternion<float>::*)(int)>(&kl::Quaternion<float>::operator[]), "[]"},

		{ cs::fun(&kl::Quaternion<float>::operator==), "==" },
		{ cs::fun(&kl::Quaternion<float>::operator!=), "!=" },

		{ cs::fun(&kl::Quaternion<float>::operator+), "+" },
		{ cs::fun(&kl::Quaternion<float>::operator+=), "+=" },

		{ cs::fun<kl::Quaternion<float>(kl::Quaternion<float>::*)(const kl::Quaternion<float>&) const>(&kl::Quaternion<float>::operator-), "-" },
		{ cs::fun(&kl::Quaternion<float>::operator-=), "-=" },

		{ cs::fun<kl::Quaternion<float>(kl::Quaternion<float>::*)(float) const>(&kl::Quaternion<float>::operator*), "*" },
		{ cs::fun<void (kl::Quaternion<float>::*)(float)>(&kl::Quaternion<float>::operator*=), "*=" },

		{ cs::fun<kl::Quaternion<float> (kl::Quaternion<float>::*)(const kl::Quaternion<float>&) const>(&kl::Quaternion<float>::operator*), "*" },
		{ cs::fun<void (kl::Quaternion<float>::*)(const kl::Quaternion<float>&)>(&kl::Quaternion<float>::operator*=), "*=" },

		{ cs::fun<kl::Quaternion<float> (kl::Quaternion<float>::*)() const>(&kl::Quaternion<float>::operator-), "-" },
		{ cs::fun(&kl::Quaternion<float>::length), "length" },

		{ cs::fun([](const kl::Quaternion<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Quaternion"] = "Four component imaginary number.";

	// Vertex
	cs::utility::add_class<kl::Vertex<float>>(*INTERP_SCRIPT_MODULE, "Vertex",
	{
		cs::constructor<kl::Vertex<float>()>(),
		cs::constructor<kl::Vertex<float>(const kl::Vertex<float>&)>(),

		cs::constructor<kl::Vertex<float>(kl::Float3)>(),
		cs::constructor<kl::Vertex<float>(kl::Float3, kl::Float2)>(),
		cs::constructor<kl::Vertex<float>(kl::Float3, kl::Float3)>(),
		cs::constructor<kl::Vertex<float>(kl::Float3, kl::Float2, kl::Float3)>(),
	},
	{
		{ cs::fun(&kl::Vertex<float>::world), "world" },
		{ cs::fun(&kl::Vertex<float>::normal), "normal" },
		{ cs::fun(&kl::Vertex<float>::texture), "texture" },

		{ cs::fun<kl::Vertex<float>& (kl::Vertex<float>::*)(const kl::Vertex<float>&)>(&kl::Vertex<float>::operator=), "=" },

		{ cs::fun([](const kl::Vertex<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Vertex"] = "A 3D point of a mesh.";
	INTERP_SCRIPT_MEMBERS["world"] = "world";
	INTERP_SCRIPT_MEMBERS["normal"] = "normal";
	INTERP_SCRIPT_MEMBERS["texture"] = "texture";

	// Triangle
	cs::utility::add_class<kl::Triangle<float>>(*INTERP_SCRIPT_MODULE, "Triangle",
	{
		cs::constructor<kl::Triangle<float>()>(),
		cs::constructor<kl::Triangle<float>(const kl::Triangle<float>&)>(),

		cs::constructor<kl::Triangle<float>(kl::Vertex<float>, kl::Vertex<float>, kl::Vertex<float>)>(),
	},
	{
		{ cs::fun(&kl::Triangle<float>::a), "a" },
		{ cs::fun(&kl::Triangle<float>::b), "b" },
		{ cs::fun(&kl::Triangle<float>::c), "c" },

		{ cs::fun<kl::Triangle<float>& (kl::Triangle<float>::*)(const kl::Triangle<float>&)>(&kl::Triangle<float>::operator=), "=" },

		{ cs::fun([](const kl::Triangle<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Triangle"] = "A 3D triangle in space.";
	INTERP_SCRIPT_MEMBERS["a"] = "a";
	INTERP_SCRIPT_MEMBERS["b"] = "b";
	INTERP_SCRIPT_MEMBERS["c"] = "c";

	// Plane
	cs::utility::add_class<kl::Plane<float>>(*INTERP_SCRIPT_MODULE, "Plane",
	{
		cs::constructor<kl::Plane<float>()>(),
		cs::constructor<kl::Plane<float>(const kl::Plane<float>&)>(),

		cs::constructor<kl::Plane<float>(kl::Float3, kl::Float3)>(),
	},
	{
		{ cs::fun(&kl::Plane<float>::origin), "origin" },

		{ cs::fun<kl::Plane<float>& (kl::Plane<float>::*)(const kl::Plane<float>&)>(&kl::Plane<float>::operator=), "=" },

		{ cs::fun(&kl::Plane<float>::set_normal), "set_normal" },
		{ cs::fun(&kl::Plane<float>::normal), "normal" },

		{ cs::fun(&kl::Plane<float>::in_front), "in_front" },

		{ cs::fun([](const kl::Plane<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Plane"] = "A 3D plane in space.";
	INTERP_SCRIPT_MEMBERS["origin"] = "origin";
	INTERP_SCRIPT_IDENTIFIERS["set_normal"] = "Normalizes and sets the normal.";
	INTERP_SCRIPT_IDENTIFIERS["normal"] = "Returns the normalized normal.";
	INTERP_SCRIPT_IDENTIFIERS["in_front"] = "Checks if a 3D point is in front of the plane.";

	// Sphere
	cs::utility::add_class<kl::Sphere<float>>(*INTERP_SCRIPT_MODULE, "Sphere",
	{
		cs::constructor<kl::Sphere<float>()>(),
		cs::constructor<kl::Sphere<float>(const kl::Sphere<float>&)>(),

		cs::constructor<kl::Sphere<float>(kl::Float3, float)>(),
	},
	{
		{ cs::fun(&kl::Sphere<float>::origin), "origin" },
		{ cs::fun(&kl::Sphere<float>::radius), "radius" },

		{ cs::fun<kl::Sphere<float>& (kl::Sphere<float>::*)(const kl::Sphere<float>&)>(&kl::Sphere<float>::operator=), "=" },

		{ cs::fun([](const kl::Sphere<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Sphere"] = "A 3D sphere in space.";
	INTERP_SCRIPT_MEMBERS["radius"] = "radius";

	// Ray
	cs::utility::add_class<kl::Ray<float>>(*INTERP_SCRIPT_MODULE, "Ray",
	{
		cs::constructor<kl::Ray<float>()>(),
		cs::constructor<kl::Ray<float>(const kl::Ray<float>&)>(),

		cs::constructor<kl::Ray<float>(kl::Float3, kl::Float3)>(),
		cs::constructor<kl::Ray<float>(kl::Float3, kl::Float4x4, kl::Float2)>(),
	},
	{
		{ cs::fun(&kl::Ray<float>::origin), "origin" },

		{ cs::fun<kl::Ray<float>& (kl::Ray<float>::*)(const kl::Ray<float>&)>(&kl::Ray<float>::operator=), "=" },

		{ cs::fun(&kl::Ray<float>::set_direction), "set_direction" },
		{ cs::fun(&kl::Ray<float>::direction), "direction" },

		{ cs::fun(&kl::Ray<float>::intersect_plane), "intersect_plane" },
		{ cs::fun(&kl::Ray<float>::intersect_triangle), "intersect_triangle" },

		{ cs::fun(&kl::Ray<float>::can_intersect_sphere), "can_intersect_sphere" },
		{ cs::fun(&kl::Ray<float>::intersect_sphere), "intersect_sphere" },

		{ cs::fun([](const kl::Ray<float>& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Ray"] = "A 3D ray in space.";
	INTERP_SCRIPT_IDENTIFIERS["set_direction"] = "Sets the normalized direction.";
	INTERP_SCRIPT_IDENTIFIERS["direction"] = "Returns the normalized direction.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_plane"] = "Ray interesects plane.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_triangle"] = "Ray interesects triangle.";
	INTERP_SCRIPT_IDENTIFIERS["can_intersect_sphere"] = "Checks if this ray can intersect a sphere.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_sphere"] = "Ray interesects sphere.";

	// Color
	cs::utility::add_class<kl::Color>(*INTERP_SCRIPT_MODULE, "Color",
	{
		cs::constructor<kl::Color()>(),
		cs::constructor<kl::Color(const kl::Color&)>(),

		cs::constructor<kl::Color(byte, byte, byte)>(),
		cs::constructor<kl::Color(byte, byte, byte, byte)>(),
	},
	{
		{ cs::fun(&kl::Color::r), "r" },
		{ cs::fun(&kl::Color::g), "g" },
		{ cs::fun(&kl::Color::b), "b" },
		{ cs::fun(&kl::Color::a), "a" },

		{ cs::fun<kl::Color& (kl::Color::*)(const kl::Color&)>(&kl::Color::operator=), "=" },

		{ cs::fun(&kl::Color::operator==), "==" },
		{ cs::fun(&kl::Color::operator!=), "!=" },

		{ cs::fun(&kl::Color::gray), "gray" },
		{ cs::fun(&kl::Color::inverted), "inverted" },
		{ cs::fun(&kl::Color::as_ascii), "as_ascii" },

		{ cs::fun<kl::Color (kl::Color::*)(const kl::Color&, float) const>(&kl::Color::mix), "mix" },
		{ cs::fun<kl::Color (kl::Color::*)(const kl::Color&) const>(&kl::Color::mix), "mix" },

		{ cs::fun([](const kl::Color& object) {
			return kl::format('(', int(object.r),
				", ", int(object.g),
				", ", int(object.b),
				", ", int(object.a), ')');
		}), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Color"] = "Four component byte[0, 255] color.";
	INTERP_SCRIPT_MEMBERS["r"] = "r";
	INTERP_SCRIPT_MEMBERS["g"] = "g";
	INTERP_SCRIPT_MEMBERS["b"] = "b";
	INTERP_SCRIPT_MEMBERS["a"] = "a";
	INTERP_SCRIPT_IDENTIFIERS["gray"] = "Returns the color as gray.";
	INTERP_SCRIPT_IDENTIFIERS["inverted"] = "Inverts the color channels.";
	INTERP_SCRIPT_IDENTIFIERS["as_ascii"] = "Converts the color to closes ascii char.";
	INTERP_SCRIPT_IDENTIFIERS["mix"] = "Mixes two colors.";

	// Mesh
	cs::utility::add_class<Mesh>(*INTERP_SCRIPT_MODULE, "Mesh",
	{},
	{
		{ cs::fun(&Mesh::data_buffer), "data_buffer" },

		{ cs::fun(&Mesh::topology), "topology"},
		{ cs::fun(&Mesh::render_wireframe), "render_wireframe" },

		{ cs::fun(&Mesh::reload), "reload" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Mesh"] = "Object that contains triangle data.";
	INTERP_SCRIPT_MEMBERS["data_buffer"] = "data_buffer";
	INTERP_SCRIPT_MEMBERS["topology"] = "topology";
	INTERP_SCRIPT_MEMBERS["render_wireframe"] = "render_wireframe";
	INTERP_SCRIPT_IDENTIFIERS["reload"] = "Reloads the self.";

	// Texture
	cs::utility::add_class<Texture>(*INTERP_SCRIPT_MODULE, "Texture",
	{},
	{
		{ cs::fun(&Texture::data_buffer), "data_buffer" },

		{ cs::fun(&Texture::reload_as_2D), "reload_as_2D" },
		{ cs::fun(&Texture::reload_as_cube), "reload_as_cube" },

		{ cs::fun(&Texture::create_target_view), "create_target_view" },
		{ cs::fun(&Texture::create_depth_view), "create_depth_view" },
		{ cs::fun(&Texture::create_shader_view), "create_shader_view" },
		{ cs::fun(&Texture::create_access_view), "create_access_view" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Texture"] = "Object that contains pixel data.";
	INTERP_SCRIPT_IDENTIFIERS["reload_as_2D"] = "Reloads the texture as 2D.";
	INTERP_SCRIPT_IDENTIFIERS["reload_as_cube"] = "Reloads the texture as a cube map.";
	INTERP_SCRIPT_IDENTIFIERS["create_target_view"] = "Creates target view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_depth_view"] = "Creates depth view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_shader_view"] = "Creates shader view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_access_view"] = "Creates access view for this texture.";

	// Material
	cs::utility::add_class<Material>(*INTERP_SCRIPT_MODULE, "Material",
	{},
	{
		{ cs::fun(&Material::color), "color" },
		{ cs::fun(&Material::texture_blend), "texture_blend" },

		{ cs::fun(&Material::reflection_factor), "reflection_factor" },
		{ cs::fun(&Material::refraction_factor), "refraction_factor" },
		{ cs::fun(&Material::refraction_index), "refraction_index" },

		{ cs::fun(&Material::custom_data), "custom_data" },

		{ cs::fun(&Material::color_map_name), "color_map_name" },
		{ cs::fun(&Material::normal_map_name), "normal_map_name" },
		{ cs::fun(&Material::roughness_map_name), "roughness_map_name" },

		{ cs::fun(&Material::shader_name), "shader_name" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Material"] = "Object that defines the look of the entity.";
	INTERP_SCRIPT_MEMBERS["color"] = "color";
	INTERP_SCRIPT_MEMBERS["texture_blend"] = "texture_blend";
	INTERP_SCRIPT_MEMBERS["reflection_factor"] = "reflection_factor";
	INTERP_SCRIPT_MEMBERS["refraction_factor"] = "refraction_factor";
	INTERP_SCRIPT_MEMBERS["refraction_index"] = "refraction_index";
	INTERP_SCRIPT_MEMBERS["custom_data"] = "custom_data";
	INTERP_SCRIPT_MEMBERS["color_map_name"] = "color_map_name";
	INTERP_SCRIPT_MEMBERS["normal_map_name"] = "normal_map_name";
	INTERP_SCRIPT_MEMBERS["roughness_map_name"] = "roughness_map_name";
	INTERP_SCRIPT_MEMBERS["shader_name"] = "shader_name";

	// Shader
	cs::utility::add_class<Shader>(*INTERP_SCRIPT_MODULE, "Shader",
	{},
	{
		{ cs::fun(&Shader::type), "type" },
		{ cs::fun(&Shader::data_buffer), "data_buffer" },

		{ cs::fun(&Shader::reload), "reload" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Shader"] = "Custom shader override for a material.";
	INTERP_SCRIPT_MEMBERS["type"] = "Type of the object.";

	// Entity
	cs::utility::add_class<Entity>(*INTERP_SCRIPT_MODULE, "Entity",
	{},
	{
		{ cs::fun(&Entity::scale), "scale" },
		{ cs::fun(&Entity::casts_shadows), "casts_shadows" },

		{ cs::fun(&Entity::mesh_name), "mesh_name" },
		{ cs::fun(&Entity::material_name), "material_name" },

		{ cs::fun(&Entity::model_matrix), "model_matrix" },
		{ cs::fun(&Entity::collider_matrix), "collider_matrix" },

		{ cs::fun(&Entity::set_rotation), "set_rotation" },
		{ cs::fun(&Entity::rotation), "rotation" },

		{ cs::fun(&Entity::set_position), "set_position" },
		{ cs::fun(&Entity::position), "position" },

		{ cs::fun(&Entity::set_dynamic), "set_dynamic" },
		{ cs::fun(&Entity::is_dynamic), "is_dynamic" },

		{ cs::fun(&Entity::set_gravity), "set_gravity" },
		{ cs::fun(&Entity::has_gravity), "has_gravity" },

		{ cs::fun(&Entity::set_mass), "set_mass" },
		{ cs::fun(&Entity::mass), "mass" },

		{ cs::fun(&Entity::set_velocity), "set_velocity" },
		{ cs::fun(&Entity::velocity), "velocity" },

		{ cs::fun(&Entity::set_angular), "set_angular" },
		{ cs::fun(&Entity::angular), "angular" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Entity"] = "Base entity that's a part of a scene.";
	INTERP_SCRIPT_MEMBERS["scale"] = "scale";
	INTERP_SCRIPT_MEMBERS["casts_shadows"] = "casts_shadows";
	INTERP_SCRIPT_MEMBERS["mesh_name"] = "mesh_name";
	INTERP_SCRIPT_MEMBERS["material_name"] = "material_name";
	INTERP_SCRIPT_IDENTIFIERS["model_matrix"] = "Returns the model matrix of entity.";
	INTERP_SCRIPT_IDENTIFIERS["collider_matrix"] = "Returns the collider matrix of entity.";
	INTERP_SCRIPT_IDENTIFIERS["set_rotation"] = "Sets the entity rotation.";
	INTERP_SCRIPT_IDENTIFIERS["rotation"] = "Returns entity rotation.";
	INTERP_SCRIPT_IDENTIFIERS["set_position"] = "Sets the entity position.";
	INTERP_SCRIPT_IDENTIFIERS["position"] = "Returns the entity position.";
	INTERP_SCRIPT_IDENTIFIERS["set_dynamic"] = "Changes the dynamic state of entity.";
	INTERP_SCRIPT_IDENTIFIERS["is_dynamic"] = "Returns the dynamic state of entity.";
	INTERP_SCRIPT_IDENTIFIERS["set_gravity"] = "Sets the gravity state.";
	INTERP_SCRIPT_IDENTIFIERS["has_gravity"] = "Returns the gravity state.";
	INTERP_SCRIPT_IDENTIFIERS["set_mass"] = "Sets the mass of entity.";
	INTERP_SCRIPT_IDENTIFIERS["mass"] = "Returns the mass of entity.";
	INTERP_SCRIPT_IDENTIFIERS["set_velocity"] = "Sets the directional velocity of entity.";
	INTERP_SCRIPT_IDENTIFIERS["velocity"] = "Returns the directional velocity of entity.";
	INTERP_SCRIPT_IDENTIFIERS["set_angular"] = "Sets the angular velocity of entity.";
	INTERP_SCRIPT_IDENTIFIERS["angular"] = "Returns the angular velocity of entity.";

	// Camera
	cs::utility::add_class<Camera>(*INTERP_SCRIPT_MODULE, "Camera",
	{},
	{
		{ cs::fun(&Camera::type), "type" },

		{ cs::fun(&Camera::aspect_ratio), "aspect_ratio" },
		{ cs::fun(&Camera::field_of_view), "field_of_view" },
		{ cs::fun(&Camera::width), "width" },
		{ cs::fun(&Camera::height), "height" },
		{ cs::fun(&Camera::near_plane), "near_plane" },
		{ cs::fun(&Camera::far_plane), "far_plane" },
		{ cs::fun(&Camera::sensitivity), "sensitivity" },
		{ cs::fun(&Camera::speed), "speed" },
		{ cs::fun(&Camera::background), "background" },
		{ cs::fun(&Camera::custom_data), "custom_data" },
		{ cs::fun(&Camera::skybox_name), "skybox_name" },
		{ cs::fun(&Camera::shader_name), "shader_name" },

		{ cs::fun(&Camera::update_aspect_ratio), "update_aspect_ratio" },

		{ cs::fun(&Camera::set_forward), "set_forward" },
		{ cs::fun(&Camera::forward), "forward" },

		{ cs::fun(&Camera::set_up), "set_up" },
		{ cs::fun(&Camera::up), "up" },

		{ cs::fun(&Camera::right), "right" },

		{ cs::fun(&Camera::move_forward), "move_forward" },
		{ cs::fun(&Camera::move_back), "move_back" },
		{ cs::fun(&Camera::move_right), "move_right" },
		{ cs::fun(&Camera::move_left), "move_left" },
		{ cs::fun(&Camera::move_up), "move_up" },
		{ cs::fun(&Camera::move_down), "move_down" },

		{ cs::fun(&Camera::rotate), "rotate" },

		{ cs::fun(&Camera::view_matrix), "view_matrix" },
		{ cs::fun(&Camera::projection_matrix), "projection_matrix" },
		{ cs::fun(&Camera::camera_matrix), "camera_matrix" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Camera"] = "Entity that has a view of the scene.";
	INTERP_SCRIPT_MEMBERS["aspect_ratio"] = "aspect_ratio";
	INTERP_SCRIPT_MEMBERS["field_of_view"] = "field_of_view";
	INTERP_SCRIPT_MEMBERS["width"] = "width";
	INTERP_SCRIPT_MEMBERS["height"] = "height";
	INTERP_SCRIPT_MEMBERS["near_plane"] = "near_plane";
	INTERP_SCRIPT_MEMBERS["far_plane"] = "far_plane";
	INTERP_SCRIPT_MEMBERS["sensitivity"] = "sensitivity";
	INTERP_SCRIPT_MEMBERS["speed"] = "speed";
	INTERP_SCRIPT_MEMBERS["background"] = "background";
	INTERP_SCRIPT_MEMBERS["custom_data"] = "custom_data";
	INTERP_SCRIPT_MEMBERS["skybox_name"] = "skybox_name";
	INTERP_SCRIPT_MEMBERS["shader_name"] = "shader_name";
	INTERP_SCRIPT_IDENTIFIERS["update_aspect_ratio"] = "Sets the camera's aspect ratio.";
	INTERP_SCRIPT_IDENTIFIERS["set_forward"] = "Sets the normalized forward vector of camera.";
	INTERP_SCRIPT_IDENTIFIERS["forward"] = "Returns the normalized forward vector of camera.";
	INTERP_SCRIPT_IDENTIFIERS["set_up"] = "Sets the normalized up vector of camera.";
	INTERP_SCRIPT_IDENTIFIERS["up"] = "Returns the normalized up vector of camera.";
	INTERP_SCRIPT_IDENTIFIERS["right"] = "Returns the normalized right vector of camera.";
	INTERP_SCRIPT_IDENTIFIERS["move_forward"] = "Moves the camera forward.";
	INTERP_SCRIPT_IDENTIFIERS["move_back"] = "Moves the camera back.";
	INTERP_SCRIPT_IDENTIFIERS["move_right"] = "Moves the camera right.";
	INTERP_SCRIPT_IDENTIFIERS["move_left"] = "Moves the camera left.";
	INTERP_SCRIPT_IDENTIFIERS["move_up"] = "Moves the camera up.";
	INTERP_SCRIPT_IDENTIFIERS["move_down"] = "Moves the camera down.";
	INTERP_SCRIPT_IDENTIFIERS["rotate"] = "Handles the camera rotation with a mouse.";
	INTERP_SCRIPT_IDENTIFIERS["view_matrix"] = "Returns the camera's view matrix.";
	INTERP_SCRIPT_IDENTIFIERS["projection_matrix"] = "Returns the camera's projection matrix.";
	INTERP_SCRIPT_IDENTIFIERS["camera_matrix"] = "Returns the camera's matrix.";

	// Light
	cs::utility::add_class<Light>(*INTERP_SCRIPT_MODULE, "Light",
	{},
	{
		{ cs::fun(&Light::light_at_point), "light_at_point" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Light"] = "Base class of every light class.";
	INTERP_SCRIPT_IDENTIFIERS["light_at_point"] = "Returns the lights intensity at some 3D point.";

	// Ambient light
	cs::utility::add_class<AmbientLight>(*INTERP_SCRIPT_MODULE, "AmbientLight",
	{},
	{
		{ cs::fun(&AmbientLight::color), "color" },
		{ cs::fun(&AmbientLight::intensity), "intensity" },
	});
	INTERP_SCRIPT_IDENTIFIERS["AmbientLight"] = "Non directional light at shines at all points.";
	INTERP_SCRIPT_MEMBERS["color"] = "color";
	INTERP_SCRIPT_MEMBERS["intensity"] = "intensity";

	// Point light
	cs::utility::add_class<PointLight>(*INTERP_SCRIPT_MODULE, "PointLight",
	{},
	{
		{ cs::fun(&PointLight::color), "color" },
	});
	INTERP_SCRIPT_IDENTIFIERS["PointLight"] = "Light that shines in all directions.";

	// Directional light
	cs::utility::add_class<DirectionalLight>(*INTERP_SCRIPT_MODULE, "DirectionalLight",
	{},
	{
		{ cs::fun(&DirectionalLight::point_size), "point_size" },
		{ cs::fun(&DirectionalLight::color), "color" },

		{ cs::fun(&DirectionalLight::map_resolution), "map_resolution" },

		{ cs::fun(&DirectionalLight::set_direction), "set_direction" },
		{ cs::fun(&DirectionalLight::direction), "direction" },

		{ cs::fun(&DirectionalLight::light_matrix), "light_matrix" },
	});
	INTERP_SCRIPT_IDENTIFIERS["DirectionalLight"] = "Light that shines in a single direction and is infinitely far away.";
	INTERP_SCRIPT_MEMBERS["point_size"] = "point_size";
	INTERP_SCRIPT_IDENTIFIERS["map_resolution"] = "Returns the shadow map resolution of directional light.";
	INTERP_SCRIPT_IDENTIFIERS["light_matrix"] = "Returns the light matrix.";

	// Scene
	cs::utility::add_class<Scene>(*INTERP_SCRIPT_MODULE, "Scene",
	{},
	{
		{ cs::fun(&Scene::main_camera_name), "main_camera_name" },
		{ cs::fun(&Scene::main_ambient_light_name), "main_ambient_light_name" },
		{ cs::fun(&Scene::main_directional_light_name), "main_directional_light_name" },

		{ cs::fun(&Scene::set_gravity), "set_gravity" },
		{ cs::fun(&Scene::gravity), "gravity" },

		{ cs::fun(&Scene::helper_new_mesh), "new_mesh" },
		{ cs::fun(&Scene::helper_new_texture), "new_texture" },
		{ cs::fun(&Scene::helper_new_material), "new_material" },
		{ cs::fun(&Scene::helper_new_shader), "new_shader" },
		{ cs::fun(&Scene::helper_new_entity), "new_entity" },

		{ cs::fun(&Scene::helper_get_mesh), "get_mesh" },
		{ cs::fun(&Scene::helper_get_texture), "get_texture" },
		{ cs::fun(&Scene::helper_get_material), "get_material" },
		{ cs::fun(&Scene::helper_get_shader), "get_shader" },
		{ cs::fun(&Scene::helper_get_entity), "get_entity" },

		{ cs::fun(&Scene::helper_remove_mesh), "remove_mesh" },
		{ cs::fun(&Scene::helper_remove_texture), "remove_texture" },
		{ cs::fun(&Scene::helper_remove_material), "remove_material" },
		{ cs::fun(&Scene::helper_remove_shader), "remove_shader" },
		{ cs::fun(&Scene::helper_remove_entity), "remove_entity" },

		{ cs::fun(&Scene::helper_contains_mesh), "contains_mesh" },
		{ cs::fun(&Scene::helper_contains_texture), "contains_texture" },
		{ cs::fun(&Scene::helper_contains_material), "contains_material" },
		{ cs::fun(&Scene::helper_contains_shader), "contains_shader" },
		{ cs::fun(&Scene::helper_contains_entity), "contains_entity" },

		{ cs::fun(&Scene::helper_mesh_count), "mesh_count" },
		{ cs::fun(&Scene::helper_texture_count), "texture_count" },
		{ cs::fun(&Scene::helper_material_count), "material_count" },
		{ cs::fun(&Scene::helper_shader_count), "shader_count" },
		{ cs::fun(&Scene::helper_entity_count), "entity_count" },

		{ cs::fun(&Scene::helper_get_all_meshes), "get_all_meshes" },
		{ cs::fun(&Scene::helper_get_all_textures), "get_all_textures" },
		{ cs::fun(&Scene::helper_get_all_materials), "get_all_materials" },
		{ cs::fun(&Scene::helper_get_all_shaders), "get_all_shaders" },
		{ cs::fun(&Scene::helper_get_all_entities), "get_all_entities" },

		{ cs::fun<Camera* (Scene::*)(const std::string&)>(&Scene::get_casted<Camera>), "get_camera"},
		{ cs::fun<Light* (Scene::*)(const std::string&)>(&Scene::get_casted<Light>), "get_light" },
		{ cs::fun<AmbientLight* (Scene::*)(const std::string&)>(&Scene::get_casted<AmbientLight>), "get_ambient_light" },
		{ cs::fun<PointLight* (Scene::*)(const std::string&)>(&Scene::get_casted<PointLight>), "get_point_light" },
		{ cs::fun<DirectionalLight* (Scene::*)(const std::string&)>(&Scene::get_casted<DirectionalLight>), "get_directional_light" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Scene"] = "Collection of meshes, textures, materials, scripts and entities.";
	INTERP_SCRIPT_MEMBERS["main_camera_name"] = "main_camera_name";
	INTERP_SCRIPT_MEMBERS["main_ambient_light_name"] = "main_ambient_light_name";
	INTERP_SCRIPT_MEMBERS["main_directional_light_name"] = "main_directional_light_name";
	INTERP_SCRIPT_IDENTIFIERS["new_mesh"] = "Creates a new mesh in scene.";
	INTERP_SCRIPT_IDENTIFIERS["new_texture"] = "Creates a new texture in scene.";
	INTERP_SCRIPT_IDENTIFIERS["new_material"] = "Creates a new material in scene.";
	INTERP_SCRIPT_IDENTIFIERS["new_shader"] = "Creates a new shader in scene.";
	INTERP_SCRIPT_IDENTIFIERS["new_entity"] = "Creates a new entity in scene.";
	INTERP_SCRIPT_IDENTIFIERS["get_mesh"] = "Returns a scene mesh.";
	INTERP_SCRIPT_IDENTIFIERS["get_texture"] = "Returns a scene texture.";
	INTERP_SCRIPT_IDENTIFIERS["get_material"] = "Returns a scene material.";
	INTERP_SCRIPT_IDENTIFIERS["get_shader"] = "Returns a scene shader.";
	INTERP_SCRIPT_IDENTIFIERS["get_entity"] = "Returns a scene entity.";
	INTERP_SCRIPT_IDENTIFIERS["remove_mesh"] = "Removes a mesh from scene.";
	INTERP_SCRIPT_IDENTIFIERS["remove_texture"] = "Removes a texture from scene.";
	INTERP_SCRIPT_IDENTIFIERS["remove_material"] = "Removes a material from scene.";
	INTERP_SCRIPT_IDENTIFIERS["remove_shader"] = "Removes a shader from scene.";
	INTERP_SCRIPT_IDENTIFIERS["remove_entity"] = "Removes an entity from scene.";
	INTERP_SCRIPT_IDENTIFIERS["contains_mesh"] = "Returns true if scene contains mesh.";
	INTERP_SCRIPT_IDENTIFIERS["contains_texture"] = "Returns true if scene contains texture.";
	INTERP_SCRIPT_IDENTIFIERS["contains_material"] = "Returns true if scene contains material.";
	INTERP_SCRIPT_IDENTIFIERS["contains_shader"] = "Returns true if scene contains shader.";
	INTERP_SCRIPT_IDENTIFIERS["contains_entity"] = "Returns true if scene contains entity.";
	INTERP_SCRIPT_IDENTIFIERS["mesh_count"] = "Returns the integer count of meshes in scene.";
	INTERP_SCRIPT_IDENTIFIERS["texture_count"] = "Returns the integer count of texture in scene.";
	INTERP_SCRIPT_IDENTIFIERS["material_count"] = "Returns the integer count of materials in scene.";
	INTERP_SCRIPT_IDENTIFIERS["shader_count"] = "Returns the integer count of shaders in scene.";
	INTERP_SCRIPT_IDENTIFIERS["entity_count"] = "Returns the integer count of entities in scene.";
	INTERP_SCRIPT_IDENTIFIERS["get_all_meshes"] = "Returns a map of all scene meshes.";
	INTERP_SCRIPT_IDENTIFIERS["get_all_textures"] = "Returns a map of all scene textures.";
	INTERP_SCRIPT_IDENTIFIERS["get_all_materials"] = "Returns a map of all scene materials.";
	INTERP_SCRIPT_IDENTIFIERS["get_all_shaders"] = "Returns a map of all scene shaders.";
	INTERP_SCRIPT_IDENTIFIERS["get_all_entities"] = "Returns a map of all scene entities.";
	INTERP_SCRIPT_IDENTIFIERS["get_camera"] = "Returns a scene camera.";
	INTERP_SCRIPT_IDENTIFIERS["get_light"] = "Returns a scene light.";
	INTERP_SCRIPT_IDENTIFIERS["get_ambient_light"] = "Returns a scene ambient light.";
	INTERP_SCRIPT_IDENTIFIERS["get_point_light"] = "Returns a scene point light.";
	INTERP_SCRIPT_IDENTIFIERS["get_directional_light"] = "Returns a scene directional light.";

	// Key
	cs::utility::add_class<kl::Key>(*INTERP_SCRIPT_MODULE, "Key",
	{},
	{
		{ cs::fun(&kl::Key::is_down), "is_down" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Key"] = "Class representing a single keyboard or a mouse button.";
	INTERP_SCRIPT_IDENTIFIERS["is_down"] = "Returns the down state of key.";

	// Keyboard
	cs::utility::add_class<kl::Keyboard>(*INTERP_SCRIPT_MODULE, "Keyboard",
	{},
	{
		{ cs::fun(&kl::Keyboard::q), "q" },
		{ cs::fun(&kl::Keyboard::w), "w" },
		{ cs::fun(&kl::Keyboard::e), "e" },
		{ cs::fun(&kl::Keyboard::r), "r" },
		{ cs::fun(&kl::Keyboard::t), "t" },
		{ cs::fun(&kl::Keyboard::z), "z" },
		{ cs::fun(&kl::Keyboard::u), "u" },
		{ cs::fun(&kl::Keyboard::i), "i" },
		{ cs::fun(&kl::Keyboard::o), "o" },
		{ cs::fun(&kl::Keyboard::p), "p" },
		{ cs::fun(&kl::Keyboard::a), "a" },
		{ cs::fun(&kl::Keyboard::s), "s" },
		{ cs::fun(&kl::Keyboard::d), "d" },
		{ cs::fun(&kl::Keyboard::f), "f" },
		{ cs::fun(&kl::Keyboard::g), "g" },
		{ cs::fun(&kl::Keyboard::h), "h" },
		{ cs::fun(&kl::Keyboard::j), "j" },
		{ cs::fun(&kl::Keyboard::k), "k" },
		{ cs::fun(&kl::Keyboard::l), "l" },
		{ cs::fun(&kl::Keyboard::y), "y" },
		{ cs::fun(&kl::Keyboard::x), "x" },
		{ cs::fun(&kl::Keyboard::c), "c" },
		{ cs::fun(&kl::Keyboard::v), "v" },
		{ cs::fun(&kl::Keyboard::b), "b" },
		{ cs::fun(&kl::Keyboard::n), "n" },
		{ cs::fun(&kl::Keyboard::m), "m" },

		{ cs::fun(&kl::Keyboard::num0), "num0" },
		{ cs::fun(&kl::Keyboard::num1), "num1" },
		{ cs::fun(&kl::Keyboard::num2), "num2" },
		{ cs::fun(&kl::Keyboard::num3), "num3" },
		{ cs::fun(&kl::Keyboard::num4), "num4" },
		{ cs::fun(&kl::Keyboard::num5), "num5" },
		{ cs::fun(&kl::Keyboard::num6), "num6" },
		{ cs::fun(&kl::Keyboard::num7), "num7" },
		{ cs::fun(&kl::Keyboard::num8), "num8" },
		{ cs::fun(&kl::Keyboard::num9), "num9" },

		{ cs::fun(&kl::Keyboard::period), "period" },
		{ cs::fun(&kl::Keyboard::comma), "comma" },
		{ cs::fun(&kl::Keyboard::plus), "plus" },
		{ cs::fun(&kl::Keyboard::minus), "minus" },

		{ cs::fun(&kl::Keyboard::esc), "esc" },
		{ cs::fun(&kl::Keyboard::tab), "tab" },
		{ cs::fun(&kl::Keyboard::caps), "caps" },
		{ cs::fun(&kl::Keyboard::shift), "shift" },
		{ cs::fun(&kl::Keyboard::ctrl), "ctrl" },
		{ cs::fun(&kl::Keyboard::alt), "alt" },
		{ cs::fun(&kl::Keyboard::space), "space" },
		{ cs::fun(&kl::Keyboard::enter), "enter" },
		{ cs::fun(&kl::Keyboard::insert), "insert" },
		{ cs::fun(&kl::Keyboard::delet), "delete" },

		{ cs::fun(&kl::Keyboard::up), "up" },
		{ cs::fun(&kl::Keyboard::left), "left" },
		{ cs::fun(&kl::Keyboard::down), "down" },
		{ cs::fun(&kl::Keyboard::right), "right" },

		{ cs::fun(&kl::Keyboard::f1), "f1" },
		{ cs::fun(&kl::Keyboard::f2), "f2" },
		{ cs::fun(&kl::Keyboard::f3), "f3" },
		{ cs::fun(&kl::Keyboard::f4), "f4" },
		{ cs::fun(&kl::Keyboard::f5), "f5" },
		{ cs::fun(&kl::Keyboard::f6), "f6" },
		{ cs::fun(&kl::Keyboard::f7), "f7" },
		{ cs::fun(&kl::Keyboard::f8), "f8" },
		{ cs::fun(&kl::Keyboard::f9), "f9" },
		{ cs::fun(&kl::Keyboard::f10), "f10" },
		{ cs::fun(&kl::Keyboard::f11), "f11" },
		{ cs::fun(&kl::Keyboard::f12), "f12" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Keyboard"] = "Class representing a physical keyboard.";
	INTERP_SCRIPT_MEMBERS["q"] = "q";
	INTERP_SCRIPT_MEMBERS["w"] = "w";
	INTERP_SCRIPT_MEMBERS["e"] = "e";
	INTERP_SCRIPT_MEMBERS["r"] = "r";
	INTERP_SCRIPT_MEMBERS["t"] = "t";
	INTERP_SCRIPT_MEMBERS["z"] = "z";
	INTERP_SCRIPT_MEMBERS["u"] = "u";
	INTERP_SCRIPT_MEMBERS["i"] = "i";
	INTERP_SCRIPT_MEMBERS["o"] = "o";
	INTERP_SCRIPT_MEMBERS["p"] = "p";
	INTERP_SCRIPT_MEMBERS["a"] = "a";
	INTERP_SCRIPT_MEMBERS["s"] = "s";
	INTERP_SCRIPT_MEMBERS["d"] = "d";
	INTERP_SCRIPT_MEMBERS["f"] = "f";
	INTERP_SCRIPT_MEMBERS["g"] = "g";
	INTERP_SCRIPT_MEMBERS["h"] = "h";
	INTERP_SCRIPT_MEMBERS["j"] = "j";
	INTERP_SCRIPT_MEMBERS["k"] = "k";
	INTERP_SCRIPT_MEMBERS["l"] = "l";
	INTERP_SCRIPT_MEMBERS["y"] = "y";
	INTERP_SCRIPT_MEMBERS["x"] = "x";
	INTERP_SCRIPT_MEMBERS["c"] = "c";
	INTERP_SCRIPT_MEMBERS["v"] = "v";
	INTERP_SCRIPT_MEMBERS["b"] = "b";
	INTERP_SCRIPT_MEMBERS["n"] = "n";
	INTERP_SCRIPT_MEMBERS["m"] = "m";
	INTERP_SCRIPT_MEMBERS["num0"] = "num0";
	INTERP_SCRIPT_MEMBERS["num1"] = "num1";
	INTERP_SCRIPT_MEMBERS["num2"] = "num2";
	INTERP_SCRIPT_MEMBERS["num3"] = "num3";
	INTERP_SCRIPT_MEMBERS["num4"] = "num4";
	INTERP_SCRIPT_MEMBERS["num5"] = "num5";
	INTERP_SCRIPT_MEMBERS["num6"] = "num6";
	INTERP_SCRIPT_MEMBERS["num7"] = "num7";
	INTERP_SCRIPT_MEMBERS["num8"] = "num8";
	INTERP_SCRIPT_MEMBERS["num9"] = "num9";
	INTERP_SCRIPT_MEMBERS["period"] = "period";
	INTERP_SCRIPT_MEMBERS["comma"] = "comma";
	INTERP_SCRIPT_MEMBERS["plus"] = "plus";
	INTERP_SCRIPT_MEMBERS["minus"] = "minus";
	INTERP_SCRIPT_MEMBERS["esc"] = "esc";
	INTERP_SCRIPT_MEMBERS["tab"] = "tab";
	INTERP_SCRIPT_MEMBERS["caps"] = "caps";
	INTERP_SCRIPT_MEMBERS["shift"] = "shift";
	INTERP_SCRIPT_MEMBERS["ctrl"] = "ctrl";
	INTERP_SCRIPT_MEMBERS["alt"] = "alt";
	INTERP_SCRIPT_MEMBERS["space"] = "space";
	INTERP_SCRIPT_MEMBERS["enter"] = "enter";
	INTERP_SCRIPT_MEMBERS["insert"] = "insert";
	INTERP_SCRIPT_MEMBERS["delet"] = "delet";
	INTERP_SCRIPT_MEMBERS["up"] = "up";
	INTERP_SCRIPT_MEMBERS["left"] = "left";
	INTERP_SCRIPT_MEMBERS["down"] = "down";
	INTERP_SCRIPT_MEMBERS["right"] = "right";
	INTERP_SCRIPT_MEMBERS["f1"] = "f1";
	INTERP_SCRIPT_MEMBERS["f2"] = "f2";
	INTERP_SCRIPT_MEMBERS["f3"] = "f3";
	INTERP_SCRIPT_MEMBERS["f4"] = "f4";
	INTERP_SCRIPT_MEMBERS["f5"] = "f5";
	INTERP_SCRIPT_MEMBERS["f6"] = "f6";
	INTERP_SCRIPT_MEMBERS["f7"] = "f7";
	INTERP_SCRIPT_MEMBERS["f8"] = "f8";
	INTERP_SCRIPT_MEMBERS["f9"] = "f9";
	INTERP_SCRIPT_MEMBERS["f10"] = "f10";
	INTERP_SCRIPT_MEMBERS["f11"] = "f11";
	INTERP_SCRIPT_MEMBERS["f12"] = "f12";

	// Mouse
	cs::utility::add_class<kl::Mouse>(*INTERP_SCRIPT_MODULE, "Mouse",
	{},
	{
		{ cs::fun(&kl::Mouse::left), "left" },
		{ cs::fun(&kl::Mouse::middle), "middle" },
		{ cs::fun(&kl::Mouse::right), "right" },

		{ cs::fun(&kl::Mouse::set_hidden), "set_hiden" },
		{ cs::fun(&kl::Mouse::is_hidden), "is_hidden" },

		{ cs::fun(&kl::Mouse::set_position), "set_position" },
		{ cs::fun(&kl::Mouse::position), "position" },

		{ cs::fun(&kl::Mouse::normalized_position), "normalized_position" },
		{ cs::fun(&kl::Mouse::scroll), "scroll" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Mouse"] = "Class representing a physical mouse.";
	INTERP_SCRIPT_MEMBERS["left"] = "left";
	INTERP_SCRIPT_MEMBERS["middle"] = "middle";
	INTERP_SCRIPT_MEMBERS["right"] = "right";
	INTERP_SCRIPT_IDENTIFIERS["set_hidden"] = "Sets the mouse hidden state.";
	INTERP_SCRIPT_IDENTIFIERS["is_hidden"] = "Returns the mouse hidden state.";
	INTERP_SCRIPT_IDENTIFIERS["normalized_position"] = "Returns the normalized position.";
	INTERP_SCRIPT_IDENTIFIERS["scroll"] = "Returns the mouse scroll value.";

	// Window
	cs::utility::add_class<kl::Window>(*INTERP_SCRIPT_MODULE, "Window",
	{},
	{
		{ cs::fun(&kl::Window::is_open), "is_open" },
		{ cs::fun(&kl::Window::close), "close" },
		{ cs::fun(&kl::Window::is_resizeable), "is_resizeable" },
		{ cs::fun(&kl::Window::set_resizeable), "set_resizeable" },
		{ cs::fun(&kl::Window::maximize), "maximize" },
		{ cs::fun(&kl::Window::minimize), "minimize" },
		{ cs::fun(&kl::Window::restore), "restore" },
		{ cs::fun(&kl::Window::in_fullscreen), "in_fullscreen" },
		{ cs::fun(&kl::Window::set_fullscreen), "set_fullscreen" },
		{ cs::fun(&kl::Window::position), "position" },
		{ cs::fun(&kl::Window::set_position), "set_position" },
		{ cs::fun(&kl::Window::width), "width" },
		{ cs::fun(&kl::Window::set_width), "set_width" },
		{ cs::fun(&kl::Window::height), "height" },
		{ cs::fun(&kl::Window::set_height), "set_height" },
		{ cs::fun(&kl::Window::size), "size" },
		{ cs::fun(&kl::Window::resize), "resize" },
		{ cs::fun(&kl::Window::aspect_ratio), "aspect_ratio" },
		{ cs::fun(&kl::Window::frame_center), "frame_center" },
		{ cs::fun(&kl::Window::set_title), "set_title" },
		{ cs::fun(&kl::Window::set_icon), "set_icon" },
	});
	INTERP_SCRIPT_IDENTIFIERS["is_open"] = "Returns true if the window is open, false otherwise.";
	INTERP_SCRIPT_IDENTIFIERS["close"] = "Closes the window.";
	INTERP_SCRIPT_IDENTIFIERS["is_resizeable"] = "Returns true if the window is resizeable, false otherwise.";
	INTERP_SCRIPT_IDENTIFIERS["set_resizeable"] = "Enables or disables window resizing feature.";
	INTERP_SCRIPT_IDENTIFIERS["maximize"] = "Maximizes the window.";
	INTERP_SCRIPT_IDENTIFIERS["minimize"] = "Minimizes the window.";
	INTERP_SCRIPT_IDENTIFIERS["restore"] = "Restores the window.";
	INTERP_SCRIPT_IDENTIFIERS["in_fullscreen"] = "Returns true if the window is in fullscreen mode, false otherwise.";
	INTERP_SCRIPT_IDENTIFIERS["set_fullscreen"] = "Enables or disables window fullscreen mode.";
	INTERP_SCRIPT_IDENTIFIERS["position"] = "Returns the window position on screen as Int2.";
	INTERP_SCRIPT_IDENTIFIERS["set_position"] = "Sets the window position on screen.";
	INTERP_SCRIPT_IDENTIFIERS["width"] = "Returns the window width. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["set_width"] = "Sets the window width. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["height"] = "Returns the window height. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["set_height"] = "Sets the window height. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["size"] = "Returns the window size. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["resize"] = "Sets the window size. (true for client area, false for full window)";
	INTERP_SCRIPT_IDENTIFIERS["aspect_ratio"] = "Returns the window aspect ratio. (width / height)";
	INTERP_SCRIPT_IDENTIFIERS["frame_center"] = "Returns the window center point as Int2.";
	INTERP_SCRIPT_IDENTIFIERS["set_title"] = "Sets the window title.";
	INTERP_SCRIPT_IDENTIFIERS["set_icon"] = "Sets the window icon by passing a path to the icon file.";

	return 0;
}();

const int load_constants = [&]
{
	using namespace titian;

	// Numbers
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((float) kl::PI), "PI");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((float) kl::TO_RADIANS), "TO_RADIANS");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((float) kl::TO_DEGREES), "TO_DEGREES");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((float) kl::TO_FLOAT_COLOR), "TO_FLOAT_COLOR");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((float) kl::TO_BYTE_COLOR), "TO_BYTE_COLOR");
	INTERP_SCRIPT_IDENTIFIERS["PI"] = "Constant of a number PI (3.1415...).";
	INTERP_SCRIPT_IDENTIFIERS["TO_RADIANS"] = "Constant for converting degrees to radians (PI / 180).";
	INTERP_SCRIPT_IDENTIFIERS["TO_DEGREES"] = "Constant for converting degrees to radians (180 / PI).";
	INTERP_SCRIPT_IDENTIFIERS["TO_FLOAT_COLOR"] = "Constant for converting byte color to float color (1 / 255).";
	INTERP_SCRIPT_IDENTIFIERS["TO_BYTE_COLOR"] = "Constant for converting float color to byte color (255 / 1).";

	// Colors
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::CONSOLE), "CONSOLE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::BLACK), "BLACK");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::WHITE), "WHITE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::GRAY), "GRAY");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::RED), "RED");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::GREEN), "GREEN");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::BLUE), "BLUE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::CYAN), "CYAN");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::PURPLE), "PURPLE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::YELLOW), "YELLOW");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::ORANGE), "ORANGE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::MAGENTA), "MAGENTA");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::CRIMSON), "CRIMSON");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::WHEAT), "WHEAT");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((kl::Color) kl::colors::SKY), "SKY");
	INTERP_SCRIPT_IDENTIFIERS["CONSOLE"] = "Constant color (204, 204, 204).";
	INTERP_SCRIPT_IDENTIFIERS["BLACK"] = "Constant color (0, 0, 0).";
	INTERP_SCRIPT_IDENTIFIERS["WHITE"] = "Constant color (255, 255, 255).";
	INTERP_SCRIPT_IDENTIFIERS["GRAY"] = "Constant color (50, 50, 50).";
	INTERP_SCRIPT_IDENTIFIERS["RED"] = "Constant color (255, 0, 0).";
	INTERP_SCRIPT_IDENTIFIERS["GREEN"] = "Constant color (0, 255, 0).";
	INTERP_SCRIPT_IDENTIFIERS["BLUE"] = "Constant color (0, 0, 255).";
	INTERP_SCRIPT_IDENTIFIERS["CYAN"] = "Constant color (30, 180, 170).";
	INTERP_SCRIPT_IDENTIFIERS["PURPLE"] = "Constant color (220, 0, 220).";
	INTERP_SCRIPT_IDENTIFIERS["YELLOW"] = "Constant color (220, 220, 0).";
	INTERP_SCRIPT_IDENTIFIERS["ORANGE"] = "Constant color (255, 140, 0).";
	INTERP_SCRIPT_IDENTIFIERS["MAGENTA"] = "Constant color (155, 0, 155).";
	INTERP_SCRIPT_IDENTIFIERS["CRIMSON"] = "Constant color (100, 0, 0).";
	INTERP_SCRIPT_IDENTIFIERS["WHEAT"] = "Constant color (245, 220, 180).";
	INTERP_SCRIPT_IDENTIFIERS["SKY"] = "Constant color (190, 245, 255).";

	// Mesh
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_POINTLIST), "TOPOLOGY_POINTS");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_LINELIST), "TOPOLOGY_LINES");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "TOPOLOGY_TRIANGLES");
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_POINTS"] = "Makes the rasterizer render this mesh as an array of POINTS.";
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_LINES"] = "Makes the rasterizer render this mesh as an array of LINES.";
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_TRIANGLES"] = "Makes the rasterizer render this mesh as an array of TRIANGLES.";

	// Camera
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) CameraType::PERSPECTIVE), "CAMERA_PERSPECTIVE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) CameraType::ORTHOGRAPHIC), "CAMERA_ORTHOGRAPHIC");
	INTERP_SCRIPT_IDENTIFIERS["CAMERA_PERSPECTIVE"] = "Camera that has a perspective view of the scene.";
	INTERP_SCRIPT_IDENTIFIERS["CAMERA_ORTHOGRAPHIC"] = "Camera that has an orthographic view of the scene.";

	return 0;
}();

const int load_functions = [&]
{
	using namespace titian;

	// Var info
	INTERP_SCRIPT_MODULE->eval("global is_null = fun(object) { return is_var_null(object); }");
	INTERP_SCRIPT_MODULE->eval("global is_valid = fun(object) { return !is_null(object); }");
	INTERP_SCRIPT_MODULE->eval("global get_type = fun(object) { return type_name(object); }");
	INTERP_SCRIPT_IDENTIFIERS["is_null"] = "Returns true if the given object IS null, false otherwise.";
	INTERP_SCRIPT_IDENTIFIERS["is_valid"] = "Returns true if the given object IS NOT null, false otherwise.";
	INTERP_SCRIPT_IDENTIFIERS["get_type"] = "Returns the type name (string) of the given object.";

	// Casting
	INTERP_SCRIPT_IDENTIFIERS["to_string"] = "Converts the given object to a string.";
	INTERP_SCRIPT_IDENTIFIERS["to_float"] = "Converts the given object to a float.";
	INTERP_SCRIPT_IDENTIFIERS["to_int"] = "Converts the given object to an int.";

	// Calls
	INTERP_SCRIPT_IDENTIFIERS["on_start"] = "Called once at the start of the game.";
	INTERP_SCRIPT_IDENTIFIERS["on_update"] = "Called every frame of the game.";
	INTERP_SCRIPT_IDENTIFIERS["on_collision"] = "Called every time a collision happens.";
	INTERP_SCRIPT_IDENTIFIERS["on_ui"] = "Called every frame of the UI.";

	// Logging
	INTERP_SCRIPT_MODULE->add(cs::fun(&Logger::log<const std::string&>), "log");
	INTERP_SCRIPT_MODULE->eval("global print = fun(object) { return log(to_string(object)); }");
	INTERP_SCRIPT_IDENTIFIERS["log"] = "Outputs the given string to the log window.";
	INTERP_SCRIPT_IDENTIFIERS["print"] = "Converts the given object to a string and logs it.";

	// Time
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> float { return BOUND_LAYERS::app_layer->timer.elapsed(); }), "get_elapsed_t");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> float { return BOUND_LAYERS::app_layer->timer.delta(); }), "get_delta_t");
	INTERP_SCRIPT_IDENTIFIERS["get_elapsed_t"] = "Returns the elapsed time since the game start.";
	INTERP_SCRIPT_IDENTIFIERS["get_delta_t"] = "Returns the time since the last frame.";

	// Input
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Keyboard* { return &BOUND_LAYERS::app_layer->window.keyboard; }), "get_keyboard");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Mouse* { return &BOUND_LAYERS::app_layer->window.mouse; }), "get_mouse");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Window* { return &BOUND_LAYERS::app_layer->window; }), "get_window");
	INTERP_SCRIPT_IDENTIFIERS["get_keyboard"] = "Returns a reference to the game keyboard.";
	INTERP_SCRIPT_IDENTIFIERS["get_mouse"] = "Returns a reference to the game mouse.";
	INTERP_SCRIPT_IDENTIFIERS["get_window"] = "Returns a reference to the game window.";

	// Float3x3
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float3x3::translation), "Float3x3_translation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float3x3::rotation), "Float3x3_rotation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float3x3::scaling), "Float3x3_scaling");
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_translation"] = "Creates a 3x3 translation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_rotation"] = "Creates a 3x3 rotation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_scaling"] = "Creates a 3x3 scaling matrix.";

	// Float4x4
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::translation), "Float4x4_translation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::rotation), "Float4x4_rotation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::scaling), "Float4x4_scaling");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::look_at), "Float4x4_look_at");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::perspective), "Float4x4_perspective");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::Float4x4::orthographic), "Float4x4_orthographic");
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_translation"] = "Creates a 4x4 translation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_rotation"] = "Creates a 4x4 rotation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_scaling"] = "Creates a 4x4 scaling matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_look_at"] = "Creates a 4x4 look at matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_perspective"] = "Creates a 4x4 perspective matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_orthographic"] = "Creates a 4x4 orthographic matrix.";

	// Math
	INTERP_SCRIPT_MODULE->add(cs::fun<int (*)(int)>(&std::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::abs), "abs");
	INTERP_SCRIPT_IDENTIFIERS["abs"] = "Returns absolute value of the given value.";

	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::sin), "sin");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::cos), "cos");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::tan), "tan");
	INTERP_SCRIPT_IDENTIFIERS["sin"] = "Returns sin of the given angle in radians.";
	INTERP_SCRIPT_IDENTIFIERS["cos"] = "Returns cos of the given angle in radians.";
	INTERP_SCRIPT_IDENTIFIERS["tan"] = "Returns tan of the given angle in radians.";

	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::asin), "asin");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::acos), "acos");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&std::atan), "atan");
	INTERP_SCRIPT_IDENTIFIERS["asin"] = "Returns angle in radians of the given sin.";
	INTERP_SCRIPT_IDENTIFIERS["acos"] = "Returns angle in radians of the given cos.";
	INTERP_SCRIPT_IDENTIFIERS["atan"] = "Returns angle in radians of the given tan.";

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::sin_deg<float>), "sin_deg");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::cos_deg<float>), "cos_deg");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::tan_deg<float>), "tan_deg");
	INTERP_SCRIPT_IDENTIFIERS["sin_deg"] = "Returns sin of the given angle in degrees.";
	INTERP_SCRIPT_IDENTIFIERS["cos_deg"] = "Returns cos of the given angle in degrees.";
	INTERP_SCRIPT_IDENTIFIERS["tan_deg"] = "Returns tan of the given angle in degrees.";

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::asin_deg<float>), "asin_deg");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::acos_deg<float>), "acos_deg");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::atan_deg<float>), "atan_deg");
	INTERP_SCRIPT_IDENTIFIERS["asin_deg"] = "Returns angle in degrees of the given sin.";
	INTERP_SCRIPT_IDENTIFIERS["acos_deg"] = "Returns angle in degrees of the given cos.";
	INTERP_SCRIPT_IDENTIFIERS["atan_deg"] = "Returns angle in degrees of the given tan.";

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::calc_ndc<float>), "calc_ndc");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::calc_ndc_ar<float>), "calc_ndc_ar");
	INTERP_SCRIPT_IDENTIFIERS["calc_ndc"] = "Converts coorinates into ndc.";
	INTERP_SCRIPT_IDENTIFIERS["calc_ndc_ar"] = "Converts coorinates into ndc and applies aspect ratio.";

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::line_x<float>), "line_x");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::line_y<float>), "line_y");
	INTERP_SCRIPT_IDENTIFIERS["line_x"] = "Calculates line x.";
	INTERP_SCRIPT_IDENTIFIERS["line_y"] = "Calculates line y.";

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::wrap<float>), "wrap");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::unwrap<float>), "unwrap");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::clamp<float>), "clamp");
	INTERP_SCRIPT_IDENTIFIERS["wrap"] = "Wraps the given value to the range [0, 1].";
	INTERP_SCRIPT_IDENTIFIERS["unwrap"] = "Unwraps the given value to the defined range.";
	INTERP_SCRIPT_IDENTIFIERS["clamp"] = "Clamps the given value between the given min and max.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Quaternion<float> (*)(const kl::Float3&)>(&kl::to_quaternion<float>), "to_quaternion");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Quaternion<float> (*)(const kl::Float3&, const kl::Float3&)>(&kl::to_quaternion<float>), "to_quaternion");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Quaternion<float>&)>(&kl::to_euler<float>), "to_euler");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&, const kl::Float3&)>(&kl::to_euler<float>), "to_euler");
	INTERP_SCRIPT_IDENTIFIERS["to_quaternion"] = "Converts the given euler angles to a quaternion.";
	INTERP_SCRIPT_IDENTIFIERS["to_euler"] = "Converts the given quaternion to euler angles.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Complex<float> (*)(const kl::Complex<float>&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Complex<float> (*)(const kl::Complex<float>&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Complex<float> (*)(const kl::Complex<float>&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_IDENTIFIERS["abs"] = "Returns the absolute value.";
	INTERP_SCRIPT_IDENTIFIERS["normalize"] = "Normalizes the given vector.";
	INTERP_SCRIPT_IDENTIFIERS["inverse"] = "Returns the inverse of the given math object.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Quaternion<float> (*)(const kl::Quaternion<float>&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Quaternion<float> (*)(const kl::Quaternion<float>&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Quaternion<float> (*)(const kl::Quaternion<float>&)>(&kl::inverse), "inverse");

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Int2 (*)(const kl::Int2&)>(&kl::abs), "abs");

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2 (*)(const kl::Float2&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2 (*)(const kl::Float2&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const kl::Float2&, const kl::Float2&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(kl::Float2, kl::Float2, bool)>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2 (*)(const kl::Float2&, float)>(&kl::rotate<float>), "rotate");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2 (*)(const kl::Float2&, const kl::Float2&)>(&kl::reflect<float>), "reflect");
	INTERP_SCRIPT_IDENTIFIERS["dot"] = "Returns the dot product of two vectors.";
	INTERP_SCRIPT_IDENTIFIERS["angle"] = "Returns the angle in degrees between two vectors.";
	INTERP_SCRIPT_IDENTIFIERS["rotate"] = "Returns the rotated vector.";
	INTERP_SCRIPT_IDENTIFIERS["reflect"] = "Returns the reflected vector.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const kl::Float3&, const kl::Float3&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const kl::Float3&, const kl::Float3&)>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&, const kl::Float3&, float)>(&kl::rotate<float>), "rotate");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&, kl::Float3)>(&kl::reflect<float>), "reflect");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(const kl::Float3&, const kl::Float3&)>(&kl::cross<float>), "cross");
	INTERP_SCRIPT_IDENTIFIERS["cross"] = "Returns the cross product of two vectors.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4 (*)(const kl::Float4&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4 (*)(const kl::Float4&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const kl::Float4&, const kl::Float4&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const kl::Float4&, const kl::Float4&)>(&kl::angle), "angle");

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2x2 (*)(const kl::Float2x2&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2x2 (*)(const kl::Float2x2&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2x2 (*)(const kl::Float2x2&)>(&kl::transpose), "transpose");
	INTERP_SCRIPT_IDENTIFIERS["transpose"] = "Returns the transposed matrix.";

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3x3 (*)(const kl::Float3x3&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3x3 (*)(const kl::Float3x3&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3x3 (*)(const kl::Float3x3&)>(&kl::transpose), "transpose");

	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4x4 (*)(const kl::Float4x4&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4x4 (*)(const kl::Float4x4&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4x4 (*)(const kl::Float4x4&)>(&kl::transpose), "transpose");

	// Random
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_bool), "gen_random_bool");
	INTERP_SCRIPT_MODULE->add(cs::fun<int (*)(int, int)>(&kl::random::gen_int), "gen_random_int");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Int2 (*)(int, int)>(&kl::random::gen_int2), "gen_random_int2");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float, float)>(&kl::random::gen_float), "gen_random_float");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float2 (*)(float, float)>(&kl::random::gen_float2), "gen_random_float2");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float3 (*)(float, float)>(&kl::random::gen_float3), "gen_random_float3");
	INTERP_SCRIPT_MODULE->add(cs::fun<kl::Float4(*)(float, float)>(&kl::random::gen_float4), "gen_random_float4");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_color), "gen_random_color");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_char), "gen_random_char");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_string), "gen_random_string");
	INTERP_SCRIPT_IDENTIFIERS["gen_random_bool"] = "Generates a random bool.";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_int"] = "Generates a random integer (start_inclusive, end_exclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float"] = "Generates a random float (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float2"] = "Generates a random float2 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float3"] = "Generates a random float3 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float4"] = "Generates a random float4 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_color"] = "Generates a random color (is_grayscale).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_char"] = "Generates a random char (can_be_upper).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_string"] = "Generates a random string (length).";

	// UI
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_separator), "ui_separator");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_same_line), "ui_same_line");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_set_next_width), "ui_set_next_width");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_cursor_pos), "ui_cursor_pos");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_set_cursor_pos), "ui_set_cursor_pos");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_window), "ui_window");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_button), "ui_button");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_checkbox), "ui_checkbox");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_text), "ui_text");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_colored_text), "ui_colored_text");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_int), "ui_input_int");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_float), "ui_input_float");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_float2), "ui_input_float2");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_float3), "ui_input_float3");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_float4), "ui_input_float4");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_text), "ui_input_text");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_input_text_multiline), "ui_input_text_multiline");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_edit_color3), "ui_edit_color3");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_edit_color4), "ui_edit_color4");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_drag_int), "ui_drag_int");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_drag_float), "ui_drag_float");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_drag_float2), "ui_drag_float2");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_drag_float3), "ui_drag_float3");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_drag_float4), "ui_drag_float4");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_slide_int), "ui_slide_int");
	INTERP_SCRIPT_MODULE->add(cs::fun(&ui_slide_float), "ui_slide_float");
	INTERP_SCRIPT_IDENTIFIERS["ui_separator"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_same_line"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_set_next_width"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_cursor_pos"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_set_cursor_pos"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_window"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_button"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_checkbox"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_text"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_colored_text"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_int"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_float"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_float2"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_float3"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_float4"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_text"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_input_text_multiline"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_edit_color3"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_edit_color4"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_drag_int"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_drag_float"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_drag_float2"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_drag_float3"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_drag_float4"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_slide_int"] = "...";
	INTERP_SCRIPT_IDENTIFIERS["ui_slide_float"] = "...";

	return 0;
}();
