#pragma once

#include "ots/container/data_container.h"


namespace kl::ots {
	class MapContainer : public DataContainer, public MapContainerType<std::string, Ref<DataContainer>>
	{
	public:
		MapContainer();
		MapContainer(const std::initializer_list<std::pair<std::string, Ref<DataContainer>>>& items);
		MapContainer(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string(int depth = 0) const override;
	};
}
