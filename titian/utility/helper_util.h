#pragma once

#include "globals.h"

namespace titian
{
float3& px_cast(px::PxVec3& vec);
float3 const& px_cast(px::PxVec3 const& vec);

float4& px_cast(px::PxQuat& quat);
float4 const& px_cast(px::PxQuat const& quat);

px::PxVec3& px_cast(float3& vec);
px::PxVec3 const& px_cast(float3 const& vec);

px::PxQuat& px_cast(float4& quat);
px::PxQuat const& px_cast(float4 const& quat);

inline Func<void(StringRef const&)> TITIAN_VERIFY_LOGGER = [](StringRef const& message) -> void {
    kl::print("Failed to verify: ", message);
};

template <typename... Args> constexpr bool ti_verify(bool state, Args&&... args)
{
    if (!state)
        TITIAN_VERIFY_LOGGER(kl::format(args...));
    return state;
}

template <typename... Args> constexpr void ti_assert(bool state, Args&&... args)
{
    if (!state)
    {
        const String message = kl::format(args...);
        MessageBoxA(nullptr, message.data(), "Titian assert failed.", MB_ICONERROR | MB_OK);
        std::abort();
    }
}
} // namespace titian
