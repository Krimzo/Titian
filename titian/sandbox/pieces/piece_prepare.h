#pragma once

#include "sandbox_piece.h"


namespace sandbox {
    class SandboxPiecePrepare : public SandboxPiece
    {
    public:
        SandboxPiecePrepare(TitianEditor* editor);

        void setup_self() override;
    };
}
