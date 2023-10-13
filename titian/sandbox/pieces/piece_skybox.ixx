export module piece_skybox;

export import sandbox_piece;

export namespace titian {
	class SandboxPieceSkybox : public SandboxPiece
	{
	public:
		SandboxPieceSkybox(TitianEditor* editor)
			: SandboxPiece(editor)
		{}

		~SandboxPieceSkybox() override
		{}

		void setup_self() override
		{
			kl::Object<kl::GPU> gpu = editor->app_layer->gpu;
			kl::Object<Scene> scene = editor->game_layer->scene;

			kl::Object<Texture> sky = new Texture(gpu);
			sky->data_buffer = kl::Image("preview/skyboxes/clouds.png");

			sky->load_as_cube();
			sky->create_shader_view(nullptr);

			scene->textures["sky"] = sky;
			scene->camera->skybox = "sky";
		}
	};
}
