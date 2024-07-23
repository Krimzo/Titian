#pragma once

#include "sandbox_piece.h"


namespace titian {
	class SandboxPieceSkybox : public SandboxPiece
	{
	public:
		SandboxPieceSkybox(TitianEditor* editor);

		void setup_self() override;
	};
}
