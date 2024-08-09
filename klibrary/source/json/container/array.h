#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Array : public Container, public ArrayStorageType<Ref<Container>>
	{
	public:
		Array();
		Array(const std::initializer_list<Ref<Container>>& items);
		Array(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string(int depth = 0) const override;
	};
}
