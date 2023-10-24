export module interpreted_script_parts;

export import chaiscript;
export import logger;
export import entity;

export namespace titian {
	const chaiscript::ModulePtr INTERPRETED_SCRIPT_MODULE = chaiscript::ModulePtr(new chaiscript::Module());

	const std::string INTERPRETED_SCRIPT_PREDEFINED_SOURCE =
	R"(
		global print = fun(object) {
			return log(object.to_string());
		}
	)";

	const std::unordered_set<std::string> INTERPRETED_SCRIPT_KEYWORDS
	{
		"attr",
		"auto",
		"break",
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

	std::unordered_map<std::string, std::string> INTERPRETED_SCRIPT_IDENTIFIERS
	{};
}

int load_types = [&]
{
	using namespace titian;

	// Int2
	chaiscript::utility::add_class<kl::Int2>(*INTERPRETED_SCRIPT_MODULE, "Int2",
	{
		chaiscript::constructor<kl::Int2()>(),
		chaiscript::constructor<kl::Int2(int)>(),
		chaiscript::constructor<kl::Int2(int, int)>(),
	},
	{
		{ chaiscript::fun(&kl::Int2::x), "x" },
		{ chaiscript::fun(&kl::Int2::y), "y" },

		{ chaiscript::fun(&kl::Int2::operator kl::Float2), "as_float2" },

		{ chaiscript::fun<int&, kl::Int2, int>(&kl::Int2::operator[]), "[]"},

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Int2"] = "Two component integer vector.";

	// Float2
	chaiscript::utility::add_class<kl::Float2>(*INTERPRETED_SCRIPT_MODULE, "Float2",
	{
		chaiscript::constructor<kl::Float2()>(),
		chaiscript::constructor<kl::Float2(float)>(),
		chaiscript::constructor<kl::Float2(float, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Float2::x), "x" },
		{ chaiscript::fun(&kl::Float2::y), "y" },

		{ chaiscript::fun(&kl::Float2::operator kl::Int2), "as_int2" },
		{ chaiscript::fun(&kl::Float2::operator kl::Complex), "as_complex" },

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float2"] = "Two component float vector.";

	// Float3
	chaiscript::utility::add_class<kl::Float3>(*INTERPRETED_SCRIPT_MODULE, "Float3",
	{
		chaiscript::constructor<kl::Float3()>(),
		chaiscript::constructor<kl::Float3(float)>(),
		chaiscript::constructor<kl::Float3(float, float, float)>(),

		chaiscript::constructor<kl::Float3(kl::Float2, float)>(),
		chaiscript::constructor<kl::Float3(float, kl::Float2)>(),
	},
	{
		{ chaiscript::fun(&kl::Float3::x), "x" },
		{ chaiscript::fun(&kl::Float3::y), "y" },
		{ chaiscript::fun(&kl::Float3::z), "z" },

		{ chaiscript::fun(&kl::Float3::operator kl::Color), "as_color" },
		{ chaiscript::fun(&kl::Float3::operator kl::Quaternion), "as_quaternion" },
		{ chaiscript::fun(&kl::Float3::xy), "xy"},

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float3"] = "Three component float vector.";

	// Float4
	chaiscript::utility::add_class<kl::Float4>(*INTERPRETED_SCRIPT_MODULE, "Float4",
	{
		chaiscript::constructor<kl::Float4()>(),
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

		{ chaiscript::fun(&kl::Float4::operator kl::Color), "as_color" },
		{ chaiscript::fun(&kl::Float4::operator kl::Quaternion), "as_quaternion" },
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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float4"] = "Four component float vector.";

	// Float2x2
	chaiscript::utility::add_class<kl::Float2x2>(*INTERPRETED_SCRIPT_MODULE, "Float2x2",
	{
		chaiscript::constructor<kl::Float2x2()>(),
	},
	{
		{ chaiscript::fun<float&, kl::Float2x2, int>(&kl::Float2x2::operator[]), "[]"},
		//{ chaiscript::fun<float&, kl::Float2x2, int, int>(&kl::Float2x2::operator()), "()"},

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float2x2"] = "Two float wide square matrix.";

	// Float3x3
	chaiscript::utility::add_class<kl::Float3x3>(*INTERPRETED_SCRIPT_MODULE, "Float3x3",
	{
		chaiscript::constructor<kl::Float3x3()>(),
	},
	{
		{ chaiscript::fun<float&, kl::Float3x3, int>(&kl::Float3x3::operator[]), "[]"},
		//{ chaiscript::fun<float&, kl::Float3x3, int, int>(&kl::Float3x3::operator()), "()"},

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float3x3"] = "Three float wide square matrix.";

	// Float4x4
	chaiscript::utility::add_class<kl::Float4x4>(*INTERPRETED_SCRIPT_MODULE, "Float4x4",
	{
		chaiscript::constructor<kl::Float4x4()>(),
	},
	{
		{ chaiscript::fun<float&, kl::Float4x4, int>(&kl::Float4x4::operator[]), "[]"},
		//{ chaiscript::fun<float&, kl::Float4x4, int, int>(&kl::Float4x4::operator()), "()"},

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Float4x4"] = "Four float wide square matrix.";

	// Complex
	chaiscript::utility::add_class<kl::Complex>(*INTERPRETED_SCRIPT_MODULE, "Complex",
	{
		chaiscript::constructor<kl::Complex()>(),
		chaiscript::constructor<kl::Complex(float, float)>(),

		chaiscript::constructor<kl::Complex(float)>(),
	},
	{
		{ chaiscript::fun(&kl::Complex::r), "r" },
		{ chaiscript::fun(&kl::Complex::i), "i" },

		{ chaiscript::fun(&kl::Complex::operator kl::Float2), "as_float2" },

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Complex"] = "Two component imaginary number.";

	// Quaternion
	chaiscript::utility::add_class<kl::Quaternion>(*INTERPRETED_SCRIPT_MODULE, "Quaternion",
	{
		chaiscript::constructor<kl::Quaternion()>(),
		chaiscript::constructor<kl::Quaternion(float, float, float)>(),
		chaiscript::constructor<kl::Quaternion(float, float, float, float)>(),

		chaiscript::constructor<kl::Quaternion(kl::Float3, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Quaternion::w), "w" },
		{ chaiscript::fun(&kl::Quaternion::x), "x" },
		{ chaiscript::fun(&kl::Quaternion::y), "y" },
		{ chaiscript::fun(&kl::Quaternion::z), "z" },

		{ chaiscript::fun(&kl::Quaternion::operator kl::Float3), "as_float3" },
		{ chaiscript::fun(&kl::Quaternion::operator kl::Float4), "as_float4" },

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Quaternion"] = "Four component imaginary number.";

	// Vertex
	chaiscript::utility::add_class<kl::Vertex>(*INTERPRETED_SCRIPT_MODULE, "Vertex",
	{
		chaiscript::constructor<kl::Vertex()>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float2)>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float3)>(),
		chaiscript::constructor<kl::Vertex(kl::Float3, kl::Float2, kl::Float3)>(),
	},
	{
		{ chaiscript::fun(&kl::Vertex::world), "world" },
		{ chaiscript::fun(&kl::Vertex::normal), "normal" },
		{ chaiscript::fun(&kl::Vertex::texture), "texture" },

		{ chaiscript::fun([](const kl::Vertex& object) { return kl::format(object); }), "to_string" },
	});
	INTERPRETED_SCRIPT_IDENTIFIERS["Vertex"] = "A 3D point of a mesh.";

	// Triangle
	chaiscript::utility::add_class<kl::Triangle>(*INTERPRETED_SCRIPT_MODULE, "Triangle",
	{
		chaiscript::constructor<kl::Triangle()>(),
		chaiscript::constructor<kl::Triangle(kl::Vertex, kl::Vertex, kl::Vertex)>(),
	},
	{
		{ chaiscript::fun(&kl::Triangle::a), "a" },
		{ chaiscript::fun(&kl::Triangle::b), "b" },
		{ chaiscript::fun(&kl::Triangle::c), "c" },

		{ chaiscript::fun([](const kl::Triangle& object) { return kl::format(object); }), "to_string" },
	});
	INTERPRETED_SCRIPT_IDENTIFIERS["Triangle"] = "A 3D triangle in space.";

	// Plane
	chaiscript::utility::add_class<kl::Plane>(*INTERPRETED_SCRIPT_MODULE, "Plane",
	{
		chaiscript::constructor<kl::Plane()>(),
		chaiscript::constructor<kl::Plane(kl::Float3, kl::Float3)>(),
	},
	{
		{ chaiscript::fun(&kl::Plane::origin), "origin" },

		{ chaiscript::fun(&kl::Plane::set_normal), "set_normal" },
		{ chaiscript::fun(&kl::Plane::normal), "normal" },

		{ chaiscript::fun(&kl::Plane::in_front), "in_front" },

		{ chaiscript::fun([](const kl::Plane& object) { return kl::format(object); }), "to_string" },
	});
	INTERPRETED_SCRIPT_IDENTIFIERS["Plane"] = "A 3D plane in space.";

	// Sphere
	chaiscript::utility::add_class<kl::Sphere>(*INTERPRETED_SCRIPT_MODULE, "Sphere",
	{
		chaiscript::constructor<kl::Sphere()>(),
		chaiscript::constructor<kl::Sphere(kl::Float3, float)>(),
	},
	{
		{ chaiscript::fun(&kl::Sphere::origin), "origin" },
		{ chaiscript::fun(&kl::Sphere::radius), "radius" },

		{ chaiscript::fun([](const kl::Sphere& object) { return kl::format(object); }), "to_string" },
	});
	INTERPRETED_SCRIPT_IDENTIFIERS["Sphere"] = "A 3D sphere in space.";

	// Ray
	chaiscript::utility::add_class<kl::Ray>(*INTERPRETED_SCRIPT_MODULE, "Ray",
	{
		chaiscript::constructor<kl::Ray()>(),
		chaiscript::constructor<kl::Ray(kl::Float3, kl::Float3)>(),
		chaiscript::constructor<kl::Ray(kl::Float3, kl::Float4x4, kl::Float2)>(),
	},
	{
		{ chaiscript::fun(&kl::Ray::origin), "origin" },

		{ chaiscript::fun(&kl::Ray::set_direction), "set_direction" },
		{ chaiscript::fun(&kl::Ray::direction), "direction" },

		{ chaiscript::fun(&kl::Ray::intersect_plane), "intersect_plane" },
		{ chaiscript::fun(&kl::Ray::intersect_triangle), "intersect_triangle" },

		{ chaiscript::fun(&kl::Ray::can_intersect_sphere), "can_intersect_sphere" },
		{ chaiscript::fun(&kl::Ray::intersect_sphere), "intersect_sphere" },

		{ chaiscript::fun([](const kl::Ray& object) { return kl::format(object); }), "to_string" },
	});
	INTERPRETED_SCRIPT_IDENTIFIERS["Ray"] = "A 3D ray in space.";

	// Color
	chaiscript::utility::add_class<kl::Color>(*INTERPRETED_SCRIPT_MODULE, "Color",
	{
		chaiscript::constructor<kl::Color()>(),
		chaiscript::constructor<kl::Color(byte, byte, byte)>(),
		chaiscript::constructor<kl::Color(byte, byte, byte, byte)>(),
	},
	{
		{ chaiscript::fun(&kl::Color::r), "r" },
		{ chaiscript::fun(&kl::Color::g), "g" },
		{ chaiscript::fun(&kl::Color::b), "b" },
		{ chaiscript::fun(&kl::Color::a), "a" },

		{ chaiscript::fun(&kl::Color::operator kl::Float3), "as_float3" },
		{ chaiscript::fun(&kl::Color::operator kl::Float4), "as_float4" },

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
	INTERPRETED_SCRIPT_IDENTIFIERS["Color"] = "Four component byte[0, 255] color.";

	// Entity
	chaiscript::utility::add_class<Entity>(*INTERPRETED_SCRIPT_MODULE, "Entity",
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
	INTERPRETED_SCRIPT_IDENTIFIERS["Entity"] = "Base entity that's a part of a scene.";

	return 0;
}();

int load_constants = [&]
{
	using namespace titian;

	// Numbers
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::PI), "PI");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_RADIANS), "TO_RADIANS");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_DEGREES), "TO_DEGREES");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_FLOAT_COLOR), "TO_FLOAT_COLOR");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::TO_BYTE_COLOR), "TO_BYTE_COLOR");
	INTERPRETED_SCRIPT_IDENTIFIERS["PI"] = "Constant of a number PI (3.1415...).";
	INTERPRETED_SCRIPT_IDENTIFIERS["TO_RADIANS"] = "Constant for converting degrees to radians (PI / 180).";
	INTERPRETED_SCRIPT_IDENTIFIERS["TO_DEGREES"] = "Constant for converting degrees to radians (180 / PI).";
	INTERPRETED_SCRIPT_IDENTIFIERS["TO_FLOAT_COLOR"] = "Constant for converting byte color to float color (1 / 255).";
	INTERPRETED_SCRIPT_IDENTIFIERS["TO_BYTE_COLOR"] = "Constant for converting float color to byte color (255 / 1).";

	// Colors
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CONSOLE), "CONSOLE");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::BLACK), "BLACK");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::WHITE), "WHITE");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::GRAY), "GRAY");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::RED), "RED");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::GREEN), "GREEN");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::BLUE), "BLUE");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CYAN), "CYAN");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::PURPLE), "PURPLE");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::YELLOW), "YELLOW");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::ORANGE), "ORANGE");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::MAGENTA), "MAGENTA");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::CRIMSON), "CRIMSON");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::WHEAT), "WHEAT");
	INTERPRETED_SCRIPT_MODULE->add_global_const(chaiscript::const_var(&kl::colors::SKY), "SKY");
	INTERPRETED_SCRIPT_IDENTIFIERS["CONSOLE"] = "Constant color (204, 204, 204).";
	INTERPRETED_SCRIPT_IDENTIFIERS["BLACK"] = "Constant color (0, 0, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["WHITE"] = "Constant color (255, 255, 255).";
	INTERPRETED_SCRIPT_IDENTIFIERS["GRAY"] = "Constant color (50, 50, 50).";
	INTERPRETED_SCRIPT_IDENTIFIERS["RED"] = "Constant color (255, 0, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["GREEN"] = "Constant color (0, 255, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["BLUE"] = "Constant color (0, 0, 255).";
	INTERPRETED_SCRIPT_IDENTIFIERS["CYAN"] = "Constant color (30, 180, 170).";
	INTERPRETED_SCRIPT_IDENTIFIERS["PURPLE"] = "Constant color (220, 0, 220).";
	INTERPRETED_SCRIPT_IDENTIFIERS["YELLOW"] = "Constant color (220, 220, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["ORANGE"] = "Constant color (255, 140, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["MAGENTA"] = "Constant color (155, 0, 155).";
	INTERPRETED_SCRIPT_IDENTIFIERS["CRIMSON"] = "Constant color (100, 0, 0).";
	INTERPRETED_SCRIPT_IDENTIFIERS["WHEAT"] = "Constant color (245, 220, 180).";
	INTERPRETED_SCRIPT_IDENTIFIERS["SKY"] = "Constant color (190, 245, 255).";

	return 0;
}();

int load_functions = [&]
{
	using namespace titian;

	// Calls
	INTERPRETED_SCRIPT_IDENTIFIERS["on_start"] = "Called once at the start of the game.";
	INTERPRETED_SCRIPT_IDENTIFIERS["on_update"] = "Called every frame of the game.";

	// Logging
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&Logger::log), "log");
	INTERPRETED_SCRIPT_IDENTIFIERS["log"] = "Outputs the given string to the log window.";
	INTERPRETED_SCRIPT_IDENTIFIERS["print"] = "Logs the given object if that object contains method to_string.";

	// Float3x3
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::translation), "Float3x3_translation");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::rotation), "Float3x3_rotation");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float3x3::scaling), "Float3x3_scaling");

	// Float4x4
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::translation), "Float4x4_translation");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::rotation), "Float4x4_rotation");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::scaling), "Float4x4_scaling");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::look_at), "Float4x4_look_at");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::perspective), "Float4x4_perspective");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::Float4x4::orthographic), "Float4x4_orthographic");

	// Math
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::sin_deg), "sin_deg");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::cos_deg), "cos_deg");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::tan_deg), "tan_deg");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::asin_deg), "asin_deg");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::acos_deg), "acos_deg");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::atan_deg), "atan_deg");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::calc_ndc), "calc_ndc");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::calc_ndc_ar), "calc_ndc_ar");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::line_x), "line_x");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::line_y), "line_y");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::wrap), "wrap");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::unwrap), "unwrap");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::clamp), "clamp");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::to_quaternion), "to_quaternion");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun(&kl::to_euler), "to_euler");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::normalize), "normalize");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Complex, const kl::Complex&>(&kl::inverse), "inverse");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::normalize), "normalize");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Quaternion, const kl::Quaternion&>(&kl::inverse), "inverse");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Int2, const kl::Int2&>(&kl::abs), "abs");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::normalize), "normalize");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float2&>(&kl::dot), "dot");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, kl::Float2, kl::Float2, bool>(&kl::angle), "angle");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::rotate), "rotate");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2, const kl::Float2&>(&kl::reflect), "reflect");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::normalize), "normalize");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float3&>(&kl::dot), "dot");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float3&, const kl::Float3&>(&kl::angle), "angle");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::rotate), "rotate");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::reflect), "reflect");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3, const kl::Float3&>(&kl::cross), "cross");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4, const kl::Float4&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4, const kl::Float4&>(&kl::normalize), "normalize");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float4&>(&kl::dot), "dot");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<float, const kl::Float4&, const kl::Float4&>(&kl::angle), "angle");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::inverse), "inverse");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float2x2, const kl::Float2x2&>(&kl::transpose), "transpose");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::inverse), "inverse");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float3x3, const kl::Float3x3&>(&kl::transpose), "transpose");

	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::abs), "abs");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::inverse), "inverse");
	INTERPRETED_SCRIPT_MODULE->add(chaiscript::fun<kl::Float4x4, const kl::Float4x4&>(&kl::transpose), "transpose");

	// Scene


	return 0;
}();
