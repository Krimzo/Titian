#pragma once

#include "jdk.h"


class class_info
{
	jclass m_class = nullptr;
	std::unordered_map<uint32_t, jmethod> m_constructors = {};
	std::unordered_map<std::string, jmethod> m_methods = {};
	std::unordered_map<std::string, jfield> m_fields = {};

public:
	class_info();

	// Load
	void load_class(const std::string& name);
	void load_constructor(uint32_t index, const std::vector<std::string>& parameters);
	void load_method(const std::string& name, const std::string& return_type, const std::vector<std::string>& parameters);
	void load_field(const std::string& name, const std::string& signature);

	// Get
	jclass get_class() const;
	jmethod get_constructor(uint32_t index) const;
	jmethod get_method(const std::string& name) const;
	jfield get_field(const std::string& name) const;

	// Make
	jobject new_instance(uint32_t constructor_index, ...) const;
};
