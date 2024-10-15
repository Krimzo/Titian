#include "titian.h"


template<typename V>
static const std::type_info& variant_type(const V& variant)
{
	return std::visit([](auto&& value) -> const std::type_info& { return typeid(value); }, variant);
}

template<typename V>
static void* variant_ptr(V& variant)
{
	return std::visit([](auto&& value) -> void* { return &value; }, variant);
}

titian::GUISectionScriptingParameters::GUISectionScriptingParameters()
	: GUISection("GUISectionScriptingParameters")
{}

void titian::GUISectionScriptingParameters::render_gui()
{
	const TimeBomb _ = bench_time_bomb();

	GUILayer& gui_layer = Layers::get<GUILayer>();
	Scene& scene = Layers::get<GameLayer>().scene();

	im::PushFont(gui_layer.roboto_font_small);

	if (im::Begin("Scripting Parameters")) {
		int script_counter = 1;

		for (auto& [name, script] : scene.scripts) {
			if (script_counter > 1) {
				im::Separator();
			}
			im::Text(kl::format("Script ", name).data());

			if (InterpScript* interp_script = dynamic_cast<InterpScript*>(&script)) {
				for (auto& [name, value] : interp_script->get_parameters()) {
					display_interp_parameter_editor(script_counter, name, value);
				}
			}
			else if (NodeScript* node_script = dynamic_cast<NodeScript*>(&script)) {
				for (auto& [name, info] : node_script->var_storage) {
					if (info.global) {
						display_node_parameter_editor(script_counter, name, variant_type(info.value), variant_ptr(info.value));
					}
				}
			}
			script_counter += 1;
		}
	}
	im::End();

	im::PopFont();
}

void titian::GUISectionScriptingParameters::display_interp_parameter_editor(const int script_id, const StringView& name, InterpScript::Parameter& parameter)
{
	const String identifier = kl::format(name, "##", script_id);
	if (parameter.is<bool>()) {
		bool temp = parameter.get<bool>();
		if (im::Checkbox(identifier.data(), &temp)) {
			parameter.set<bool>(temp);
		}
	}
	else if (parameter.is<int>()) {
		int temp = parameter.get<int>();
		if (im::DragInt(identifier.data(), &temp)) {
			parameter.set<int>(temp);
		}
	}
	else if (parameter.is<Int2>()) {
		Int2 temp = parameter.get<Int2>();
		if (im::DragInt2(identifier.data(), &temp.x)) {
			parameter.set<Int2>(temp);
		}
	}
	else if (parameter.is<float>()) {
		float temp = parameter.get<float>();
		if (im::DragFloat(identifier.data(), &temp)) {
			parameter.set<float>(temp);
		}
	}
	else if (parameter.is<Float2>()) {
		Float2 temp = parameter.get<Float2>();
		if (im::DragFloat2(identifier.data(), &temp.x)) {
			parameter.set<Float2>(temp);
		}
	}
	else if (parameter.is<Float3>()) {
		Float3 temp = parameter.get<Float3>();
		if (im::DragFloat3(identifier.data(), &temp.x)) {
			parameter.set<Float3>(temp);
		}
	}
	else if (parameter.is<Float4>()) {
		Float4 temp = parameter.get<Float4>();
		if (im::DragFloat4(identifier.data(), &temp.x)) {
			parameter.set<Float4>(temp);
		}
	}
	else if (parameter.is<Complex>()) {
		Complex temp = parameter.get<Complex>();
		if (im::DragFloat2(identifier.data(), &temp.r)) {
			parameter.set<Float2>(temp);
		}
	}
	else if (parameter.is<Quaternion>()) {
		Float4 temp = parameter.get<Quaternion>();
		if (im::DragFloat4(identifier.data(), &temp.x)) {
			parameter.set<Quaternion>(temp);
		}
	}
	else if (parameter.is<RGB>()) {
		Float4 temp = parameter.get<RGB>();
		if (im::ColorEdit4(identifier.data(), &temp.x)) {
			parameter.set<RGB>(temp);
		}
	}
	else if (parameter.is<String>()) {
		String temp = parameter.get<String>();
		if (im::InputText(identifier.data(), &temp)) {
			parameter.set<String>(temp);
		}
	}
	else {
		im::Text(kl::format("[unknown] ", identifier).data());
	}
}

void titian::GUISectionScriptingParameters::display_node_parameter_editor(const int script_id, const StringView& name, const std::type_info& type, void* ptr)
{
	if (!ptr) {
		return;
	}
	const String identifier = kl::format(name, "##", script_id);
	if (type == typeid(void*)) {
		String temp = kl::format("0x", *(void**) ptr);
		im::InputText(identifier.data(), &temp, ImGuiInputTextFlags_ReadOnly);
	}
	else if (type == typeid(bool)) {
		im::Checkbox(identifier.data(), (bool*) ptr);
	}
	else if (type == typeid(int)) {
		im::DragInt(identifier.data(), (int*) ptr);
	}
	else if (type == typeid(Int2)) {
		im::DragInt2(identifier.data(), (int*) ptr);
	}
	else if (type == typeid(float)) {
		im::DragFloat(identifier.data(), (float*) ptr);
	}
	else if (type == typeid(Float2)) {
		im::DragFloat2(identifier.data(), (float*) ptr);
	}
	else if (type == typeid(Float3)) {
		im::DragFloat3(identifier.data(), (float*) ptr);
	}
	else if (type == typeid(Float4)) {
		im::DragFloat4(identifier.data(), (float*) ptr);
	}
	else if (type == typeid(Complex)) {
		im::DragFloat2(identifier.data(), (float*) ptr);
	}
	else if (type == typeid(Quaternion)) {
		Float4 temp = *(Quaternion*) ptr;
		if (im::DragFloat4(identifier.data(), &temp.x)) {
			*(Quaternion*) ptr = temp;
		}
	}
	else if (type == typeid(RGB)) {
		Float4 temp = *(RGB*) ptr;
		if (im::ColorEdit4(identifier.data(), &temp.x)) {
			*(RGB*) ptr = temp;
		}
	}
	else if (type == typeid(String)) {
		im::InputText(identifier.data(), (String*) ptr);
	}
	else {
		im::Text(kl::format("[unknown] ", name).data());
	}
}
