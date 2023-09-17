#include "klib.h"


kl::DLL::DLL()
{}

kl::DLL::DLL(const std::string& path)
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

void kl::DLL::load(const std::string& path)
{
    unload();
    m_path = path;
    m_module = LoadLibraryA(path.c_str());
}

void kl::DLL::reload()
{
    unload();
    m_module = LoadLibraryA(m_path.c_str());
}

void kl::DLL::unload()
{
    if (m_module) {
        FreeLibrary(m_module);
        m_module = nullptr;
    }
}
