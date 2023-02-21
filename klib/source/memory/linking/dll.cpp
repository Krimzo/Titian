#include "memory/linking/dll.h"


kl::dll::dll()
{}

kl::dll::dll(const std::string& path)
{
    load(path);
}

kl::dll::~dll()
{
    unload();
}

void kl::dll::load(const std::string& path)
{
    unload();
    this->path = path;
    module = LoadLibraryA(path.c_str());
}

void kl::dll::reload()
{
    unload();
    module = LoadLibraryA(path.c_str());
}

void kl::dll::unload()
{
    if (module) FreeLibrary(module);
    module = nullptr;
}

bool kl::dll::is_valid()
{
    return (bool) module;
}
