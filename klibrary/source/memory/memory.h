#pragma once

#include "memory/safety/ref.h"
#include "memory/safety/com_ref.h"
#include "memory/files/file.h"
#include "memory/files/dll.h"


namespace kl {
    template<typename T>
    T* allocate(uint64_t count)
    {
        return (T*) ::calloc(count, sizeof(T));
    }

    template<typename T>
    void deallocate(T*& ptr)
    {
        if (ptr) {
            ::free((void*) ptr);
            ptr = nullptr;
        }
    }

    template<typename T>
    void copy(void* dst, const void* src, uint64_t count)
    {
        ::memcpy(dst, src, count * sizeof(T));
    }
}
