export module serialization;

export import serializable;

export namespace titian {
    inline constexpr uint32_t SERIAL_VERSION = 0x21092023;
    inline const std::string SERIAL_VERSION_FORMAT = kl::format("0x", std::hex, SERIAL_VERSION);
}

export namespace titian {
    bool serialize(const Serializable* serializable, const std::string_view& path)
    {
        kl::File file = { path, true};
        if (!file) {
            return false;
        }
        serializable->serialize(&file);
        file.close();
        return true;
    }

    bool deserialize(Serializable* serializable, const std::string_view& path)
    {
        kl::File file = { path, false};
        if (!file) {
            return false;
        }
        serializable->deserialize(&file);
        file.close();
        return true;
    }
}
