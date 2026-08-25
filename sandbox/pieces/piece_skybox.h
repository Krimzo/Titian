#pragma once

#include "sandbox_piece.h"


namespace titian
{
struct SandboxPieceSkybox : SandboxPiece
{
    SandboxPieceSkybox( TitianEditor& editor );

    void setup_self() override;
};
}
