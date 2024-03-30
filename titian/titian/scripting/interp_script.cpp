#include "main.h"


/* Inter script */
titian::InterpScript::InterpScript()
	: Script(Type::INTERP)
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
	m_engine = new chaiscript::ChaiScript();

	try {
		m_engine->add(INTERP_SCRIPT_MODULE);
		m_engine->eval(this->source);
		m_start_function = m_engine->eval<std::function<void(Scene*)>>("on_start");
		m_update_function = m_engine->eval<std::function<void(Scene*)>>("on_update");
	}
	catch (std::exception& e) {
		Logger::log(e.what());
	}
}

void titian::InterpScript::call_start()
{
	if (m_start_function) {
		try {
			m_start_function(&GameLayer::BOUND_SELF->scene);
		}
		catch (std::exception& e) {
			Logger::log(e.what());
		}
	}
}

void titian::InterpScript::call_update()
{
	if (m_update_function) {
		try {
			m_update_function(&GameLayer::BOUND_SELF->scene);
		}
		catch (std::exception& e) {
			Logger::log(e.what());
		}
	}
}

std::map<std::string, chaiscript::Boxed_Value> titian::InterpScript::get_parameters()
{
	if (!m_engine) {
		return {};
	}

	std::map<std::string, chaiscript::Boxed_Value> result;
	for (auto& [name, value] : m_engine->get_state().engine_state.m_global_objects) {
		if (name.starts_with("p_")) {
			result[name] = value;
		}
	}
	return result;
}

/* SCRIPT BINDS */
int load_types = [&]
{
	using namespace titian;
	
	// Bootstrap
	chaiscript::bootstrap::standard_library::vector_type<std::vector<kl::Vertex>>("MeshData", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::vector_type<std::vector<kl::Color>>("TextureData", *INTERP_SCRIPT_MODULE);

	chaiscript::bootstrap::standard_library::vector_type<std::vector<Mesh*>>("MeshVector", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::vector_type<std::vector<Texture*>>("TextureVector", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::vector_type<std::vector<Material*>>("MaterialVector", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::vector_type<std::vector<Entity*>>("EntityVector", *INTERP_SCRIPT_MODULE);

	chaiscript::bootstrap::standard_library::map_type<std::map<std::string, Mesh*>>("MeshMap", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::map_type<std::map<std::string, Texture*>>("TextureMap", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::map_type<std::map<std::string, Material*>>("MaterialMap", *INTERP_SCRIPT_MODULE);
	chaiscript::bootstrap::standard_library::map_type<std::map<std::string, Entity*>>("EntityMap", *INTERP_SCRIPT_MODULE);

	// Derived
	INTERP_SCRIPT_MODULE->add(chaiscript::base_class<Entity, Camera>());
	INTERP_SCRIPT_MODULE->add(chaiscript::base_class<Entity, Light>());
	INTERP_SCRIPT_MODULE->add(chaiscript::base_class<Light, AmbientLight>());
	INTERP_SCRIPT_MODULE->add(chaiscript::base_class<Light, PointLight>());
	INTERP_SCRIPT_MODULE->add(chaiscript::base_class<Light, DirectionalLight>());

	// Casts
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Int2, kl::Float2>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float2, kl::Int2>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float2, kl::Complex>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float3, kl::Color>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float3, kl::Quaternion>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float4, kl::Color>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Float4, kl::Quaternion>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Complex, kl::Float2>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Quaternion, kl::Float3>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Quaternion, kl::Float4>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Color, kl::Float3>());
	INTERP_SCRIPT_MODULE->add(chaiscript::type_conversion<kl::Color, kl::Float4>());

	// Int2
	chaiscript::utility::add_class<kl::Int2>(*INTERP_SCRIPT_MODULE, "Int2",
	{
		chaiscript::constructor<kl::Int2()>(),
		chaiscript::constructor<kl::Int2(const kl::Int2&)>(),

		chaiscript::constructor<kl::Int2(int)>(),
		chaiscript::constructor<kl::Int2(int, int)>(),
	},
	{
		{ chaiscript::fun(&kl::Int2::x), "x" },
		{ chaiscript::fun(&kl::Int2::y), "y" },

		{ chaiscript::fun(static_cast<kl::Int2& (kl::Int2::*)(const kl::Int2&)>(&kl::Int2::operator=)), "=" },

		{ chaiscript::fun<int&, kl::Int2, int>(&kl::Int2::operator[]), "[]" },

		{ chaiscript::fun(&kl::Int2::operator==), "==" },
		{ chaiscript::fun(&kl::Int2::operator!=), "!=" },

		{ chaiscript::fun(&kl::Int2::operator+), "+" },
		{ chaiscript::fun(&kl::Int2::operator+=), "+=" },

		{ chaiscript::fun<kl::Int2, kl::Int2, const kl::Int2&>(&kl::Int2::operator-), "-" },
		{ chaiscript::fun(&kl::Int2::operator-=), "-=" },

		{ chaiscript::fun<kl::Int2, kl::Int2, int>(&kl::Int2::operator*), "*" },
		{ chaiscript::fun<void, kl::Int2, int>(&kl::Int2::operator*=), "*=" },

		{ chaiscript::fun<kl::Int2, kl::Int2, const kl::Int2&>(&kl::Int2::operator*), "*" },
		{ chaiscript::fun<void, kl::Int2, const kl::Int2&>(&kl::Int2::operator*=), "*=" },

		{ chaiscript::fun<kl::Int2, kl::Int2, int>(&kl::Int2::operator/), "/" },
		{ chaiscript::fun<void, kl::Int2, int>(&kl::Int2::operator/=), "/=" },

		{ chaiscript::fun<kl::Int2, kl::Int2, const kl::Int2&>(&kl::Int2::operator/), "/" },
		{ chaiscript::fun<void, kl::Int2, const kl::Int2&>(&kl::Int2::operator/=), "/=" },

		{ chaiscript::fun<kl::Int2, kl::Int2>(&kl::Int2::operator-), "-" },

		{ chaiscript::fun([](const kl::Int2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Int2"] = "Two component integer vector.";

	// Float2
	chaiscript::utility::add_class<kl::Float2>(*INTERP_SCRIPT_MODULE, "Float2",
	{
		chaiscript::constructor<kl::Float2()>(),
		chaiscript::constructor<kl::Float2(const kl::Float2&)>(),

		chaiscript::constructor<kl::Float2(float)>(),
		chaiscript::constructor<kl::Float2(float, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Float2::x), "x" },
		{ chaiscript::fun(&kl::Float2::y), "y" },

		{ chaiscript::fun(static_cast<kl::Float2& (kl::Float2::*)(const kl::Float2&)>(&kl::Float2::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Float2, int>(&kl::Float2::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float2::operator==), "==" },
		{ chaiscript::fun(&kl::Float2::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float2::operator+), "+" },
		{ chaiscript::fun(&kl::Float2::operator+=), "+=" },

		{ chaiscript::fun<kl::Float2, kl::Float2, const kl::Float2&>(&kl::Float2::operator-), "-" },
		{ chaiscript::fun(&kl::Float2::operator-=), "-=" },

		{ chaiscript::fun<kl::Float2, kl::Float2, float>(&kl::Float2::operator*), "*" },
		{ chaiscript::fun<void, kl::Float2, float>(&kl::Float2::operator*=), "*=" },

		{ chaiscript::fun<kl::Float2, kl::Float2, const kl::Float2&>(&kl::Float2::operator*), "*" },
		{ chaiscript::fun<void, kl::Float2, const kl::Float2&>(&kl::Float2::operator*=), "*=" },

		{ chaiscript::fun<kl::Float2, kl::Float2, float>(&kl::Float2::operator/), "/" },
		{ chaiscript::fun<void, kl::Float2, float>(&kl::Float2::operator/=), "/=" },

		{ chaiscript::fun<kl::Float2, kl::Float2, const kl::Float2&>(&kl::Float2::operator/), "/" },
		{ chaiscript::fun<void, kl::Float2, const kl::Float2&>(&kl::Float2::operator/=), "/=" },

		{ chaiscript::fun<kl::Float2, kl::Float2>(&kl::Float2::operator-), "-"},
		{ chaiscript::fun(&kl::Float2::length), "length" },

		{ chaiscript::fun([](const kl::Float2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float2"] = "Two component float vector.";
	INTERP_SCRIPT_IDENTIFIERS["length"] = "Returns length of this vector.";

	// Float3
	chaiscript::utility::add_class<kl::Float3>(*INTERP_SCRIPT_MODULE, "Float3",
	{
		chaiscript::constructor<kl::Float3()>(),
		chaiscript::constructor<kl::Float3(const kl::Float3&)>(),

		chaiscript::constructor<kl::Float3(float)>(),
		chaiscript::constructor<kl::Float3(float, float, float)>(),

		chaiscript::constructor<kl::Float3(kl::Float2, float)>(),
		chaiscript::constructor<kl::Float3(float, kl::Float2)>(),
	},
	{
		{ chaiscript::fun(&kl::Float3::x), "x" },
		{ chaiscript::fun(&kl::Float3::y), "y" },
		{ chaiscript::fun(&kl::Float3::z), "z" },

		{ chaiscript::fun(static_cast<kl::Float3& (kl::Float3::*)(const kl::Float3&)>(&kl::Float3::operator=)), "=" },

		{ chaiscript::fun(&kl::Float3::xy), "xy" },

		{ chaiscript::fun<float&, kl::Float3, int>(&kl::Float3::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float3::operator==), "==" },
		{ chaiscript::fun(&kl::Float3::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float3::operator+), "+" },
		{ chaiscript::fun(&kl::Float3::operator+=), "+=" },

		{ chaiscript::fun<kl::Float3, kl::Float3, const kl::Float3&>(&kl::Float3::operator-), "-" },
		{ chaiscript::fun(&kl::Float3::operator-=), "-=" },

		{ chaiscript::fun<kl::Float3, kl::Float3, float>(&kl::Float3::operator*), "*" },
		{ chaiscript::fun<void, kl::Float3, float>(&kl::Float3::operator*=), "*=" },

		{ chaiscript::fun<kl::Float3, kl::Float3, const kl::Float3&>(&kl::Float3::operator*), "*" },
		{ chaiscript::fun<void, kl::Float3, const kl::Float3&>(&kl::Float3::operator*=), "*=" },

		{ chaiscript::fun<kl::Float3, kl::Float3, float>(&kl::Float3::operator/), "/" },
		{ chaiscript::fun<void, kl::Float3, float>(&kl::Float3::operator/=), "/=" },

		{ chaiscript::fun<kl::Float3, kl::Float3, const kl::Float3&>(&kl::Float3::operator/), "/" },
		{ chaiscript::fun<void, kl::Float3, const kl::Float3&>(&kl::Float3::operator/=), "/=" },

		{ chaiscript::fun<kl::Float3, kl::Float3>(&kl::Float3::operator-), "-"},
		{ chaiscript::fun(&kl::Float3::length), "length" },

		{ chaiscript::fun([](const kl::Float3& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float3"] = "Three component float vector.";
	INTERP_SCRIPT_IDENTIFIERS["xy"] = "Returns x and y as a Float2.";

	// Float4
	chaiscript::utility::add_class<kl::Float4>(*INTERP_SCRIPT_MODULE, "Float4",
	{
		chaiscript::constructor<kl::Float4()>(),
		chaiscript::constructor<kl::Float4(const kl::Float4&)>(),

		chaiscript::constructor<kl::Float4(float)>(),
		chaiscript::constructor<kl::Float4(float, float, float, float)>(),

		chaiscript::constructor<kl::Float4(kl::Float2, float, float)>(),
		chaiscript::constructor<kl::Float4(float, kl::Float2, float)>(),
		chaiscript::constructor<kl::Float4(float, float, kl::Float2)>(),
		chaiscript::constructor<kl::Float4(kl::Float2, kl::Float2)>(),

		chaiscript::constructor<kl::Float4(kl::Float3, float)>(),
		chaiscript::constructor<kl::Float4(float, kl::Float3)>(),
	},
	{
		{ chaiscript::fun(&kl::Float4::x), "x" },
		{ chaiscript::fun(&kl::Float4::y), "y" },
		{ chaiscript::fun(&kl::Float4::z), "z" },
		{ chaiscript::fun(&kl::Float4::w), "w" },

		{ chaiscript::fun(static_cast<kl::Float4& (kl::Float4::*)(const kl::Float4&)>(&kl::Float4::operator=)), "=" },

		{ chaiscript::fun(&kl::Float4::xy), "xy" },
		{ chaiscript::fun(&kl::Float4::xyz), "xyz" },

		{ chaiscript::fun<float&, kl::Float4, int>(&kl::Float4::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float4::operator==), "==" },
		{ chaiscript::fun(&kl::Float4::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float4::operator+), "+" },
		{ chaiscript::fun(&kl::Float4::operator+=), "+=" },

		{ chaiscript::fun<kl::Float4, kl::Float4, const kl::Float4&>(&kl::Float4::operator-), "-" },
		{ chaiscript::fun(&kl::Float4::operator-=), "-=" },

		{ chaiscript::fun<kl::Float4, kl::Float4, float>(&kl::Float4::operator*), "*" },
		{ chaiscript::fun<void, kl::Float4, float>(&kl::Float4::operator*=), "*=" },

		{ chaiscript::fun<kl::Float4, kl::Float4, const kl::Float4&>(&kl::Float4::operator*), "*" },
		{ chaiscript::fun<void, kl::Float4, const kl::Float4&>(&kl::Float4::operator*=), "*=" },

		{ chaiscript::fun<kl::Float4, kl::Float4, float>(&kl::Float4::operator/), "/" },
		{ chaiscript::fun<void, kl::Float4, float>(&kl::Float4::operator/=), "/=" },

		{ chaiscript::fun<kl::Float4, kl::Float4, const kl::Float4&>(&kl::Float4::operator/), "/" },
		{ chaiscript::fun<void, kl::Float4, const kl::Float4&>(&kl::Float4::operator/=), "/=" },

		{ chaiscript::fun<kl::Float4, kl::Float4>(&kl::Float4::operator-), "-"},
		{ chaiscript::fun(&kl::Float4::length), "length" },
		
		{ chaiscript::fun([](const kl::Float4& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float4"] = "Four component float vector.";
	INTERP_SCRIPT_IDENTIFIERS["xyz"] = "Returns x, y and z as a Float3.";

	// Float2x2
	chaiscript::utility::add_class<kl::Float2x2>(*INTERP_SCRIPT_MODULE, "Float2x2",
	{
		chaiscript::constructor<kl::Float2x2()>(),
		chaiscript::constructor<kl::Float2x2(const kl::Float2x2&)>(),
	},
	{
		{ chaiscript::fun(static_cast<kl::Float2x2& (kl::Float2x2::*)(const kl::Float2x2&)>(&kl::Float2x2::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Float2x2, int>(&kl::Float2x2::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float2x2::operator==), "==" },
		{ chaiscript::fun(&kl::Float2x2::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float2x2::operator+), "+" },
		{ chaiscript::fun(&kl::Float2x2::operator+=), "+=" },

		{ chaiscript::fun(&kl::Float2x2::operator-), "-" },
		{ chaiscript::fun(&kl::Float2x2::operator-=), "-=" },

		{ chaiscript::fun<kl::Float2x2, kl::Float2x2, float>(&kl::Float2x2::operator*), "*" },
		{ chaiscript::fun<void, kl::Float2x2, float>(&kl::Float2x2::operator*=), "*=" },

		{ chaiscript::fun<kl::Float2x2, kl::Float2x2, const kl::Float2x2&>(&kl::Float2x2::operator*), "*" },
		{ chaiscript::fun<void, kl::Float2x2, const kl::Float2x2&>(&kl::Float2x2::operator*=), "*=" },

		{ chaiscript::fun<kl::Float2, kl::Float2x2, const kl::Float2&>(&kl::Float2x2::operator*), "*"},

		{ chaiscript::fun(&kl::Float2x2::determinant), "determinant" },

		{ chaiscript::fun([](const kl::Float2x2& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float2x2"] = "Two float wide square matrix.";
	INTERP_SCRIPT_IDENTIFIERS["determinant"] = "Returns the determinant of this matrix.";

	// Float3x3
	chaiscript::utility::add_class<kl::Float3x3>(*INTERP_SCRIPT_MODULE, "Float3x3",
	{
		chaiscript::constructor<kl::Float3x3()>(),
		chaiscript::constructor<kl::Float3x3(const kl::Float3x3&)>(),
	},
	{
		{ chaiscript::fun(static_cast<kl::Float3x3& (kl::Float3x3::*)(const kl::Float3x3&)>(&kl::Float3x3::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Float3x3, int>(&kl::Float3x3::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float3x3::operator==), "==" },
		{ chaiscript::fun(&kl::Float3x3::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float3x3::operator+), "+" },
		{ chaiscript::fun(&kl::Float3x3::operator+=), "+=" },

		{ chaiscript::fun(&kl::Float3x3::operator-), "-" },
		{ chaiscript::fun(&kl::Float3x3::operator-=), "-=" },

		{ chaiscript::fun<kl::Float3x3, kl::Float3x3, float>(&kl::Float3x3::operator*), "*" },
		{ chaiscript::fun<void, kl::Float3x3, float>(&kl::Float3x3::operator*=), "*=" },

		{ chaiscript::fun<kl::Float3x3, kl::Float3x3, const kl::Float3x3&>(&kl::Float3x3::operator*), "*" },
		{ chaiscript::fun<void, kl::Float3x3, const kl::Float3x3&>(&kl::Float3x3::operator*=), "*=" },

		{ chaiscript::fun<kl::Float3, kl::Float3x3, const kl::Float3&>(&kl::Float3x3::operator*), "*"},

		{ chaiscript::fun(&kl::Float3x3::determinant), "determinant" },

		{ chaiscript::fun([](const kl::Float3x3& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float3x3"] = "Three float wide square matrix.";

	// Float4x4
	chaiscript::utility::add_class<kl::Float4x4>(*INTERP_SCRIPT_MODULE, "Float4x4",
	{
		chaiscript::constructor<kl::Float4x4()>(),
		chaiscript::constructor<kl::Float4x4(const kl::Float4x4&)>(),
	},
	{
		{ chaiscript::fun(static_cast<kl::Float4x4& (kl::Float4x4::*)(const kl::Float4x4&)>(&kl::Float4x4::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Float4x4, int>(&kl::Float4x4::operator[]), "[]"},

		{ chaiscript::fun(&kl::Float4x4::operator==), "==" },
		{ chaiscript::fun(&kl::Float4x4::operator!=), "!=" },

		{ chaiscript::fun(&kl::Float4x4::operator+), "+" },
		{ chaiscript::fun(&kl::Float4x4::operator+=), "+=" },

		{ chaiscript::fun(&kl::Float4x4::operator-), "-" },
		{ chaiscript::fun(&kl::Float4x4::operator-=), "-=" },

		{ chaiscript::fun<kl::Float4x4, kl::Float4x4, float>(&kl::Float4x4::operator*), "*" },
		{ chaiscript::fun<void, kl::Float4x4, float>(&kl::Float4x4::operator*=), "*=" },

		{ chaiscript::fun<kl::Float4x4, kl::Float4x4, const kl::Float4x4&>(&kl::Float4x4::operator*), "*" },
		{ chaiscript::fun<void, kl::Float4x4, const kl::Float4x4&>(&kl::Float4x4::operator*=), "*=" },

		{ chaiscript::fun<kl::Float4, kl::Float4x4, const kl::Float4&>(&kl::Float4x4::operator*), "*"},

		{ chaiscript::fun(&kl::Float4x4::determinant), "determinant" },

		{ chaiscript::fun([](const kl::Float4x4& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Float4x4"] = "Four float wide square matrix.";

	// Complex
	chaiscript::utility::add_class<kl::Complex>(*INTERP_SCRIPT_MODULE, "Complex",
	{
		chaiscript::constructor<kl::Complex()>(),
		chaiscript::constructor<kl::Complex(const kl::Complex&)>(),

		chaiscript::constructor<kl::Complex(float, float)>(),
		chaiscript::constructor<kl::Complex(float)>(),
	},
	{
		{ chaiscript::fun(&kl::Complex::r), "r" },
		{ chaiscript::fun(&kl::Complex::i), "i" },

		{ chaiscript::fun(static_cast<kl::Complex& (kl::Complex::*)(const kl::Complex&)>(&kl::Complex::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Complex, int>(&kl::Complex::operator[]), "[]"},

		{ chaiscript::fun(&kl::Complex::operator==), "==" },
		{ chaiscript::fun(&kl::Complex::operator!=), "!=" },

		{ chaiscript::fun(&kl::Complex::operator+), "+" },
		{ chaiscript::fun(&kl::Complex::operator+=), "+=" },

		{ chaiscript::fun<kl::Complex, kl::Complex, const kl::Complex&>(&kl::Complex::operator-), "-" },
		{ chaiscript::fun(&kl::Complex::operator-=), "-=" },

		{ chaiscript::fun<kl::Complex, kl::Complex, float>(&kl::Complex::operator*), "*" },
		{ chaiscript::fun<void, kl::Complex, float>(&kl::Complex::operator*=), "*=" },

		{ chaiscript::fun<kl::Complex, kl::Complex, const kl::Complex&>(&kl::Complex::operator*), "*" },
		{ chaiscript::fun<void, kl::Complex, const kl::Complex&>(&kl::Complex::operator*=), "*=" },

		{ chaiscript::fun<kl::Complex, kl::Complex>(&kl::Complex::operator-), "-" },
		{ chaiscript::fun(&kl::Complex::length), "length" },

		{ chaiscript::fun([](const kl::Complex& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Complex"] = "Two component imaginary number.";

	// Quaternion
	chaiscript::utility::add_class<kl::Quaternion>(*INTERP_SCRIPT_MODULE, "Quaternion",
	{
		chaiscript::constructor<kl::Quaternion()>(),
		chaiscript::constructor<kl::Quaternion(const kl::Quaternion&)>(),

		chaiscript::constructor<kl::Quaternion(float, float, float)>(),
		chaiscript::constructor<kl::Quaternion(float, float, float, float)>(),
		chaiscript::constructor<kl::Quaternion(kl::Float3, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Quaternion::w), "w" },
		{ chaiscript::fun(&kl::Quaternion::x), "x" },
		{ chaiscript::fun(&kl::Quaternion::y), "y" },
		{ chaiscript::fun(&kl::Quaternion::z), "z" },

		{ chaiscript::fun(static_cast<kl::Quaternion& (kl::Quaternion::*)(const kl::Quaternion&)>(&kl::Quaternion::operator=)), "=" },

		{ chaiscript::fun<float&, kl::Quaternion, int>(&kl::Quaternion::operator[]), "[]"},

		{ chaiscript::fun(&kl::Quaternion::operator==), "==" },
		{ chaiscript::fun(&kl::Quaternion::operator!=), "!=" },

		{ chaiscript::fun(&kl::Quaternion::operator+), "+" },
		{ chaiscript::fun(&kl::Quaternion::operator+=), "+=" },

		{ chaiscript::fun<kl::Quaternion, kl::Quaternion, const kl::Quaternion&>(&kl::Quaternion::operator-), "-" },
		{ chaiscript::fun(&kl::Quaternion::operator-=), "-=" },

		{ chaiscript::fun<kl::Quaternion, kl::Quaternion, float>(&kl::Quaternion::operator*), "*" },
		{ chaiscript::fun<void, kl::Quaternion, float>(&kl::Quaternion::operator*=), "*=" },

		{ chaiscript::fun<kl::Quaternion, kl::Quaternion, const kl::Quaternion&>(&kl::Quaternion::operator*), "*" },
		{ chaiscript::fun<void, kl::Quaternion, const kl::Quaternion&>(&kl::Quaternion::operator*=), "*=" },

		{ chaiscript::fun<kl::Quaternion, kl::Quaternion>(&kl::Quaternion::operator-), "-" },
		{ chaiscript::fun(&kl::Quaternion::length), "length" },

		{ chaiscript::fun([](const kl::Quaternion& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Quaternion"] = "Four component imaginary number.";

	// Vertex
	chaiscript::utility::add_class<kl::Vertex>(*INTERP_SCRIPT_MODULE, "Vertex",
	{
		chaiscript::constructor<kl::Vertex()>(),
		chaiscript::constructor<kl::Vertex(const kl::Vertex&)>(),

		chaiscript::constructor<kl::Vertex(kl::Float3)>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float2)>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float3)>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float2, kl::Float3)>(),
	},
	{
		{ chaiscript::fun(&kl::Vertex::world), "world" },
		{ chaiscript::fun(&kl::Vertex::normal), "normal" },
		{ chaiscript::fun(&kl::Vertex::texture), "texture" },

		{ chaiscript::fun(static_cast<kl::Vertex& (kl::Vertex::*)(const kl::Vertex&)>(&kl::Vertex::operator=)), "=" },

		{ chaiscript::fun([](const kl::Vertex& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Vertex"] = "A 3D point of a mesh.";

	// Triangle
	chaiscript::utility::add_class<kl::Triangle>(*INTERP_SCRIPT_MODULE, "Triangle",
	{
		chaiscript::constructor<kl::Triangle()>(),
		chaiscript::constructor<kl::Triangle(const kl::Triangle&)>(),

		chaiscript::constructor<kl::Triangle(kl::Vertex, kl::Vertex, kl::Vertex)>(),
	},
	{
		{ chaiscript::fun(&kl::Triangle::a), "a" },
		{ chaiscript::fun(&kl::Triangle::b), "b" },
		{ chaiscript::fun(&kl::Triangle::c), "c" },

		{ chaiscript::fun(static_cast<kl::Triangle& (kl::Triangle::*)(const kl::Triangle&)>(&kl::Triangle::operator=)), "=" },

		{ chaiscript::fun([](const kl::Triangle& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Triangle"] = "A 3D triangle in space.";

	// Plane
	chaiscript::utility::add_class<kl::Plane>(*INTERP_SCRIPT_MODULE, "Plane",
	{
		chaiscript::constructor<kl::Plane()>(),
		chaiscript::constructor<kl::Plane(const kl::Plane&)>(),

		chaiscript::constructor<kl::Plane(kl::Float3, kl::Float3)>(),
	},
	{
		{ chaiscript::fun(&kl::Plane::origin), "origin" },

		{ chaiscript::fun(static_cast<kl::Plane& (kl::Plane::*)(const kl::Plane&)>(&kl::Plane::operator=)), "=" },

		{ chaiscript::fun(&kl::Plane::set_normal), "set_normal" },
		{ chaiscript::fun(&kl::Plane::normal), "normal" },

		{ chaiscript::fun(&kl::Plane::in_front), "in_front" },

		{ chaiscript::fun([](const kl::Plane& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Plane"] = "A 3D plane in space.";
	INTERP_SCRIPT_IDENTIFIERS["set_normal"] = "Normalizes and sets the normal.";
	INTERP_SCRIPT_IDENTIFIERS["normal"] = "Returns the normalized normal.";
	INTERP_SCRIPT_IDENTIFIERS["in_front"] = "Checks if a 3D point is in front of the plane.";

	// Sphere
	chaiscript::utility::add_class<kl::Sphere>(*INTERP_SCRIPT_MODULE, "Sphere",
	{
		chaiscript::constructor<kl::Sphere()>(),
		chaiscript::constructor<kl::Sphere(const kl::Sphere&)>(),

		chaiscript::constructor<kl::Sphere(kl::Float3, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Sphere::origin), "origin" },
		{ chaiscript::fun(&kl::Sphere::radius), "radius" },

		{ chaiscript::fun(static_cast<kl::Sphere& (kl::Sphere::*)(const kl::Sphere&)>(&kl::Sphere::operator=)), "=" },

		{ chaiscript::fun([](const kl::Sphere& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Sphere"] = "A 3D sphere in space.";

	// Ray
	chaiscript::utility::add_class<kl::Ray>(*INTERP_SCRIPT_MODULE, "Ray",
	{
		chaiscript::constructor<kl::Ray()>(),
		chaiscript::constructor<kl::Ray(const kl::Ray&)>(),

		chaiscript::constructor<kl::Ray(kl::Float3, kl::Float3)>(),
		chaiscript::constructor<kl::Ray(kl::Float3, kl::Float4x4, kl::Float2)>(),
	},
	{
		{ chaiscript::fun(&kl::Ray::origin), "origin" },

		{ chaiscript::fun(static_cast<kl::Ray& (kl::Ray::*)(const kl::Ray&)>(&kl::Ray::operator=)), "=" },

		{ chaiscript::fun(&kl::Ray::set_direction), "set_direction" },
		{ chaiscript::fun(&kl::Ray::direction), "direction" },

		{ chaiscript::fun(&kl::Ray::intersect_plane), "intersect_plane" },
		{ chaiscript::fun(&kl::Ray::intersect_triangle), "intersect_triangle" },

		{ chaiscript::fun(&kl::Ray::can_intersect_sphere), "can_intersect_sphere" },
		{ chaiscript::fun(&kl::Ray::intersect_sphere), "intersect_sphere" },

		{ chaiscript::fun([](const kl::Ray& object) { return kl::format(object); }), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Ray"] = "A 3D ray in space.";
	INTERP_SCRIPT_IDENTIFIERS["set_direction"] = "Sets the normalized direction.";
	INTERP_SCRIPT_IDENTIFIERS["direction"] = "Returns the normalized direction.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_plane"] = "Ray interesects plane.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_triangle"] = "Ray interesects triangle.";
	INTERP_SCRIPT_IDENTIFIERS["can_intersect_sphere"] = "Checks if this ray can intersect a sphere.";
	INTERP_SCRIPT_IDENTIFIERS["intersect_sphere"] = "Ray interesects sphere.";

	// Color
	chaiscript::utility::add_class<kl::Color>(*INTERP_SCRIPT_MODULE, "Color",
	{
		chaiscript::constructor<kl::Color()>(),
		chaiscript::constructor<kl::Color(const kl::Color&)>(),

		chaiscript::constructor<kl::Color(byte, byte, byte)>(),
		chaiscript::constructor<kl::Color(byte, byte, byte, byte)>(),
	},
	{
		{ chaiscript::fun(&kl::Color::r), "r" },
		{ chaiscript::fun(&kl::Color::g), "g" },
		{ chaiscript::fun(&kl::Color::b), "b" },
		{ chaiscript::fun(&kl::Color::a), "a" },

		{ chaiscript::fun(static_cast<kl::Color& (kl::Color::*)(const kl::Color&)>(&kl::Color::operator=)), "=" },

		{ chaiscript::fun(&kl::Color::operator==), "==" },
		{ chaiscript::fun(&kl::Color::operator!=), "!=" },

		{ chaiscript::fun(&kl::Color::gray), "gray" },
		{ chaiscript::fun(&kl::Color::inverted), "inverted" },
		{ chaiscript::fun(&kl::Color::as_ascii), "as_ascii" },

		{ chaiscript::fun<kl::Color, kl::Color, const kl::Color&, float>(&kl::Color::mix), "mix" },
		{ chaiscript::fun<kl::Color, kl::Color, const kl::Color&>(&kl::Color::mix), "mix" },

		{ chaiscript::fun([](const kl::Color& object) {
			return kl::format('(', static_cast<int>(object.r),
				", ", static_cast<int>(object.g),
				", ", static_cast<int>(object.b),
				", ", static_cast<int>(object.a), ')');
		}), "to_string" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Color"] = "Four component byte[0, 255] color.";
	INTERP_SCRIPT_IDENTIFIERS["gray"] = "Returns the color as gray.";
	INTERP_SCRIPT_IDENTIFIERS["inverted"] = "Inverts the color channels.";
	INTERP_SCRIPT_IDENTIFIERS["as_ascii"] = "Converts the color to closes ascii char.";
	INTERP_SCRIPT_IDENTIFIERS["mix"] = "Mixes two colors.";

	// Mesh
	chaiscript::utility::add_class<Mesh>(*INTERP_SCRIPT_MODULE, "Mesh",
	{},
	{
		{ chaiscript::fun(&Mesh::data_buffer), "data_buffer" },

		{ chaiscript::fun(&Mesh::topology), "topology"},
		{ chaiscript::fun(&Mesh::render_wireframe), "render_wireframe" },

		{ chaiscript::fun(&Mesh::reload), "reload" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Mesh"] = "Object that contains triangle data.";
	INTERP_SCRIPT_IDENTIFIERS["reload"] = "Reloads the self.";

	// Texture
	chaiscript::utility::add_class<Texture>(*INTERP_SCRIPT_MODULE, "Texture",
	{},
	{
		{ chaiscript::fun(&Texture::data_buffer), "data_buffer" },

		{ chaiscript::fun(&Texture::load_as_2D), "load_as_2D" },
		{ chaiscript::fun(&Texture::load_as_cube), "load_as_cube" },

		{ chaiscript::fun(&Texture::create_target_view), "create_target_view" },
		{ chaiscript::fun(&Texture::create_depth_view), "create_depth_view" },
		{ chaiscript::fun(&Texture::create_shader_view), "create_shader_view" },
		{ chaiscript::fun(&Texture::create_access_view), "create_access_view" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Texture"] = "Object that contains pixel data.";
	INTERP_SCRIPT_IDENTIFIERS["load_as_2D"] = "Reloads the texture as 2D.";
	INTERP_SCRIPT_IDENTIFIERS["load_as_cube"] = "Reloads the texture as a cube map.";
	INTERP_SCRIPT_IDENTIFIERS["create_target_view"] = "Creates target view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_depth_view"] = "Creates depth view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_shader_view"] = "Creates shader view for this texture.";
	INTERP_SCRIPT_IDENTIFIERS["create_access_view"] = "Creates access view for this texture.";

	// Material
	chaiscript::utility::add_class<Material>(*INTERP_SCRIPT_MODULE, "Material",
	{},
	{
		{ chaiscript::fun(&Material::alpha_blend), "alpha_blend" },
		{ chaiscript::fun(&Material::texture_blend), "texture_blend" },

		{ chaiscript::fun(&Material::reflection_factor), "reflection_factor" },
		{ chaiscript::fun(&Material::refraction_factor), "refraction_factor" },
		{ chaiscript::fun(&Material::refraction_index), "refraction_index" },

		{ chaiscript::fun(&Material::color), "color" },

		{ chaiscript::fun(&Material::color_map_name), "color_map_name" },
		{ chaiscript::fun(&Material::normal_map_name), "normal_map_name" },
		{ chaiscript::fun(&Material::roughness_map_name), "roughness_map_name" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Material"] = "Object that defines the look of the entity.";

	// Shader
	chaiscript::utility::add_class<Shader>(*INTERP_SCRIPT_MODULE, "Shader",
	{},
	{
		{ chaiscript::fun(&Shader::data_buffer), "data_buffer" },

		{ chaiscript::fun(&Shader::reload), "reload" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Shader"] = "Custom shader override for a material.";

	// Entity
	chaiscript::utility::add_class<Entity>(*INTERP_SCRIPT_MODULE, "Entity",
	{},
	{
		{ chaiscript::fun(&Entity::scale), "scale" },
		{ chaiscript::fun(&Entity::mesh_name), "mesh_name" },
		{ chaiscript::fun(&Entity::material_name), "material_name" },

		{ chaiscript::fun(&Entity::model_matrix), "model_matrix" },
		{ chaiscript::fun(&Entity::collider_matrix), "collider_matrix" },

		{ chaiscript::fun(&Entity::set_rotation), "set_rotation" },
		{ chaiscript::fun(&Entity::rotation), "rotation" },

		{ chaiscript::fun(&Entity::set_position), "set_position" },
		{ chaiscript::fun(&Entity::position), "position" },

		{ chaiscript::fun(&Entity::set_dynamic), "set_dynamic" },
		{ chaiscript::fun(&Entity::is_dynamic), "is_dynamic" },

		{ chaiscript::fun(&Entity::set_gravity), "set_gravity" },
		{ chaiscript::fun(&Entity::has_gravity), "has_gravity" },

		{ chaiscript::fun(&Entity::set_mass), "set_mass" },
		{ chaiscript::fun(&Entity::mass), "mass" },

		{ chaiscript::fun(&Entity::set_velocity), "set_velocity" },
		{ chaiscript::fun(&Entity::velocity), "velocity" },

		{ chaiscript::fun(&Entity::set_angular), "set_angular" },
		{ chaiscript::fun(&Entity::angular), "angular" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Entity"] = "Base entity that's a part of a scene.";
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
	chaiscript::utility::add_class<Camera>(*INTERP_SCRIPT_MODULE, "Camera",
	{},
	{
		{ chaiscript::fun(&Camera::aspect_ratio), "aspect_ratio" },
		{ chaiscript::fun(&Camera::field_of_view), "field_of_view" },
		{ chaiscript::fun(&Camera::near_plane), "near_plane" },
		{ chaiscript::fun(&Camera::far_plane), "far_plane" },
		{ chaiscript::fun(&Camera::sensitivity), "sensitivity" },
		{ chaiscript::fun(&Camera::speed), "speed" },
		{ chaiscript::fun(&Camera::background), "background" },
		{ chaiscript::fun(&Camera::skybox_name), "skybox_name" },

		{ chaiscript::fun(&Camera::update_aspect_ratio), "update_aspect_ratio" },

		{ chaiscript::fun(&Camera::set_forward), "set_forward" },
		{ chaiscript::fun(&Camera::forward), "forward" },

		{ chaiscript::fun(&Camera::set_up), "set_up" },
		{ chaiscript::fun(&Camera::up), "up" },

		{ chaiscript::fun(&Camera::right), "right" },

		{ chaiscript::fun(&Camera::move_forward), "move_forward" },
		{ chaiscript::fun(&Camera::move_back), "move_back" },
		{ chaiscript::fun(&Camera::move_right), "move_right" },
		{ chaiscript::fun(&Camera::move_left), "move_left" },
		{ chaiscript::fun(&Camera::move_up), "move_up" },
		{ chaiscript::fun(&Camera::move_down), "move_down" },

		{ chaiscript::fun(&Camera::rotate), "rotate" },

		{ chaiscript::fun(&Camera::view_matrix), "view_matrix" },
		{ chaiscript::fun(&Camera::projection_matrix), "projection_matrix" },
		{ chaiscript::fun(&Camera::camera_matrix), "camera_matrix" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Camera"] = "Entity that has a view of the scene.";
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
	chaiscript::utility::add_class<Light>(*INTERP_SCRIPT_MODULE, "Light",
	{},
	{
		{ chaiscript::fun(&Light::light_at_point), "light_at_point" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Light"] = "Base class of every light class.";
	INTERP_SCRIPT_IDENTIFIERS["light_at_point"] = "Returns the lights intensity at some 3D point.";

	// Ambient light
	chaiscript::utility::add_class<AmbientLight>(*INTERP_SCRIPT_MODULE, "AmbientLight",
	{},
	{
		{ chaiscript::fun(&AmbientLight::color), "color" },
		{ chaiscript::fun(&AmbientLight::intensity), "intensity" },
	});
	INTERP_SCRIPT_IDENTIFIERS["AmbientLight"] = "Non directional light at shines at all points.";

	// Point light
	chaiscript::utility::add_class<PointLight>(*INTERP_SCRIPT_MODULE, "PointLight",
	{},
	{
		{ chaiscript::fun(&PointLight::color), "color" },
	});
	INTERP_SCRIPT_IDENTIFIERS["PointLight"] = "Light that shines in all directions.";

	// Directional light
	chaiscript::utility::add_class<DirectionalLight>(*INTERP_SCRIPT_MODULE, "DirectionalLight",
	{},
	{
		{ chaiscript::fun(&DirectionalLight::point_size), "point_size" },
		{ chaiscript::fun(&DirectionalLight::color), "color" },

		{ chaiscript::fun(&DirectionalLight::map_resolution), "map_resolution" },

		{ chaiscript::fun(&DirectionalLight::set_direction), "set_direction" },
		{ chaiscript::fun(&DirectionalLight::direction), "direction" },

		{ chaiscript::fun(&DirectionalLight::light_matrix), "light_matrix" },
	});
	INTERP_SCRIPT_IDENTIFIERS["DirectionalLight"] = "Light that shines in a single direction and is infinitely far away.";
	INTERP_SCRIPT_IDENTIFIERS["map_resolution"] = "Returns the shadow map resolution of directional light.";
	INTERP_SCRIPT_IDENTIFIERS["light_matrix"] = "Returns the light matrix.";

	// Scene
	chaiscript::utility::add_class<Scene>(*INTERP_SCRIPT_MODULE, "Scene",
	{},
	{
		{ chaiscript::fun(&Scene::main_camera_name), "main_camera_name" },
		{ chaiscript::fun(&Scene::main_ambient_light_name), "main_ambient_light_name" },
		{ chaiscript::fun(&Scene::main_directional_light_name), "main_directional_light_name" },

		{ chaiscript::fun(&Scene::set_gravity), "set_gravity" },
		{ chaiscript::fun(&Scene::gravity), "gravity" },

		{ chaiscript::fun(&Scene::helper_new_mesh), "new_mesh" },
		{ chaiscript::fun(&Scene::helper_new_texture), "new_texture" },
		{ chaiscript::fun(&Scene::helper_new_material), "new_material" },
		{ chaiscript::fun(&Scene::helper_new_shader), "new_shader" },
		{ chaiscript::fun(&Scene::helper_new_entity), "new_entity" },

		{ chaiscript::fun(&Scene::helper_get_mesh), "get_mesh" },
		{ chaiscript::fun(&Scene::helper_get_texture), "get_texture" },
		{ chaiscript::fun(&Scene::helper_get_material), "get_material" },
		{ chaiscript::fun(&Scene::helper_get_shader), "get_shader" },
		{ chaiscript::fun(&Scene::helper_get_entity), "get_entity" },

		{ chaiscript::fun(&Scene::helper_remove_mesh), "remove_mesh" },
		{ chaiscript::fun(&Scene::helper_remove_texture), "remove_texture" },
		{ chaiscript::fun(&Scene::helper_remove_material), "remove_material" },
		{ chaiscript::fun(&Scene::helper_remove_shader), "remove_shader" },
		{ chaiscript::fun(&Scene::helper_remove_entity), "remove_entity" },

		{ chaiscript::fun(&Scene::helper_contains_mesh), "contains_mesh" },
		{ chaiscript::fun(&Scene::helper_contains_texture), "contains_texture" },
		{ chaiscript::fun(&Scene::helper_contains_material), "contains_material" },
		{ chaiscript::fun(&Scene::helper_contains_shader), "contains_shader" },
		{ chaiscript::fun(&Scene::helper_contains_entity), "contains_entity" },

		{ chaiscript::fun(&Scene::helper_mesh_count), "mesh_count" },
		{ chaiscript::fun(&Scene::helper_texture_count), "texture_count" },
		{ chaiscript::fun(&Scene::helper_material_count), "material_count" },
		{ chaiscript::fun(&Scene::helper_shader_count), "shader_count" },
		{ chaiscript::fun(&Scene::helper_entity_count), "entity_count" },

		{ chaiscript::fun(&Scene::helper_get_all_meshes), "get_all_meshes" },
		{ chaiscript::fun(&Scene::helper_get_all_textures), "get_all_textures" },
		{ chaiscript::fun(&Scene::helper_get_all_materials), "get_all_materials" },
		{ chaiscript::fun(&Scene::helper_get_all_shaders), "get_all_shaders" },
		{ chaiscript::fun(&Scene::helper_get_all_entities), "get_all_entities" },

		{ chaiscript::fun<Camera*>(&Scene::get_casted<Camera>), "get_camera" },
		{ chaiscript::fun<Light*>(&Scene::get_casted<Light>), "get_light" },
		{ chaiscript::fun<AmbientLight*>(&Scene::get_casted<AmbientLight>), "get_ambient_light" },
		{ chaiscript::fun<PointLight*>(&Scene::get_casted<PointLight>), "get_point_light" },
		{ chaiscript::fun<DirectionalLight*>(&Scene::get_casted<DirectionalLight>), "get_directional_light" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Scene"] = "Collection of meshes, textures, materials, scripts and entities.";
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
	chaiscript::utility::add_class<kl::Key>(*INTERP_SCRIPT_MODULE, "Key",
	{},
	{
		{ chaiscript::fun(&kl::Key::is_down), "is_down" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Key"] = "Class representing a single keyboard or a mouse button.";
	INTERP_SCRIPT_IDENTIFIERS["is_down"] = "Returns the down state of key.";

	// Keyboard
	chaiscript::utility::add_class<kl::Keyboard>(*INTERP_SCRIPT_MODULE, "Keyboard",
	{},
	{
		{ chaiscript::fun(&kl::Keyboard::q), "q" },
		{ chaiscript::fun(&kl::Keyboard::w), "w" },
		{ chaiscript::fun(&kl::Keyboard::e), "e" },
		{ chaiscript::fun(&kl::Keyboard::r), "r" },
		{ chaiscript::fun(&kl::Keyboard::t), "t" },
		{ chaiscript::fun(&kl::Keyboard::z), "z" },
		{ chaiscript::fun(&kl::Keyboard::u), "u" },
		{ chaiscript::fun(&kl::Keyboard::i), "i" },
		{ chaiscript::fun(&kl::Keyboard::o), "o" },
		{ chaiscript::fun(&kl::Keyboard::p), "p" },
		{ chaiscript::fun(&kl::Keyboard::a), "a" },
		{ chaiscript::fun(&kl::Keyboard::s), "s" },
		{ chaiscript::fun(&kl::Keyboard::d), "d" },
		{ chaiscript::fun(&kl::Keyboard::f), "f" },
		{ chaiscript::fun(&kl::Keyboard::g), "g" },
		{ chaiscript::fun(&kl::Keyboard::h), "h" },
		{ chaiscript::fun(&kl::Keyboard::j), "j" },
		{ chaiscript::fun(&kl::Keyboard::k), "k" },
		{ chaiscript::fun(&kl::Keyboard::l), "l" },
		{ chaiscript::fun(&kl::Keyboard::y), "y" },
		{ chaiscript::fun(&kl::Keyboard::x), "x" },
		{ chaiscript::fun(&kl::Keyboard::c), "c" },
		{ chaiscript::fun(&kl::Keyboard::v), "v" },
		{ chaiscript::fun(&kl::Keyboard::b), "b" },
		{ chaiscript::fun(&kl::Keyboard::n), "n" },
		{ chaiscript::fun(&kl::Keyboard::m), "m" },

		{ chaiscript::fun(&kl::Keyboard::num0), "num0" },
		{ chaiscript::fun(&kl::Keyboard::num1), "num1" },
		{ chaiscript::fun(&kl::Keyboard::num2), "num2" },
		{ chaiscript::fun(&kl::Keyboard::num3), "num3" },
		{ chaiscript::fun(&kl::Keyboard::num4), "num4" },
		{ chaiscript::fun(&kl::Keyboard::num5), "num5" },
		{ chaiscript::fun(&kl::Keyboard::num6), "num6" },
		{ chaiscript::fun(&kl::Keyboard::num7), "num7" },
		{ chaiscript::fun(&kl::Keyboard::num8), "num8" },
		{ chaiscript::fun(&kl::Keyboard::num9), "num9" },

		{ chaiscript::fun(&kl::Keyboard::period), "period" },
		{ chaiscript::fun(&kl::Keyboard::comma), "comma" },
		{ chaiscript::fun(&kl::Keyboard::plus), "plus" },
		{ chaiscript::fun(&kl::Keyboard::minus), "minus" },

		{ chaiscript::fun(&kl::Keyboard::esc), "esc" },
		{ chaiscript::fun(&kl::Keyboard::tab), "tab" },
		{ chaiscript::fun(&kl::Keyboard::caps), "caps" },
		{ chaiscript::fun(&kl::Keyboard::shift), "shift" },
		{ chaiscript::fun(&kl::Keyboard::ctrl), "ctrl" },
		{ chaiscript::fun(&kl::Keyboard::alt), "alt" },
		{ chaiscript::fun(&kl::Keyboard::space), "space" },
		{ chaiscript::fun(&kl::Keyboard::enter), "enter" },
		{ chaiscript::fun(&kl::Keyboard::insert), "insert" },
		{ chaiscript::fun(&kl::Keyboard::delet), "delete" },

		{ chaiscript::fun(&kl::Keyboard::up), "up" },
		{ chaiscript::fun(&kl::Keyboard::left), "left" },
		{ chaiscript::fun(&kl::Keyboard::down), "down" },
		{ chaiscript::fun(&kl::Keyboard::right), "right" },

		{ chaiscript::fun(&kl::Keyboard::f1), "f1" },
		{ chaiscript::fun(&kl::Keyboard::f2), "f2" },
		{ chaiscript::fun(&kl::Keyboard::f3), "f3" },
		{ chaiscript::fun(&kl::Keyboard::f4), "f4" },
		{ chaiscript::fun(&kl::Keyboard::f5), "f5" },
		{ chaiscript::fun(&kl::Keyboard::f6), "f6" },
		{ chaiscript::fun(&kl::Keyboard::f7), "f7" },
		{ chaiscript::fun(&kl::Keyboard::f8), "f8" },
		{ chaiscript::fun(&kl::Keyboard::f9), "f9" },
		{ chaiscript::fun(&kl::Keyboard::f10), "f10" },
		{ chaiscript::fun(&kl::Keyboard::f11), "f11" },
		{ chaiscript::fun(&kl::Keyboard::f12), "f12" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Keyboard"] = "Class representing a physical keyboard.";

	// Mouse
	chaiscript::utility::add_class<kl::Mouse>(*INTERP_SCRIPT_MODULE, "Mouse",
	{},
	{
		{ chaiscript::fun(&kl::Mouse::left), "left" },
		{ chaiscript::fun(&kl::Mouse::middle), "middle" },
		{ chaiscript::fun(&kl::Mouse::right), "right" },

		{ chaiscript::fun(&kl::Mouse::set_hidden), "set_hiden" },
		{ chaiscript::fun(&kl::Mouse::is_hidden), "is_hidden" },

		{ chaiscript::fun(&kl::Mouse::set_position), "set_position" },
		{ chaiscript::fun(&kl::Mouse::position), "position" },

		{ chaiscript::fun(&kl::Mouse::normalized_position), "normalized_position" },
		{ chaiscript::fun(&kl::Mouse::scroll), "scroll" },
	});
	INTERP_SCRIPT_IDENTIFIERS["Mouse"] = "Class representing a physical mouse.";
	INTERP_SCRIPT_IDENTIFIERS["set_hidden"] = "Sets the mouse hidden state.";
	INTERP_SCRIPT_IDENTIFIERS["is_hidden"] = "Returns the mouse hidden state.";
	INTERP_SCRIPT_IDENTIFIERS["normalized_position"] = "Returns the normalized position.";
	INTERP_SCRIPT_IDENTIFIERS["scroll"] = "Returns the mouse scroll value.";

	return 0;
}();

int load_constants = [&]
{
	using namespace titian;

	// Numbers
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::PI), "PI");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_RADIANS), "TO_RADIANS");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_DEGREES), "TO_DEGREES");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_FLOAT_COLOR), "TO_FLOAT_COLOR");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_BYTE_COLOR), "TO_BYTE_COLOR");
	INTERP_SCRIPT_IDENTIFIERS["PI"] = "Constant of a number PI (3.1415...).";
	INTERP_SCRIPT_IDENTIFIERS["TO_RADIANS"] = "Constant for converting degrees to radians (PI / 180).";
	INTERP_SCRIPT_IDENTIFIERS["TO_DEGREES"] = "Constant for converting degrees to radians (180 / PI).";
	INTERP_SCRIPT_IDENTIFIERS["TO_FLOAT_COLOR"] = "Constant for converting byte color to float color (1 / 255).";
	INTERP_SCRIPT_IDENTIFIERS["TO_BYTE_COLOR"] = "Constant for converting float color to byte color (255 / 1).";

	// Colors
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CONSOLE), "CONSOLE");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::BLACK), "BLACK");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::WHITE), "WHITE");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::GRAY), "GRAY");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::RED), "RED");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::GREEN), "GREEN");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::BLUE), "BLUE");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CYAN), "CYAN");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::PURPLE), "PURPLE");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::YELLOW), "YELLOW");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::ORANGE), "ORANGE");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::MAGENTA), "MAGENTA");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CRIMSON), "CRIMSON");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::WHEAT), "WHEAT");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::SKY), "SKY");
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
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var<int>(D3D_PRIMITIVE_TOPOLOGY_POINTLIST), "TOPOLOGY_POINTS");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var<int>(D3D_PRIMITIVE_TOPOLOGY_LINELIST), "TOPOLOGY_LINES");
	INTERP_SCRIPT_MODULE->add_global_const(chaiscript::const_var<int>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "TOPOLOGY_TRIANGLES");
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_POINTS"] = "Makes the rasterizer render this mesh as an array of POINTS.";
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_LINES"] = "Makes the rasterizer render this mesh as an array of LINES.";
	INTERP_SCRIPT_IDENTIFIERS["TOPOLOGY_TRIANGLES"] = "Makes the rasterizer render this mesh as an array of TRIANGLES.";

	return 0;
}();

int load_functions = [&]
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

	// Logging
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&Logger::log<const std::string&>), "log");
	INTERP_SCRIPT_MODULE->eval("global print = fun(object) { return log(to_string(object)); }");
	INTERP_SCRIPT_IDENTIFIERS["log"] = "Outputs the given string to the log window.";
	INTERP_SCRIPT_IDENTIFIERS["print"] = "Converts the given object to a string and logs it.";

	// Time
	INTERP_SCRIPT_MODULE->add(chaiscript::fun([&]() -> float { return GameLayer::BOUND_SELF->app_layer->timer->elapsed(); }), "get_elapsed_t");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun([&]() -> float { return GameLayer::BOUND_SELF->app_layer->timer->delta(); }), "get_delta_t");
	INTERP_SCRIPT_IDENTIFIERS["get_elapsed_t"] = "Returns the elapsed time since the game start.";
	INTERP_SCRIPT_IDENTIFIERS["get_delta_t"] = "Returns the time since the last frame.";

	// Input
	INTERP_SCRIPT_MODULE->add(chaiscript::fun([&]() -> kl::Keyboard* { return &GameLayer::BOUND_SELF->app_layer->window->keyboard; }), "get_keyboard");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun([&]() -> kl::Mouse* { return &GameLayer::BOUND_SELF->app_layer->window->mouse; }), "get_mouse");
	INTERP_SCRIPT_IDENTIFIERS["get_keyboard"] = "Returns a reference to the window keyboard.";
	INTERP_SCRIPT_IDENTIFIERS["get_mouse"] = "Returns a reference to the window mouse.";

	// Float3x3
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::translation), "Float3x3_translation");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::rotation), "Float3x3_rotation");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::scaling), "Float3x3_scaling");
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_translation"] = "Creates a 3x3 translation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_rotation"] = "Creates a 3x3 rotation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float3x3_scaling"] = "Creates a 3x3 scaling matrix.";

	// Float4x4
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::translation), "Float4x4_translation");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::rotation), "Float4x4_rotation");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::scaling), "Float4x4_scaling");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::look_at), "Float4x4_look_at");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::perspective), "Float4x4_perspective");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::orthographic), "Float4x4_orthographic");
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_translation"] = "Creates a 4x4 translation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_rotation"] = "Creates a 4x4 rotation matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_scaling"] = "Creates a 4x4 scaling matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_look_at"] = "Creates a 4x4 look at matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_perspective"] = "Creates a 4x4 perspective matrix.";
	INTERP_SCRIPT_IDENTIFIERS["Float4x4_orthographic"] = "Creates a 4x4 orthographic matrix.";

	// Math
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<int, int>(&std::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::abs), "abs");
	INTERP_SCRIPT_IDENTIFIERS["abs"] = "Returns absolute value of the given value.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::sin), "sin");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::cos), "cos");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::tan), "tan");
	INTERP_SCRIPT_IDENTIFIERS["sin"] = "Returns sin of the given angle in radians.";
	INTERP_SCRIPT_IDENTIFIERS["cos"] = "Returns cos of the given angle in radians.";
	INTERP_SCRIPT_IDENTIFIERS["tan"] = "Returns tan of the given angle in radians.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::asin), "asin");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::acos), "acos");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float>(&std::atan), "atan");
	INTERP_SCRIPT_IDENTIFIERS["asin"] = "Returns angle in radians of the given sin.";
	INTERP_SCRIPT_IDENTIFIERS["acos"] = "Returns angle in radians of the given cos.";
	INTERP_SCRIPT_IDENTIFIERS["atan"] = "Returns angle in radians of the given tan.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::sin_deg), "sin_deg");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::cos_deg), "cos_deg");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::tan_deg), "tan_deg");
	INTERP_SCRIPT_IDENTIFIERS["sin_deg"] = "Returns sin of the given angle in degrees.";
	INTERP_SCRIPT_IDENTIFIERS["cos_deg"] = "Returns cos of the given angle in degrees.";
	INTERP_SCRIPT_IDENTIFIERS["tan_deg"] = "Returns tan of the given angle in degrees.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::asin_deg), "asin_deg");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::acos_deg), "acos_deg");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::atan_deg), "atan_deg");
	INTERP_SCRIPT_IDENTIFIERS["asin_deg"] = "Returns angle in degrees of the given sin.";
	INTERP_SCRIPT_IDENTIFIERS["acos_deg"] = "Returns angle in degrees of the given cos.";
	INTERP_SCRIPT_IDENTIFIERS["atan_deg"] = "Returns angle in degrees of the given tan.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::calc_ndc), "calc_ndc");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::calc_ndc_ar), "calc_ndc_ar");
	INTERP_SCRIPT_IDENTIFIERS["calc_ndc"] = "Converts coorinates into ndc.";
	INTERP_SCRIPT_IDENTIFIERS["calc_ndc_ar"] = "Converts coorinates into ndc and applies aspect ratio.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::line_x), "line_x");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::line_y), "line_y");
	INTERP_SCRIPT_IDENTIFIERS["line_x"] = "Calculates line x.";
	INTERP_SCRIPT_IDENTIFIERS["line_y"] = "Calculates line y.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::wrap), "wrap");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::unwrap), "unwrap");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::clamp), "clamp");
	INTERP_SCRIPT_IDENTIFIERS["wrap"] = "Wraps the given value to the range [0, 1].";
	INTERP_SCRIPT_IDENTIFIERS["unwrap"] = "Unwraps the given value to the defined range.";
	INTERP_SCRIPT_IDENTIFIERS["clamp"] = "Clamps the given value between the given min and max.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::to_quaternion), "to_quaternion");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::to_euler), "to_euler");
	INTERP_SCRIPT_IDENTIFIERS["to_quaternion"] = "Converts the given euler angles to a quaternion.";
	INTERP_SCRIPT_IDENTIFIERS["to_euler"] = "Converts the given quaternion to euler angles.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::inverse), "inverse");
	INTERP_SCRIPT_IDENTIFIERS["abs"] = "Returns the absolute value.";
	INTERP_SCRIPT_IDENTIFIERS["normalize"] = "Normalizes the given vector.";
	INTERP_SCRIPT_IDENTIFIERS["inverse"] = "Returns the inverse of the given math object.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::inverse), "inverse");

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Int2, const kl::Int2&>(&kl::abs), "abs");

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float2&>(&kl::dot), "dot");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, kl::Float2, kl::Float2, bool>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::rotate), "rotate");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::reflect), "reflect");
	INTERP_SCRIPT_IDENTIFIERS["dot"] = "Returns the dot product of two vectors.";
	INTERP_SCRIPT_IDENTIFIERS["angle"] = "Returns the angle in degrees between two vectors.";
	INTERP_SCRIPT_IDENTIFIERS["rotate"] = "Returns the rotated vector.";
	INTERP_SCRIPT_IDENTIFIERS["reflect"] = "Returns the reflected vector.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float3&>(&kl::dot), "dot");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float3&, const kl::Float3&>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::rotate), "rotate");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::reflect), "reflect");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::cross), "cross");
	INTERP_SCRIPT_IDENTIFIERS["cross"] = "Returns the cross product of two vectors.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4, const kl::Float4&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4, const kl::Float4&>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float4&>(&kl::dot), "dot");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float4&, const kl::Float4&>(&kl::angle), "angle");

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::transpose), "transpose");
	INTERP_SCRIPT_IDENTIFIERS["transpose"] = "Returns the transposed matrix.";

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::transpose), "transpose");

	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::transpose), "transpose");

	// Random
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<bool>(&kl::random::gen_bool), "gen_random_bool");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<int, int, int>(&kl::random::gen_int), "gen_random_int");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<float, float, float>(&kl::random::gen_float), "gen_random_float");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, float, float>(&kl::random::gen_float2), "gen_random_float2");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, float, float>(&kl::random::gen_float3), "gen_random_float3");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4, float, float>(&kl::random::gen_float4), "gen_random_float4");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::random::gen_char), "gen_random_char");
	INTERP_SCRIPT_MODULE->add(chaiscript::fun(&kl::random::gen_string), "gen_random_string");
	INTERP_SCRIPT_IDENTIFIERS["gen_random_bool"] = "Generates a random bool.";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_int"] = "Generates a random integer (start_inclusive, end_exclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float"] = "Generates a random float (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float2"] = "Generates a random float2 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float3"] = "Generates a random float3 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_float4"] = "Generates a random float4 (start_inclusive, end_inclusive).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_char"] = "Generates a random char (can_be_upper).";
	INTERP_SCRIPT_IDENTIFIERS["gen_random_string"] = "Generates a random string (length).";

	return 0;
}();
