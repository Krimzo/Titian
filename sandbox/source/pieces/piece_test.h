#pragma once

#include "sandbox_piece.h"


namespace titian {
    class SandboxPieceTest : public SandboxPiece
    {
    public:
        SandboxPieceTest(TitianEditor* editor);

        void setup_self() override;
    };
}
