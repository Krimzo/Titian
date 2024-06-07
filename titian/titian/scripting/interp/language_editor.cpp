#include "main.h"


titian::LanguageEditor::LanguageEditor()
	: m_text_editor()
{
	static const bool first_time_init = [&]() {
		// chai
		TextEditor::LanguageDefinition* chai = TextEditor::LanguageDefinition::Chai();
		chai->mKeywords = INTERP_SCRIPT_KEYWORDS;
		chai->mIdentifiers.clear();
		for (const auto& [name, info] : INTERP_SCRIPT_IDENTIFIERS) {
			TextEditor::Identifier identifier = {};
			identifier.mDeclaration = info;
			chai->mIdentifiers[name] = identifier;
		}

		// hlsl
		std::unordered_map<std::string, std::string> HLSL_IDENTIFIERS;
		HLSL_IDENTIFIERS["VS_OUT"] = "Computed vertex data.";
		HLSL_IDENTIFIERS["PS_OUT"] = "Computed pixel data.";

		HLSL_IDENTIFIERS["_vertex_pre"] = "Alter the vertex before the transforms.";
		HLSL_IDENTIFIERS["_vertex_post"] = "Alter the vertex data after the computed transforms.";
		HLSL_IDENTIFIERS["_pixel_pre"] = "Alter the pixel data before the full pixel shader. (return true to cancel the full shader)";
		HLSL_IDENTIFIERS["_pixel_post"] = "Alter the pixel data after the full pixel shader.";

		HLSL_IDENTIFIERS["SHADOW_CASCADE_COUNT"] = "Directional light cascade count.";
		HLSL_IDENTIFIERS["ELAPSED_TIME"] = "Elapsed game time in seconds.";
		HLSL_IDENTIFIERS["DELTA_TIME"] = "Frame time in seconds.";
		HLSL_IDENTIFIERS["CAMERA_POSITION"] = "Camera 3D origin. (position)";
		HLSL_IDENTIFIERS["CAMERA_HAS_SKYBOX"] = "True if camera has a bound skybox, false otherwise.";
		HLSL_IDENTIFIERS["CAMERA_BACKGROUND"] = "Camera backgroun as a float4.";
		HLSL_IDENTIFIERS["AMBIENT_COLOR"] = "Ambient light color.";
		HLSL_IDENTIFIERS["AMBIENT_INTENSITY"] = "Ambient light intensity.";
		HLSL_IDENTIFIERS["SUN_DIRECTION"] = "Sun direction as a normalized float3.";
		HLSL_IDENTIFIERS["SUN_POINT_SIZE"] = "Sun point size.";
		HLSL_IDENTIFIERS["SUN_COLOR"] = "Sun color as a float4.";
		HLSL_IDENTIFIERS["OBJECT_INDEX"] = "Current entity unique index.";
		HLSL_IDENTIFIERS["OBJECT_SCALE"] = "Entity scale as a float3.";
		HLSL_IDENTIFIERS["OBJECT_ROTATION"] = "Entity rotation as a float3.";
		HLSL_IDENTIFIERS["OBJECT_POSITION"] = "Entity position as a float3.";
		HLSL_IDENTIFIERS["MATERIAL_COLOR"] = "Material color as a float4.";
		HLSL_IDENTIFIERS["ALPHA_BLEND"] = "Material alpha blend.";
		HLSL_IDENTIFIERS["TEXTURE_BLEND"] = "Material texture blend.";
		HLSL_IDENTIFIERS["REFLECTION_FACTOR"] = "Material reflection factor.";
		HLSL_IDENTIFIERS["REFRACTION_FACTOR"] = "Material refraction factor.";
		HLSL_IDENTIFIERS["REFRACTION_INDEX"] = "Material refraction index.";
		HLSL_IDENTIFIERS["HAS_NORMAL_MAP"] = "True if material has a normal map bound, false otherwise.";
		HLSL_IDENTIFIERS["HAS_ROUGHNESS_MAP"] = "True if material has a roughness map bound, false otherwise.";
		HLSL_IDENTIFIERS["W"] = "W matrix. (aka model matrix)";
		HLSL_IDENTIFIERS["V"] = "V matrix. (aka view matrix)";
		HLSL_IDENTIFIERS["VP"] = "VP matrix. (aka view/projection matrix)";
		HLSL_IDENTIFIERS["RECEIVES_SHADOWS"] = "True if entity receives shadows, false otherwise.";
		HLSL_IDENTIFIERS["SHADOW_MAP_SIZE"] = "Directional light shadow map resolution.";
		HLSL_IDENTIFIERS["SHADOW_MAP_TEXEL_SIZE"] = "Directional light shadow map texel size.";
		HLSL_IDENTIFIERS["SHADOW_CASCADES"] = "Directional light shadow coverage zones.";
		HLSL_IDENTIFIERS["LIGHT_VPs"] = "Directional light VP matrices.";
		HLSL_IDENTIFIERS["CUSTOM_DATA"] = "Custom user data of a material.";

		HLSL_IDENTIFIERS["SKYBOX_SAMPLER"] = "Skybox sampler.";
		HLSL_IDENTIFIERS["SKYBOX_TEXTURE"] = "Main skybox texture.";
		HLSL_IDENTIFIERS["SHADOW_SAMPLER"] = "Shadow sampler.";
		HLSL_IDENTIFIERS["SHADOW_TEXTURE_0"] = "Shadow cascade texture 0.";
		HLSL_IDENTIFIERS["SHADOW_TEXTURE_1"] = "Shadow cascade texture 1.";
		HLSL_IDENTIFIERS["SHADOW_TEXTURE_2"] = "Shadow cascade texture 2.";
		HLSL_IDENTIFIERS["SHADOW_TEXTURE_3"] = "Shadow cascade texture 3.";
		HLSL_IDENTIFIERS["MATERIAL_SAMPLER"] = "Main map sampler.";
		HLSL_IDENTIFIERS["COLOR_MAP"] = "Material color map.";
		HLSL_IDENTIFIERS["NORMAL_MAP"] = "Material normal map.";
		HLSL_IDENTIFIERS["ROUGHNESS_MAP"] = "Material roughness map.";

		TextEditor::LanguageDefinition* hlsl = TextEditor::LanguageDefinition::Hlsl();
		for (const auto& [name, info] : HLSL_IDENTIFIERS) {
			TextEditor::Identifier identifier = {};
			identifier.mDeclaration = info;
			hlsl->mIdentifiers[name] = identifier;
		}
		return true;
	}();

	// editor
	m_text_editor.SetPalette(TextEditor::PaletteId::Dark);
}

void titian::LanguageEditor::load(const std::string& source)
{
	m_text_editor.SetText(source);
}

void titian::LanguageEditor::edit(std::string* source)
{
	m_text_editor.Render("Interp Editor");
	*source = m_text_editor.GetText();
}

std::string titian::LanguageEditor::get_word_at_cursor() const
{
	return m_text_editor.get_word_at_cursor();
}

void titian::LanguageEditor::replace_word_at_cursor(const std::string_view& text)
{
	m_text_editor.replace_word_at_cursor(text);
}

const TextEditor::LanguageDefinition* titian::LanguageEditor::get_definition() const
{
	switch (m_text_editor.GetLanguageDefinition())
	{
	case TextEditor::LanguageDefinitionId::Chai:
		return TextEditor::LanguageDefinition::Chai();
	case TextEditor::LanguageDefinitionId::Hlsl:
		return TextEditor::LanguageDefinition::Hlsl();
	}
	return nullptr;
}

const TextEditor::Palette* titian::LanguageEditor::get_palette() const
{
	return &m_text_editor.mPalette;
}

void titian::LanguageEditor::load_chai_standard()
{
	m_text_editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Chai);
}

void titian::LanguageEditor::load_hlsl_standard()
{
	m_text_editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Hlsl);
}
