#pragma once

#include "sandbox_piece.h"


namespace sandbox {
    class SandboxPieceMonkes : public SandboxPiece
    {
    public:
        int size = 0;
        float x_offset = -16.0f;

        SandboxPieceMonkes(TitianEditor* editor, int size);

        void setup_self() override;
    };
}
