#pragma once

#include "sandbox_piece.h"


namespace titian
{
struct SandboxPieceCubes : SandboxPiece
{
    int size = 0;
    float x_offset = 16.0f;

    SandboxPieceCubes( TitianEditor& editor, int size );

    void setup_self() override;
};
}
