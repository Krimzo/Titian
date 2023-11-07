#pragma once

#include "sandbox_piece.h"


namespace sandbox {
	class SandboxPieceSkybox : public SandboxPiece
	{
	public:
		SandboxPieceSkybox(TitianEditor* editor);

		void setup_self() override;
	};
}
