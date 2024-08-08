#pragma once

#include "ots/container/data_container.h"


namespace kl::ots {
	class ArrayContainer : public DataContainer, public ArrayContainerType<Ref<DataContainer>>
	{
	public:
		ArrayContainer();
		ArrayContainer(const std::initializer_list<Ref<DataContainer>>& items);
		ArrayContainer(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string(int depth = 0) const override;
	};
}
