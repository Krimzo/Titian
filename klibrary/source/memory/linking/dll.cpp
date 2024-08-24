#include "klibrary.h"


kl::DLL::DLL()
{}

kl::DLL::DLL(const std::string_view& path)
{
    load(path);
}

kl::DLL::~DLL()
{
    unload();
}

kl::DLL::operator bool() const
{
    return (bool) m_module;
}

void kl::DLL::load(const std::string_view& path)
{
    unload();
    m_path = path;
    m_module = LoadLibraryA(path.data());
}

void kl::DLL::reload()
{
    unload();
    m_module = LoadLibraryA(m_path.data());
}

void kl::DLL::unload()
{
    if (m_module) {
        FreeLibrary(m_module);
        m_module = nullptr;
    }
}
