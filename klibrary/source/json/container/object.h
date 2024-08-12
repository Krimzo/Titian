#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Object : public ObjectStorageType<std::string, ContainerWrap>, public Container
	{
	public:
		Object();
		Object(const std::string& data);

		bool compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last) override;
		std::string decompile(int depth = 0) const override;
	};
}
