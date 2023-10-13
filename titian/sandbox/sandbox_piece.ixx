export module sandbox_piece;

export import titian_editor;

export namespace titian {
	class SandboxPiece
	{
	public:
		TitianEditor* editor = nullptr;

		SandboxPiece(TitianEditor* editor)
		{
			this->editor = editor;
		}

		virtual ~SandboxPiece()
		{}

		virtual void setup_self() = 0;
	};
}
