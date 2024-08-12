#pragma once

#include "json/container/container.h"


namespace kl::json {
	class Array : public ArrayStorageType<ContainerWrap>, public Container
	{
	public:
		Array();
		Array(const std::string& data);

		bool compile(std::vector<Token>::const_iterator first, std::vector<Token>::const_iterator last) override;
		std::string decompile(int depth = 0) const override;
	};
}
