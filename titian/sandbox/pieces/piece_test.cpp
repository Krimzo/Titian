#include "main.h"


sandbox::SandboxPieceTest::SandboxPieceTest(TitianEditor* editor)
	: SandboxPiece(editor)
{}

void sandbox::SandboxPieceTest::setup_self()
{
    /* for testing purposes */

    kl::GPU* gpu = &editor->app_layer.gpu;
	Scene* scene = &editor->game_layer.scene;
}
