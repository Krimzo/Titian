#pragma once

#include "sandbox_piece.h"


namespace titian
{
struct SandboxPieceMonkes : SandboxPiece
{
    int size = 0;
    float x_offset = -16.0f;

    SandboxPieceMonkes( TitianEditor& editor, int size );

    void setup_self() override;
};
}
