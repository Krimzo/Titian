#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Object : public Container, public ObjectStorageType<std::string, Ref<Container>>
	{
	public:
		Object();
		Object(const std::initializer_list<std::pair<std::string, Ref<Container>>>& items);
		Object(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string(int depth = 0) const override;
	};
}
