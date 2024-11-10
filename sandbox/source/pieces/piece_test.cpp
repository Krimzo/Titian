#include "sandbox.h"


titian::SandboxPieceTest::SandboxPieceTest( TitianEditor& editor )
    : SandboxPiece( editor )
{}

void titian::SandboxPieceTest::setup_self()
{
    kl::GPU& gpu = editor.app_layer.gpu;
    Scene& scene = editor.game_layer.scene();
}
