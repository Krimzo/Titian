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

kl::dll::operator bool() const
{
    return (bool) module_;
}

void kl::dll::load(const std::string& path)
{
    unload();
    path_ = path;
    module_ = LoadLibraryA(path.c_str());
}

void kl::dll::reload()
{
    unload();
    module_ = LoadLibraryA(path_.c_str());
}

void kl::dll::unload()
{
    if (module_) {
        FreeLibrary(module_);
        module_ = nullptr;
    }
}
