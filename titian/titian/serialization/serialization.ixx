export module serialization;

export import serializable;

export namespace titian {
    inline constexpr uint32_t SERIAL_VERSION = 0x21092023;
    inline const std::string SERIAL_VERSION_FORMAT = kl::format("0x", std::hex, SERIAL_VERSION);
}

export namespace titian {
    bool serialize(const std::string_view& path, const Serializable* serializable, const void* helper_data)
    {
        Serializer serializer = { path, true };
        if (!serializer) {
            return false;
        }
        serializable->serialize(&serializer, helper_data);
        return true;
    }

    bool deserialize(const std::string_view& path, Serializable* serializable, const void* helper_data)
    {
        Serializer serializer = { path, false };
        if (!serializer) {
            return false;
        }
        serializable->deserialize(&serializer, helper_data);
        return true;
    }
}
