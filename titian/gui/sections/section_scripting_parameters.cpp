#include "titian.h"

template <typename V> static std::type_info const& variant_type(V const& variant)
{
    return std::visit([](auto&& value) -> std::type_info const& { return typeid(value); }, variant);
}

template <typename V> static void* variant_ptr(V& variant)
{
    return std::visit([](auto&& value) -> void* { return &value; }, variant);
}

titian::GUISectionScriptingParameters::GUISectionScriptingParameters() : GUISection("GUISectionScriptingParameters")
{
}

void titian::GUISectionScriptingParameters::render_gui()
{
    TimeBomb _ = bench_time_bomb();

    GUILayer const& gui_layer = GUILayer::get();
    Scene const& scene = GameLayer::get().scene();

    im::PushFont(gui_layer.roboto_font_small);

    if (im::Begin("Scripting Parameters"))
    {
        int script_counter = 1;
        for (auto& [name, script] : scene.scripts)
        {
            if (script_counter > 1)
                im::Separator();
            im::Text(kl::format("Script ", name).data());

            if (InterpScript* interp_script = dynamic_cast<InterpScript*>(&script))
            {
                for (auto& [name, value] : interp_script->get_parameters())
                    display_interp_parameter_editor(script_counter, name, value);
            }
            else if (NodeScript* node_script = dynamic_cast<NodeScript*>(&script))
            {
                for (auto& [name, info] : node_script->var_storage)
                    if (info.global)
                        display_node_parameter_editor(script_counter, name, variant_type(info.value),
                                                      variant_ptr(info.value));
            }
            script_counter += 1;
        }
    }
    im::End();

    im::PopFont();
}

void titian::GUISectionScriptingParameters::display_interp_parameter_editor(int script_id, StringRef const& name,
                                                                            InterpScript::Parameter& parameter)
{
    String identifier = kl::format(name, "##", script_id);
    if (parameter.is<bool>())
    {
        bool temp = parameter.get<bool>();
        if (im::Checkbox(identifier.data(), &temp))
            parameter.set<bool>(temp);
    }
    else if (parameter.is<int>())
    {
        int temp = parameter.get<int>();
        if (im::DragInt(identifier.data(), &temp))
            parameter.set<int>(temp);
    }
    else if (parameter.is<int2>())
    {
        int2 temp = parameter.get<int2>();
        if (im::DragInt2(identifier.data(), &temp.x))
            parameter.set<int2>(temp);
    }
    else if (parameter.is<float>())
    {
        float temp = parameter.get<float>();
        if (im::DragFloat(identifier.data(), &temp))
            parameter.set<float>(temp);
    }
    else if (parameter.is<float2>())
    {
        float2 temp = parameter.get<float2>();
        if (im::DragFloat2(identifier.data(), &temp.x))
            parameter.set<float2>(temp);
    }
    else if (parameter.is<float3>())
    {
        float3 temp = parameter.get<float3>();
        if (im::DragFloat3(identifier.data(), &temp.x))
            parameter.set<float3>(temp);
    }
    else if (parameter.is<float4>())
    {
        float4 temp = parameter.get<float4>();
        if (im::DragFloat4(identifier.data(), &temp.x))
            parameter.set<float4>(temp);
    }
    else if (parameter.is<cmplx>())
    {
        cmplx temp = parameter.get<cmplx>();
        if (im::DragFloat2(identifier.data(), &temp.r))
            parameter.set<float2>(temp);
    }
    else if (parameter.is<quat>())
    {
        float4 temp = parameter.get<quat>();
        if (im::DragFloat4(identifier.data(), &temp.x))
            parameter.set<quat>(temp);
    }
    else if (parameter.is<RGB>())
    {
        float4 temp = parameter.get<RGB>();
        if (im::ColorEdit4(identifier.data(), &temp.x))
            parameter.set<RGB>(temp);
    }
    else if (parameter.is<String>())
    {
        String temp = parameter.get<String>();
        if (im::InputText(identifier.data(), &temp))
            parameter.set<String>(temp);
    }
    else
    {
        im::Text(kl::format("[unknown] ", identifier).data());
    }
}

void titian::GUISectionScriptingParameters::display_node_parameter_editor(int script_id, StringRef const& name,
                                                                          std::type_info const& type, void* ptr)
{
    if (!ptr)
        return;

    String identifier = kl::format(name, "##", script_id);
    if (type == typeid(void*))
    {
        String temp = kl::format("0x", *(void**)ptr);
        im::InputText(identifier.data(), &temp, ImGuiInputTextFlags_ReadOnly);
    }
    else if (type == typeid(bool))
    {
        im::Checkbox(identifier.data(), (bool*)ptr);
    }
    else if (type == typeid(int))
    {
        im::DragInt(identifier.data(), (int*)ptr);
    }
    else if (type == typeid(int2))
    {
        im::DragInt2(identifier.data(), (int*)ptr);
    }
    else if (type == typeid(float))
    {
        im::DragFloat(identifier.data(), (float*)ptr);
    }
    else if (type == typeid(float2))
    {
        im::DragFloat2(identifier.data(), (float*)ptr);
    }
    else if (type == typeid(float3))
    {
        im::DragFloat3(identifier.data(), (float*)ptr);
    }
    else if (type == typeid(float4))
    {
        im::DragFloat4(identifier.data(), (float*)ptr);
    }
    else if (type == typeid(cmplx))
    {
        im::DragFloat2(identifier.data(), (float*)ptr);
    }
    else if (type == typeid(quat))
    {
        float4 temp = *(quat*)ptr;
        if (im::DragFloat4(identifier.data(), &temp.x))
            *(quat*)ptr = temp;
    }
    else if (type == typeid(RGB))
    {
        float4 temp = *(RGB*)ptr;
        if (im::ColorEdit4(identifier.data(), &temp.x))
            *(RGB*)ptr = temp;
    }
    else if (type == typeid(String))
    {
        im::InputText(identifier.data(), (String*)ptr);
    }
    else
    {
        im::Text(kl::format("[unknown] ", name).data());
    }
}
