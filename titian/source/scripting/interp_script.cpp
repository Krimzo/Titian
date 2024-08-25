#include "titian.h"


/* Inter script */
titian::InterpScript::InterpScript()
{
	this->source = get_default_script();
}

void titian::InterpScript::serialize(Serializer* serializer, const void* helper_data) const
{
	Script::serialize(serializer, helper_data);

	serializer->write_string("source", source);
}

void titian::InterpScript::deserialize(const Serializer* serializer, const void* helper_data)
{
	Script::deserialize(serializer, helper_data);

	serializer->read_string("source", source);

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
	catch (Exception& e) {
		Logger::log(e.what());
	}

	try {
		m_start_function = m_engine->eval<Function<void(Scene*)>>("on_start");
	}
	catch (Exception&)
	{}

	try {
		m_update_function = m_engine->eval<Function<void(Scene*)>>("on_update");
	}
	catch (Exception&)
	{}

	try {
		m_collision_function = m_engine->eval<Function<void(Scene*, Entity*, Entity*)>>("on_collision");
	}
	catch (Exception&)
	{}

	try {
		m_ui_function = m_engine->eval<Function<void(Scene*)>>("on_ui");
	}
	catch (Exception&)
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
	catch (Exception& e) {
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
	catch (Exception& e) {
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
	catch (Exception& e) {
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
	catch (Exception& e) {
		Logger::log(e.what());
	}
}

titian::StringMap<cs::Boxed_Value> titian::InterpScript::get_parameters()
{
	if (!m_engine) {
		return {};
	}

	StringMap<cs::Boxed_Value> result;
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
	cs::bootstrap::standard_library::vector_type<Vector<String>>("StringData", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<kl::Vertex<float>>>("MeshData", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<Color>>("TextureData", *INTERP_SCRIPT_MODULE);

	cs::bootstrap::standard_library::vector_type<Vector<Mesh*>>("MeshVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<Animation*>>("AnimationVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<Texture*>>("TextureVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<Material*>>("MaterialVector", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::vector_type<Vector<Entity*>>("EntityVector", *INTERP_SCRIPT_MODULE);

	cs::bootstrap::standard_library::map_type<StringMap<Mesh*>>("MeshMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<StringMap<Animation*>>("AnimationMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<StringMap<Texture*>>("TextureMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<StringMap<Material*>>("MaterialMap", *INTERP_SCRIPT_MODULE);
	cs::bootstrap::standard_library::map_type<StringMap<Entity*>>("EntityMap", *INTERP_SCRIPT_MODULE);

	// Derived
	INTERP_SCRIPT_MODULE->add(cs::base_class<Entity, Camera>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Entity, Light>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, AmbientLight>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, PointLight>());
	INTERP_SCRIPT_MODULE->add(cs::base_class<Light, DirectionalLight>());

	// Casts
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Int2&, Float2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float2&, Int2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float2&, Complex>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float3&, Color>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float3&, Quaternion>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float4&, Color>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Float4&, Quaternion>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Complex&, Float2>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Quaternion&, Float3>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Quaternion&, Float4>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Color&, Float3>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const Color&, Float4>());
	INTERP_SCRIPT_MODULE->add(cs::type_conversion<const String&, StringView>());

	// Int2
	cs::utility::add_class<Int2>(*INTERP_SCRIPT_MODULE, "Int2",
	{
		cs::constructor<Int2()>(),
		cs::constructor<Int2(const Int2&)>(),

		cs::constructor<Int2(int)>(),
		cs::constructor<Int2(int, int)>(),
	},
	{
		{ cs::fun(&Int2::x), "x" },
		{ cs::fun(&Int2::y), "y" },

		{ cs::fun<Int2 & (Int2::*)(const Int2&)>(&Int2::operator=), "=" },

		{ cs::fun<int& (Int2::*)(int)>(&Int2::operator[]), "[]"},

		{ cs::fun(&Int2::operator==), "==" },
		{ cs::fun(&Int2::operator!=), "!=" },

		{ cs::fun(&Int2::operator+), "+" },
		{ cs::fun(&Int2::operator+=), "+=" },

		{ cs::fun<Int2(Int2::*)(const Int2&) const>(&Int2::operator-), "-"},
		{ cs::fun(&Int2::operator-=), "-=" },

		{ cs::fun<Int2 (Int2::*)(int) const>(&Int2::operator*), "*" },
		{ cs::fun<void (Int2::*)(int)>(&Int2::operator*=), "*=" },

		{ cs::fun<Int2 (Int2::*)(const Int2&) const>(&Int2::operator*), "*" },
		{ cs::fun<void (Int2::*)(const Int2&)>(&Int2::operator*=), "*=" },

		{ cs::fun<Int2 (Int2::*)(int) const>(&Int2::operator/), "/" },
		{ cs::fun<void (Int2::*)(int)>(&Int2::operator/=), "/=" },

		{ cs::fun<Int2 (Int2::*)(const Int2&) const>(&Int2::operator/), "/" },
		{ cs::fun<void (Int2::*)(const Int2&)>(&Int2::operator/=), "/=" },

		{ cs::fun<Int2(Int2::*)() const>(&Int2::operator-), "-" },

		{ cs::fun([](const Int2& object) { return kl::format(object); }), "to_string" },
	});

	// Float2
	cs::utility::add_class<Float2>(*INTERP_SCRIPT_MODULE, "Float2",
	{
		cs::constructor<Float2()>(),
		cs::constructor<Float2(const Float2&)>(),

		cs::constructor<Float2(float)>(),
		cs::constructor<Float2(float, float)>(),
	},
	{
		{ cs::fun(&Float2::x), "x" },
		{ cs::fun(&Float2::y), "y" },

		{ cs::fun<Float2& (Float2::*)(const Float2&)>(&Float2::operator=), "=" },

		{ cs::fun<float& (Float2::*)(int)>(&Float2::operator[]), "[]"},

		{ cs::fun(&Float2::operator==), "==" },
		{ cs::fun(&Float2::operator!=), "!=" },

		{ cs::fun(&Float2::operator+), "+" },
		{ cs::fun(&Float2::operator+=), "+=" },

		{ cs::fun<Float2 (Float2::*)(const Float2&) const>(&Float2::operator-), "-"},
		{ cs::fun(&Float2::operator-=), "-=" },

		{ cs::fun<Float2 (Float2::*)(float) const>(&Float2::operator*), "*" },
		{ cs::fun<void (Float2::*)(float)>(&Float2::operator*=), "*="},

		{ cs::fun<Float2 (Float2::*)(const Float2&) const>(&Float2::operator*), "*" },
		{ cs::fun<void (Float2::*)(const Float2&)>(&Float2::operator*=), "*=" },

		{ cs::fun<Float2 (Float2::*)(float) const>(&Float2::operator/), "/" },
		{ cs::fun<void (Float2::*)(float)>(&Float2::operator/=), "/=" },

		{ cs::fun<Float2 (Float2::*)(const Float2&) const>(&Float2::operator/), "/" },
		{ cs::fun<void (Float2::*)(const Float2&)>(&Float2::operator/=), "/=" },

		{ cs::fun<Float2 (Float2::*)() const>(&Float2::operator-), "-"},
		{ cs::fun(&Float2::length), "length" },

		{ cs::fun([](const Float2& object) { return kl::format(object); }), "to_string" },
	});

	// Float3
	cs::utility::add_class<Float3>(*INTERP_SCRIPT_MODULE, "Float3",
	{
		cs::constructor<Float3()>(),
		cs::constructor<Float3(const Float3&)>(),

		cs::constructor<Float3(float)>(),
		cs::constructor<Float3(float, float, float)>(),

		cs::constructor<Float3(Float2, float)>(),
		cs::constructor<Float3(float, Float2)>(),
	},
	{
		{ cs::fun(&Float3::x), "x" },
		{ cs::fun(&Float3::y), "y" },
		{ cs::fun(&Float3::z), "z" },

		{ cs::fun<Float3& (Float3::*)(const Float3&)>(&Float3::operator=), "=" },

		{ cs::fun<Float2 & (Float3::*)()>(&Float3::xy), "xy"},
		{ cs::fun<Float2& (Float3::*)()>(&Float3::yz), "yz" },

		{ cs::fun<float& (Float3::*)(int)>(&Float3::operator[]), "[]"},

		{ cs::fun(&Float3::operator==), "==" },
		{ cs::fun(&Float3::operator!=), "!=" },

		{ cs::fun(&Float3::operator+), "+" },
		{ cs::fun(&Float3::operator+=), "+=" },

		{ cs::fun<Float3 (Float3::*)(const Float3&) const>(&Float3::operator-), "-" },
		{ cs::fun(&Float3::operator-=), "-=" },

		{ cs::fun<Float3 (Float3::*)(float) const>(&Float3::operator*), "*" },
		{ cs::fun<void (Float3::*)(float)>(&Float3::operator*=), "*=" },

		{ cs::fun<Float3 (Float3::*)(const Float3&) const>(&Float3::operator*), "*" },
		{ cs::fun<void (Float3::*)(const Float3&)>(&Float3::operator*=), "*=" },

		{ cs::fun<Float3 (Float3::*)(float) const>(&Float3::operator/), "/" },
		{ cs::fun<void (Float3::*)(float)>(&Float3::operator/=), "/=" },

		{ cs::fun<Float3 (Float3::*)(const Float3&) const>(&Float3::operator/), "/" },
		{ cs::fun<void (Float3::*)(const Float3&)>(&Float3::operator/=), "/=" },

		{ cs::fun<Float3 (Float3::*)() const>(&Float3::operator-), "-"},
		{ cs::fun(&Float3::length), "length" },

		{ cs::fun([](const Float3& object) { return kl::format(object); }), "to_string" },
	});

	// Float4
	cs::utility::add_class<Float4>(*INTERP_SCRIPT_MODULE, "Float4",
	{
		cs::constructor<Float4()>(),
		cs::constructor<Float4(const Float4&)>(),

		cs::constructor<Float4(float)>(),
		cs::constructor<Float4(float, float, float, float)>(),

		cs::constructor<Float4(Float2, float, float)>(),
		cs::constructor<Float4(float, Float2, float)>(),
		cs::constructor<Float4(float, float, Float2)>(),
		cs::constructor<Float4(Float2, Float2)>(),

		cs::constructor<Float4(Float3, float)>(),
		cs::constructor<Float4(float, Float3)>(),
	},
	{
		{ cs::fun(&Float4::x), "x" },
		{ cs::fun(&Float4::y), "y" },
		{ cs::fun(&Float4::z), "z" },
		{ cs::fun(&Float4::w), "w" },

		{ cs::fun<Float4& (Float4::*)(const Float4&)>(&Float4::operator=), "=" },

		{ cs::fun<Float2& (Float4::*)()>(&Float4::xy), "xy" },
		{ cs::fun<Float2& (Float4::*)()>(&Float4::yz), "yz" },
		{ cs::fun<Float2& (Float4::*)()>(&Float4::zw), "zw" },

		{ cs::fun<Float3& (Float4::*)()>(&Float4::xyz), "xyz" },
		{ cs::fun<Float3& (Float4::*)()>(&Float4::yzw), "yzw" },

		{ cs::fun<float& (Float4::*)(int)>(&Float4::operator[]), "[]"},

		{ cs::fun(&Float4::operator==), "==" },
		{ cs::fun(&Float4::operator!=), "!=" },

		{ cs::fun(&Float4::operator+), "+" },
		{ cs::fun(&Float4::operator+=), "+=" },

		{ cs::fun<Float4 (Float4::*)(const Float4&) const>(&Float4::operator-), "-"},
		{ cs::fun(&Float4::operator-=), "-=" },

		{ cs::fun<Float4 (Float4::*)(float) const>(&Float4::operator*), "*" },
		{ cs::fun<void (Float4::*)(float)>(&Float4::operator*=), "*=" },

		{ cs::fun<Float4 (Float4::*)(const Float4&) const>(&Float4::operator*), "*" },
		{ cs::fun<void (Float4::*)(const Float4&)>(&Float4::operator*=), "*=" },

		{ cs::fun<Float4 (Float4::*)(float) const>(&Float4::operator/), "/" },
		{ cs::fun<void (Float4::*)(float)>(&Float4::operator/=), "/=" },

		{ cs::fun<Float4 (Float4::*)(const Float4&) const>(&Float4::operator/), "/" },
		{ cs::fun<void (Float4::*)(const Float4&)>(&Float4::operator/=), "/="},

		{ cs::fun<Float4 (Float4::*)() const>(&Float4::operator-), "-"},
		{ cs::fun(&Float4::length), "length" },
		
		{ cs::fun([](const Float4& object) { return kl::format(object); }), "to_string" },
	});

	// Float2x2
	cs::utility::add_class<Float2x2>(*INTERP_SCRIPT_MODULE, "Float2x2",
	{
		cs::constructor<Float2x2()>(),
		cs::constructor<Float2x2(const Float2x2&)>(),
	},
	{
		{ cs::fun<Float2x2& (Float2x2::*)(const Float2x2&)>(&Float2x2::operator=), "=" },

		{ cs::fun<float& (Float2x2::*)(int)>(&Float2x2::operator[]), "[]"},

		{ cs::fun(&Float2x2::operator==), "==" },
		{ cs::fun(&Float2x2::operator!=), "!=" },

		{ cs::fun(&Float2x2::operator+), "+" },
		{ cs::fun(&Float2x2::operator+=), "+=" },

		{ cs::fun(&Float2x2::operator-), "-" },
		{ cs::fun(&Float2x2::operator-=), "-=" },

		{ cs::fun<Float2x2 (Float2x2::*)(float) const>(&Float2x2::operator*), "*" },
		{ cs::fun<void (Float2x2::*)(float)>(&Float2x2::operator*=), "*=" },

		{ cs::fun<Float2x2 (Float2x2::*)(const Float2x2&) const>(&Float2x2::operator*), "*" },
		{ cs::fun<void (Float2x2::*)(const Float2x2&)>(&Float2x2::operator*=), "*=" },

		{ cs::fun<Float2 (Float2x2::*)(const Float2&) const>(&Float2x2::operator*), "*"},

		{ cs::fun(&Float2x2::determinant), "determinant" },

		{ cs::fun([](const Float2x2& object) { return kl::format(object); }), "to_string" },
	});

	// Float3x3
	cs::utility::add_class<Float3x3>(*INTERP_SCRIPT_MODULE, "Float3x3",
	{
		cs::constructor<Float3x3()>(),
		cs::constructor<Float3x3(const Float3x3&)>(),
	},
	{
		{ cs::fun<Float3x3& (Float3x3::*)(const Float3x3&)>(&Float3x3::operator=), "=" },

		{ cs::fun<float& (Float3x3::*)(int)>(&Float3x3::operator[]), "[]"},

		{ cs::fun(&Float3x3::operator==), "==" },
		{ cs::fun(&Float3x3::operator!=), "!=" },

		{ cs::fun(&Float3x3::operator+), "+" },
		{ cs::fun(&Float3x3::operator+=), "+=" },

		{ cs::fun(&Float3x3::operator-), "-" },
		{ cs::fun(&Float3x3::operator-=), "-=" },

		{ cs::fun<Float3x3 (Float3x3::*)(float) const>(&Float3x3::operator*), "*" },
		{ cs::fun<void (Float3x3::*)(float)>(&Float3x3::operator*=), "*=" },

		{ cs::fun<Float3x3 (Float3x3::*)(const Float3x3&) const>(&Float3x3::operator*), "*" },
		{ cs::fun<void (Float3x3::*)(const Float3x3&)>(&Float3x3::operator*=), "*=" },

		{ cs::fun<Float3 (Float3x3::*)(const Float3&) const>(&Float3x3::operator*), "*"},

		{ cs::fun(&Float3x3::determinant), "determinant" },

		{ cs::fun([](const Float3x3& object) { return kl::format(object); }), "to_string" },
	});

	// Float4x4
	cs::utility::add_class<Float4x4>(*INTERP_SCRIPT_MODULE, "Float4x4",
	{
		cs::constructor<Float4x4()>(),
		cs::constructor<Float4x4(const Float4x4&)>(),
	},
	{
		{ cs::fun<Float4x4& (Float4x4::*)(const Float4x4&)>(&Float4x4::operator=), "=" },

		{ cs::fun<float& (Float4x4::*)(int)>(&Float4x4::operator[]), "[]"},

		{ cs::fun(&Float4x4::operator==), "==" },
		{ cs::fun(&Float4x4::operator!=), "!=" },

		{ cs::fun(&Float4x4::operator+), "+" },
		{ cs::fun(&Float4x4::operator+=), "+=" },

		{ cs::fun(&Float4x4::operator-), "-" },
		{ cs::fun(&Float4x4::operator-=), "-=" },

		{ cs::fun<Float4x4 (Float4x4::*)(float) const>(&Float4x4::operator*), "*" },
		{ cs::fun<void (Float4x4::*)(float)>(&Float4x4::operator*=), "*=" },

		{ cs::fun<Float4x4 (Float4x4::*)(const Float4x4&) const>(&Float4x4::operator*), "*" },
		{ cs::fun<void (Float4x4::*)(const Float4x4&)>(&Float4x4::operator*=), "*=" },

		{ cs::fun<Float4 (Float4x4::*)(const Float4&) const>(&Float4x4::operator*), "*"},

		{ cs::fun(&Float4x4::determinant), "determinant" },

		{ cs::fun([](const Float4x4& object) { return kl::format(object); }), "to_string" },
	});

	// Complex
	cs::utility::add_class<Complex>(*INTERP_SCRIPT_MODULE, "Complex",
	{
		cs::constructor<Complex()>(),
		cs::constructor<Complex(const Complex&)>(),

		cs::constructor<Complex(float, float)>(),
		cs::constructor<Complex(float)>(),
	},
	{
		{ cs::fun(&Complex::r), "r" },
		{ cs::fun(&Complex::i), "i" },

		{ cs::fun<Complex& (Complex::*)(const Complex&)>(&Complex::operator=), "=" },

		{ cs::fun<float& (Complex::*)(int)>(&Complex::operator[]), "[]"},

		{ cs::fun(&Complex::operator==), "==" },
		{ cs::fun(&Complex::operator!=), "!=" },

		{ cs::fun(&Complex::operator+), "+" },
		{ cs::fun(&Complex::operator+=), "+=" },

		{ cs::fun<Complex (Complex::*)(const Complex&) const>(&Complex::operator-), "-" },
		{ cs::fun(&Complex::operator-=), "-=" },

		{ cs::fun<Complex (Complex::*)(float) const>(&Complex::operator*), "*" },
		{ cs::fun<void (Complex::*)(float)>(&Complex::operator*=), "*=" },

		{ cs::fun<Complex (Complex::*)(const Complex&) const>(&Complex::operator*), "*" },
		{ cs::fun<void (Complex::*)(const Complex&)>(&Complex::operator*=), "*="},

		{ cs::fun<Complex (Complex::*)() const>(&Complex::operator-), "-" },
		{ cs::fun(&Complex::length), "length" },

		{ cs::fun([](const Complex& object) { return kl::format(object); }), "to_string" },
	});

	// Quaternion
	cs::utility::add_class<Quaternion>(*INTERP_SCRIPT_MODULE, "Quaternion",
	{
		cs::constructor<Quaternion()>(),
		cs::constructor<Quaternion(const Quaternion&)>(),

		cs::constructor<Quaternion(float, float, float)>(),
		cs::constructor<Quaternion(float, float, float, float)>(),
		cs::constructor<Quaternion(Float3, float)>(),
	},
	{
		{ cs::fun(&Quaternion::w), "w" },
		{ cs::fun(&Quaternion::x), "x" },
		{ cs::fun(&Quaternion::y), "y" },
		{ cs::fun(&Quaternion::z), "z" },

		{ cs::fun<Quaternion& (Quaternion::*)(const Quaternion&)>(&Quaternion::operator=), "=" },

		{ cs::fun<float& (Quaternion::*)(int)>(&Quaternion::operator[]), "[]"},

		{ cs::fun(&Quaternion::operator==), "==" },
		{ cs::fun(&Quaternion::operator!=), "!=" },

		{ cs::fun(&Quaternion::operator+), "+" },
		{ cs::fun(&Quaternion::operator+=), "+=" },

		{ cs::fun<Quaternion(Quaternion::*)(const Quaternion&) const>(&Quaternion::operator-), "-" },
		{ cs::fun(&Quaternion::operator-=), "-=" },

		{ cs::fun<Quaternion(Quaternion::*)(float) const>(&Quaternion::operator*), "*" },
		{ cs::fun<void (Quaternion::*)(float)>(&Quaternion::operator*=), "*=" },

		{ cs::fun<Quaternion (Quaternion::*)(const Quaternion&) const>(&Quaternion::operator*), "*" },
		{ cs::fun<void (Quaternion::*)(const Quaternion&)>(&Quaternion::operator*=), "*=" },

		{ cs::fun<Quaternion (Quaternion::*)() const>(&Quaternion::operator-), "-" },
		{ cs::fun(&Quaternion::length), "length" },

		{ cs::fun([](const Quaternion& object) { return kl::format(object); }), "to_string" },
	});

	// Vertex
	cs::utility::add_class<kl::Vertex<float>>(*INTERP_SCRIPT_MODULE, "Vertex",
	{
		cs::constructor<kl::Vertex<float>()>(),
		cs::constructor<kl::Vertex<float>(const kl::Vertex<float>&)>(),

		cs::constructor<kl::Vertex<float>(Float3)>(),
		cs::constructor<kl::Vertex<float>(Float3, Float2)>(),
		cs::constructor<kl::Vertex<float>(Float3, Float3)>(),
		cs::constructor<kl::Vertex<float>(Float3, Float2, Float3)>(),
	},
	{
		{ cs::fun(&kl::Vertex<float>::world), "world" },
		{ cs::fun(&kl::Vertex<float>::normal), "normal" },
		{ cs::fun(&kl::Vertex<float>::texture), "texture" },

		{ cs::fun<kl::Vertex<float>& (kl::Vertex<float>::*)(const kl::Vertex<float>&)>(&kl::Vertex<float>::operator=), "=" },

		{ cs::fun([](const kl::Vertex<float>& object) { return kl::format(object); }), "to_string" },
	});

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

	// Plane
	cs::utility::add_class<kl::Plane<float>>(*INTERP_SCRIPT_MODULE, "Plane",
	{
		cs::constructor<kl::Plane<float>()>(),
		cs::constructor<kl::Plane<float>(const kl::Plane<float>&)>(),

		cs::constructor<kl::Plane<float>(Float3, Float3)>(),
	},
	{
		{ cs::fun(&kl::Plane<float>::origin), "origin" },

		{ cs::fun<kl::Plane<float>& (kl::Plane<float>::*)(const kl::Plane<float>&)>(&kl::Plane<float>::operator=), "=" },

		{ cs::fun(&kl::Plane<float>::set_normal), "set_normal" },
		{ cs::fun(&kl::Plane<float>::normal), "normal" },

		{ cs::fun(&kl::Plane<float>::in_front), "in_front" },

		{ cs::fun([](const kl::Plane<float>& object) { return kl::format(object); }), "to_string" },
	});

	// Sphere
	cs::utility::add_class<kl::Sphere<float>>(*INTERP_SCRIPT_MODULE, "Sphere",
	{
		cs::constructor<kl::Sphere<float>()>(),
		cs::constructor<kl::Sphere<float>(const kl::Sphere<float>&)>(),

		cs::constructor<kl::Sphere<float>(Float3, float)>(),
	},
	{
		{ cs::fun(&kl::Sphere<float>::origin), "origin" },
		{ cs::fun(&kl::Sphere<float>::radius), "radius" },

		{ cs::fun<kl::Sphere<float>& (kl::Sphere<float>::*)(const kl::Sphere<float>&)>(&kl::Sphere<float>::operator=), "=" },

		{ cs::fun([](const kl::Sphere<float>& object) { return kl::format(object); }), "to_string" },
	});

	// Ray
	cs::utility::add_class<kl::Ray<float>>(*INTERP_SCRIPT_MODULE, "Ray",
	{
		cs::constructor<kl::Ray<float>()>(),
		cs::constructor<kl::Ray<float>(const kl::Ray<float>&)>(),

		cs::constructor<kl::Ray<float>(Float3, Float3)>(),
		cs::constructor<kl::Ray<float>(Float3, Float4x4, Float2)>(),
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

	// Color
	cs::utility::add_class<Color>(*INTERP_SCRIPT_MODULE, "Color",
	{
		cs::constructor<Color()>(),
		cs::constructor<Color(const Color&)>(),

		cs::constructor<Color(byte, byte, byte)>(),
		cs::constructor<Color(byte, byte, byte, byte)>(),
	},
	{
		{ cs::fun(&Color::r), "r" },
		{ cs::fun(&Color::g), "g" },
		{ cs::fun(&Color::b), "b" },
		{ cs::fun(&Color::a), "a" },

		{ cs::fun<Color& (Color::*)(const Color&)>(&Color::operator=), "=" },

		{ cs::fun(&Color::operator==), "==" },
		{ cs::fun(&Color::operator!=), "!=" },

		{ cs::fun(&Color::gray), "gray" },
		{ cs::fun(&Color::inverted), "inverted" },
		{ cs::fun(&Color::as_ascii), "as_ascii" },

		{ cs::fun<Color (Color::*)(const Color&, float) const>(&Color::mix), "mix" },
		{ cs::fun<Color (Color::*)(const Color&) const>(&Color::mix), "mix" },

		{ cs::fun([](const Color& object) {
			return kl::format('(', int(object.r),
				", ", int(object.g),
				", ", int(object.b),
				", ", int(object.a), ')');
		}), "to_string" },
	});

	// Mesh
	cs::utility::add_class<Mesh>(*INTERP_SCRIPT_MODULE, "Mesh",
	{},
	{
		{ cs::fun(&Mesh::data_buffer), "data_buffer" },

		{ cs::fun(&Mesh::topology), "topology"},
		{ cs::fun(&Mesh::render_wireframe), "render_wireframe" },

		{ cs::fun(&Mesh::reload), "reload" },
	});

	// Animation
	cs::utility::add_class<Animation>(*INTERP_SCRIPT_MODULE, "Animation",
	{},
	{
		{ cs::fun(&Animation::animation_type), "animation_type" },
		{ cs::fun(&Animation::ticks_per_second), "ticks_per_second" },
		{ cs::fun(&Animation::duration_in_ticks), "duration_in_ticks" },

		{ cs::fun(&Animation::meshes), "meshes" },

		{ cs::fun(&Animation::get_index), "get_index" },
		{ cs::fun(&Animation::get_mesh), "get_mesh" },

		{ cs::fun(&Animation::update), "update" },
	});

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

	// Shader
	cs::utility::add_class<Shader>(*INTERP_SCRIPT_MODULE, "Shader",
	{},
	{
		{ cs::fun(&Shader::shader_type), "shader_type" },
		{ cs::fun(&Shader::data_buffer), "data_buffer" },

		{ cs::fun(&Shader::reload), "reload" },
	});

	// Entity
	cs::utility::add_class<Entity>(*INTERP_SCRIPT_MODULE, "Entity",
	{},
	{
		{ cs::fun(&Entity::scale), "scale" },
		{ cs::fun(&Entity::casts_shadows), "casts_shadows" },

		{ cs::fun(&Entity::animation_name), "animation_name" },
		{ cs::fun(&Entity::material_name), "material_name" },
		{ cs::fun(&Entity::collider_mesh_name), "collider_mesh_name" },

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

	// Camera
	cs::utility::add_class<Camera>(*INTERP_SCRIPT_MODULE, "Camera",
	{},
	{
		{ cs::fun(&Camera::camera_type), "camera_type" },

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

	// Light
	cs::utility::add_class<Light>(*INTERP_SCRIPT_MODULE, "Light",
	{},
	{
		{ cs::fun(&Light::light_at_point), "light_at_point" },
	});

	// Ambient light
	cs::utility::add_class<AmbientLight>(*INTERP_SCRIPT_MODULE, "AmbientLight",
	{},
	{
		{ cs::fun(&AmbientLight::color), "color" },
		{ cs::fun(&AmbientLight::intensity), "intensity" },
	});

	// Point light
	cs::utility::add_class<PointLight>(*INTERP_SCRIPT_MODULE, "PointLight",
	{},
	{
		{ cs::fun(&PointLight::color), "color" },
	});

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
		{ cs::fun(&Scene::helper_new_animation), "new_animation" },
		{ cs::fun(&Scene::helper_new_texture), "new_texture" },
		{ cs::fun(&Scene::helper_new_material), "new_material" },
		{ cs::fun(&Scene::helper_new_shader), "new_shader" },
		{ cs::fun(&Scene::helper_new_entity), "new_entity" },

		{ cs::fun(&Scene::helper_get_mesh), "get_mesh" },
		{ cs::fun(&Scene::helper_get_animation), "get_animation" },
		{ cs::fun(&Scene::helper_get_texture), "get_texture" },
		{ cs::fun(&Scene::helper_get_material), "get_material" },
		{ cs::fun(&Scene::helper_get_shader), "get_shader" },
		{ cs::fun(&Scene::helper_get_entity), "get_entity" },

		{ cs::fun(&Scene::helper_remove_mesh), "remove_mesh" },
		{ cs::fun(&Scene::helper_remove_animation), "remove_animation" },
		{ cs::fun(&Scene::helper_remove_texture), "remove_texture" },
		{ cs::fun(&Scene::helper_remove_material), "remove_material" },
		{ cs::fun(&Scene::helper_remove_shader), "remove_shader" },
		{ cs::fun(&Scene::helper_remove_entity), "remove_entity" },

		{ cs::fun(&Scene::helper_contains_mesh), "contains_mesh" },
		{ cs::fun(&Scene::helper_contains_animation), "contains_animation" },
		{ cs::fun(&Scene::helper_contains_texture), "contains_texture" },
		{ cs::fun(&Scene::helper_contains_material), "contains_material" },
		{ cs::fun(&Scene::helper_contains_shader), "contains_shader" },
		{ cs::fun(&Scene::helper_contains_entity), "contains_entity" },

		{ cs::fun(&Scene::helper_mesh_count), "mesh_count" },
		{ cs::fun(&Scene::helper_animation_count), "animation_count" },
		{ cs::fun(&Scene::helper_texture_count), "texture_count" },
		{ cs::fun(&Scene::helper_material_count), "material_count" },
		{ cs::fun(&Scene::helper_shader_count), "shader_count" },
		{ cs::fun(&Scene::helper_entity_count), "entity_count" },

		{ cs::fun(&Scene::helper_get_all_meshes), "get_all_meshes" },
		{ cs::fun(&Scene::helper_get_all_animations), "get_all_animations" },
		{ cs::fun(&Scene::helper_get_all_textures), "get_all_textures" },
		{ cs::fun(&Scene::helper_get_all_materials), "get_all_materials" },
		{ cs::fun(&Scene::helper_get_all_shaders), "get_all_shaders" },
		{ cs::fun(&Scene::helper_get_all_entities), "get_all_entities" },

		{ cs::fun<Camera* (Scene::*)(const StringView&)>(&Scene::get_casted<Camera>), "get_camera"},
		{ cs::fun<AmbientLight* (Scene::*)(const StringView&)>(&Scene::get_casted<AmbientLight>), "get_ambient_light" },
		{ cs::fun<PointLight* (Scene::*)(const StringView&)>(&Scene::get_casted<PointLight>), "get_point_light" },
		{ cs::fun<DirectionalLight* (Scene::*)(const StringView&)>(&Scene::get_casted<DirectionalLight>), "get_directional_light" },
	});

	// Key
	cs::utility::add_class<kl::Key>(*INTERP_SCRIPT_MODULE, "Key",
	{},
	{
		{ cs::fun(&kl::Key::is_down), "is_down" },
	});

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

	// Colors
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::CONSOLE), "CONSOLE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::BLACK), "BLACK");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::WHITE), "WHITE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::GRAY), "GRAY");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::RED), "RED");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::GREEN), "GREEN");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::BLUE), "BLUE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::CYAN), "CYAN");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::PURPLE), "PURPLE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::YELLOW), "YELLOW");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::ORANGE), "ORANGE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::MAGENTA), "MAGENTA");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::CRIMSON), "CRIMSON");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::WHEAT), "WHEAT");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((Color) kl::colors::SKY), "SKY");

	// Mesh
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_POINTLIST), "TOPOLOGY_POINTS");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_LINELIST), "TOPOLOGY_LINES");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "TOPOLOGY_TRIANGLES");

	// Animation
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) AnimationType::SEQUENTIAL), "ANIMATION_SEQUENTIAL");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) AnimationType::SKELETAL), "ANIMATION_SKELETAL");

	// Shader
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) ShaderType::MATERIAL), "SHADER_MATERIAL");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) ShaderType::CAMERA), "SHADER_CAMERA");

	// Camera
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) CameraType::PERSPECTIVE), "CAMERA_PERSPECTIVE");
	INTERP_SCRIPT_MODULE->add_global_const(cs::const_var((int) CameraType::ORTHOGRAPHIC), "CAMERA_ORTHOGRAPHIC");

	return 0;
}();

const int load_functions = [&]
{
	using namespace titian;

	// Var info
	INTERP_SCRIPT_MODULE->eval("def is_null(object) { return is_var_null(object); }");
	INTERP_SCRIPT_MODULE->eval("def is_valid(object) { return !is_null(object); }");
	INTERP_SCRIPT_MODULE->eval("def get_type(object) { return type_name(object); }");

	// Logging
	INTERP_SCRIPT_MODULE->add(cs::fun(&Logger::log<const StringView&>), "log_str");
	INTERP_SCRIPT_MODULE->eval("def log(object) { return log_str(to_string(object)); }");

	// Time
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> float { return Layers::get<AppLayer>()->timer.elapsed(); }), "get_elapsed_t");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> float { return Layers::get<AppLayer>()->timer.delta(); }), "get_delta_t");

	// Input
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Keyboard* { return &Layers::get<AppLayer>()->window.keyboard; }), "get_keyboard");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Mouse* { return &Layers::get<AppLayer>()->window.mouse; }), "get_mouse");
	INTERP_SCRIPT_MODULE->add(cs::fun([&]() -> kl::Window* { return &Layers::get<AppLayer>()->window; }), "get_window");

	// Float3x3
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float3x3::translation), "Float3x3_translation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float3x3::rotation), "Float3x3_rotation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float3x3::scaling), "Float3x3_scaling");

	// Float4x4
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::translation), "Float4x4_translation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::rotation), "Float4x4_rotation");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::scaling), "Float4x4_scaling");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::look_at), "Float4x4_look_at");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::perspective), "Float4x4_perspective");
	INTERP_SCRIPT_MODULE->add(cs::fun(&Float4x4::orthographic), "Float4x4_orthographic");

	// Math
	INTERP_SCRIPT_MODULE->add(cs::fun<int (*)(int)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::abs), "abs");

	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::sin), "sin");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::cos), "cos");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::tan), "tan");

	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::asin), "asin");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::acos), "acos");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float)>(&kl::atan), "atan");

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::sin_d<float>), "sin_d");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::cos_d<float>), "cos_d");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::tan_d<float>), "tan_d");

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::asin_d<float>), "asin_d");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::acos_d<float>), "acos_d");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::atan_d<float>), "atan_d");

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::calc_ndc<float>), "calc_ndc");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::calc_ndc_ar<float>), "calc_ndc_ar");

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::line_x<float>), "line_x");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::line_y<float>), "line_y");

	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::lerp<float>), "lerp");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::unlerp<float>), "unlerp");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::clamp<float>), "clamp");

	INTERP_SCRIPT_MODULE->add(cs::fun<Quaternion (*)(const Float3&)>(&kl::to_quaternion<float>), "to_quaternion");
	INTERP_SCRIPT_MODULE->add(cs::fun<Quaternion (*)(const Float3&, const Float3&)>(&kl::to_quaternion<float>), "to_quaternion");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Quaternion&)>(&kl::to_euler<float>), "to_euler");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&, const Float3&)>(&kl::to_euler<float>), "to_euler");

	INTERP_SCRIPT_MODULE->add(cs::fun<Complex (*)(const Complex&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Complex (*)(const Complex&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<Complex (*)(const Complex&)>(&kl::inverse), "inverse");

	INTERP_SCRIPT_MODULE->add(cs::fun<Quaternion (*)(const Quaternion&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Quaternion (*)(const Quaternion&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<Quaternion (*)(const Quaternion&)>(&kl::inverse), "inverse");

	INTERP_SCRIPT_MODULE->add(cs::fun<Int2 (*)(const Int2&)>(&kl::abs), "abs");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float2 (*)(const Float2&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2 (*)(const Float2&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const Float2&, const Float2&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(Float2, Float2, bool)>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2 (*)(const Float2&, float)>(&kl::rotate<float>), "rotate");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2 (*)(const Float2&, const Float2&)>(&kl::reflect<float>), "reflect");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const Float3&, const Float3&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const Float3&, const Float3&)>(&kl::angle), "angle");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&, const Float3&, float)>(&kl::rotate<float>), "rotate");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&, Float3)>(&kl::reflect<float>), "reflect");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(const Float3&, const Float3&)>(&kl::cross<float>), "cross");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float4 (*)(const Float4&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float4 (*)(const Float4&)>(&kl::normalize), "normalize");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const Float4&, const Float4&)>(&kl::dot<float>), "dot");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(const Float4&, const Float4&)>(&kl::angle), "angle");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float2x2 (*)(const Float2x2&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2x2 (*)(const Float2x2&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2x2 (*)(const Float2x2&)>(&kl::transpose), "transpose");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float3x3 (*)(const Float3x3&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3x3 (*)(const Float3x3&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3x3 (*)(const Float3x3&)>(&kl::transpose), "transpose");

	INTERP_SCRIPT_MODULE->add(cs::fun<Float4x4 (*)(const Float4x4&)>(&kl::abs), "abs");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float4x4 (*)(const Float4x4&)>(&kl::inverse), "inverse");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float4x4 (*)(const Float4x4&)>(&kl::transpose), "transpose");

	// Random
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_bool), "gen_random_bool");
	INTERP_SCRIPT_MODULE->add(cs::fun<int (*)(int, int)>(&kl::random::gen_int), "gen_random_int");
	INTERP_SCRIPT_MODULE->add(cs::fun<Int2 (*)(int, int)>(&kl::random::gen_int2), "gen_random_int2");
	INTERP_SCRIPT_MODULE->add(cs::fun<float (*)(float, float)>(&kl::random::gen_float), "gen_random_float");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float2 (*)(float, float)>(&kl::random::gen_float2), "gen_random_float2");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float3 (*)(float, float)>(&kl::random::gen_float3), "gen_random_float3");
	INTERP_SCRIPT_MODULE->add(cs::fun<Float4(*)(float, float)>(&kl::random::gen_float4), "gen_random_float4");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_color), "gen_random_color");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_char), "gen_random_char");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::random::gen_string), "gen_random_string");

	// Files
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::read_file), "read_file");
	INTERP_SCRIPT_MODULE->add(cs::fun(&kl::parse_obj_file), "parse_obj_file");

	// Threading
	INTERP_SCRIPT_MODULE->add(cs::fun<void (*)(int, int, const Function<void(int)>&)>(&kl::async_for), "async_for");

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

	return 0;
}();

const int load_names = [&]
{
	using namespace titian;

	cs::ChaiScript temp_engine{};
	temp_engine.add(INTERP_SCRIPT_MODULE);

	CHAI_KEYWORDS = {
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

	for (auto& [name, _] : temp_engine.get_state().engine_state.m_types) {
		CHAI_TYPES.insert(name);
	}

	for (auto& [name, func] : temp_engine.get_state().engine_state.m_function_objects) {
		auto it = std::find_if(name.begin(), name.end(), [](char c) { return !isalnum(c) && c != '_'; });
		if (it != name.end())
			continue;

		if (func->is_attribute_function()) {
			CHAI_MEMBERS.insert(name);
		}
		else {
			CHAI_FUNCTIONS.insert(name);
		}
	}

	for (auto& [name, _] : temp_engine.get_state().engine_state.m_global_objects) {
		CHAI_MEMBERS.insert(name);
	}

	return 0;
}();
