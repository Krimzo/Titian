#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Object : public Container, public ObjectStorageType<std::string, Ref<Container>>
	{
	public:
		Object();
		Object(const std::initializer_list<std::pair<std::string, Ref<Container>>>& items);
		Object(const std::string& data);

		bool compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last) override;
		std::string decompile(int depth = 0) const override;
	};
}
