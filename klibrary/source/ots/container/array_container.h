#pragma once

#include "ots/container/data_container.h"


namespace kl::ots {
	class ArrayContainer : public DataContainer, public ArrayContainerType<Object<DataContainer>>
	{
	public:
		ArrayContainer();
		ArrayContainer(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string() const override;
	};
}
