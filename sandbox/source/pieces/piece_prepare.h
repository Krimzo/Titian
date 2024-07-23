#pragma once

#include "sandbox_piece.h"


namespace titian {
    class SandboxPiecePrepare : public SandboxPiece
    {
    public:
        SandboxPiecePrepare(TitianEditor* editor);

        void setup_self() override;
    };
}
