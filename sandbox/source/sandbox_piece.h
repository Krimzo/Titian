#pragma once

#include "titian_editor.h"


namespace titian {
	struct SandboxPiece
	{
		TitianEditor& editor;

		SandboxPiece(TitianEditor& editor);
		virtual ~SandboxPiece() = default;

		virtual void setup_self() = 0;
	};
}
