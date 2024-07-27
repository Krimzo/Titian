#include "sandbox.h"


titian::SandboxPieceSkybox::SandboxPieceSkybox(TitianEditor* editor)
	: SandboxPiece(editor)
{}

void titian::SandboxPieceSkybox::setup_self()
{
	kl::GPU* gpu = &editor->app_layer.gpu;
	Scene* scene = &editor->game_layer.scene;

	Ref<Texture> sky = new Texture(gpu);
	sky->data_buffer = kl::Image("package/skyboxes/clouds.png");
	sky->reload_as_cube();
	sky->create_shader_view(nullptr);
	scene->textures["sky"] = sky;
}
