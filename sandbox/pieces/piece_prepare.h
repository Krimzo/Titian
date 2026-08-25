#pragma once

#include "sandbox_piece.h"


namespace titian
{
struct SandboxPiecePrepare : SandboxPiece
{
    SandboxPiecePrepare( TitianEditor& editor );

    void setup_self() override;
};
}
