export module serialization;

export import klib;

export namespace serialization {
    inline constexpr uint32_t VERSION = 0x27072023;
    inline const std::string VERSION_NAME = kl::format(std::hex, "0x", VERSION);
}
