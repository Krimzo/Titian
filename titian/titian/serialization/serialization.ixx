export module serialization;

export import serializable;

export namespace titian {
    inline constexpr uint32_t SERIAL_VERSION = 0x21092023;
    inline const std::string SERIAL_VERSION_FORMAT = kl::format("0x", std::hex, SERIAL_VERSION);
}

export namespace titian {
    bool serialize(const Serializable* serializable, const std::string_view& path)
    {
        Serializer serializer = { path, true };
        if (!serializer) {
            return false;
        }
        serializable->serialize(&serializer);
        return true;
    }

    bool deserialize(Serializable* serializable, const std::string_view& path)
    {
        Serializer serializer = { path, false };
        if (!serializer) {
            return false;
        }
        serializable->deserialize(&serializer);
        return true;
    }
}
