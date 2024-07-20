#include "main.h"


int sandbox_entry(const int argc, const char** argv)
{
    using namespace sandbox;
    TitianEditor editor{};

    // Pieces
    Vector<Ref<SandboxPiece>> pieces{
        new SandboxPieceTest(&editor),
        new SandboxPiecePrepare(&editor),
        new SandboxPieceSkybox(&editor),
        new SandboxPieceMonkes(&editor, 15),
        new SandboxPieceCubes(&editor, 15),
        new SandboxPieceSpheres(&editor, 7),
        new SandboxPiecePhysicsTest(&editor, 9),
    };
    for (auto& piece : pieces) {
        piece->setup_self();
    }

    // Loop
    while (editor.process_layers());
    return 0;
}
