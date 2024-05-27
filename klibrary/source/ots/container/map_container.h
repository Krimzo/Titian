#pragma once

#include "ots/container/data_container.h"


namespace kl::ots {
	class MapContainer : public DataContainer, public MapContainerType<std::string, Object<DataContainer>>
	{
	public:
		MapContainer();
		MapContainer(const std::string& data);

		bool from_string(std::string data, Preprocessor preprocessor = {}) override;
		std::string to_string() const override;
	};
}
