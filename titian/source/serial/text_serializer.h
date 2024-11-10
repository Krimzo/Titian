#pragma once

#include "serial/serial.h"


namespace titian
{
struct TextSerializer : Serializer
{
    TextSerializer( StringRef const& path, bool write );
    ~TextSerializer() override;

    operator bool() const override;

    void push_object( StringRef const& name ) override;
    void pop_object() override;

    void load_object( StringRef const& name ) const override;
    void unload_object() const override;

    void write_bool( StringRef const& name, bool value ) override;
    void read_bool( StringRef const& name, bool& value ) const override;

    void write_int( StringRef const& name, int32_t value ) override;
    void read_int( StringRef const& name, int32_t& value ) const override;

    void write_float( StringRef const& name, float value ) override;
    void read_float( StringRef const& name, float& value ) const override;

    void write_byte_array( StringRef const& name, void const* data, int32_t count ) override;
    void read_byte_array( StringRef const& name, void* data, int32_t count ) const override;

    void write_int_array( StringRef const& name, int32_t const* data, int32_t count ) override;
    void read_int_array( StringRef const& name, int32_t* data, int32_t count ) const override;

    void write_float_array( StringRef const& name, float const* data, int32_t count ) override;
    void read_float_array( StringRef const& name, float* data, int32_t count ) const override;

    void write_string( StringRef const& name, StringRef const& value ) override;
    void read_string( StringRef const& name, String& value ) const override;

private:
    String m_path;
    bool m_writing;

    mutable Vector<Ref<js::Object>> m_stack = { new js::Object() };
    bool m_is_valid = false;

    auto& current() const
    {
        return *m_stack.back();
    }
};
}
