#pragma once

#include "titian/titian_editor.h"


namespace sandbox {
	using namespace titian;

	class SandboxPiece
	{
	public:
		TitianEditor* editor = nullptr;

		SandboxPiece(TitianEditor* editor);
		virtual ~SandboxPiece() = default;

		virtual void setup_self() = 0;
	};
}
