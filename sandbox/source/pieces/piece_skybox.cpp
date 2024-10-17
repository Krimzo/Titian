#include "sandbox.h"


titian::SandboxPieceSkybox::SandboxPieceSkybox(TitianEditor& editor)
	: SandboxPiece(editor)
{}

void titian::SandboxPieceSkybox::setup_self()
{
	Scene& scene = editor.game_layer.scene();

	Ref<Texture> sky = new Texture();
	sky->image.load_from_file("package/skyboxes/clouds.png");
	sky->reload_as_cube();
	sky->create_shader_view();
	scene.textures["sky"] = sky;
}
