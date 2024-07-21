#pragma once

#include "sandbox_piece.h"


namespace sandbox {
    class SandboxPieceTest : public SandboxPiece
    {
    public:
        SandboxPieceTest(TitianEditor* editor);

        void setup_self() override;
    };
}
