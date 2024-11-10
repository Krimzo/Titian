#pragma once

#include "script/script.h"
#include "scene/scene.h"
#include "mmodule.h"


namespace titian
{
template<typename Return, typename... Args>
using NativeFunc = Return( __stdcall* )(Args...);
}

namespace titian
{
struct NativeScript : Script
{
    String data;

    NativeScript() = default;

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    bool is_valid() const override;
    void reload() override;

    void call_start( Scene& scene ) override;
    void call_update( Scene& scene ) override;
    void call_collision( Scene& scene, Entity& attacker, Entity& target ) override;
    void call_ui( Scene& scene ) override;

private:
    void* m_memory_module = nullptr;
    NativeFunc<void, Scene&> m_start_function = nullptr;
    NativeFunc<void, Scene&> m_update_function = nullptr;
    NativeFunc<void, Scene&, Entity&, Entity&> m_collision_function = nullptr;
    NativeFunc<void, Scene&> m_ui_function = nullptr;

    void unload();

    template<typename Return, typename... Args>
    NativeFunc<Return, Args...> read_function( StringRef const& function_name )
    {
        if ( !m_memory_module )
            return nullptr;

        auto function_address = MemoryGetProcAddress( m_memory_module, function_name.data() );
        return reinterpret_cast<NativeFunc<Return, Args...>>(function_address);
    }
};
}
