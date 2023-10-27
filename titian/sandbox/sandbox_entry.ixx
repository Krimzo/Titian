export module sandbox_entry;

export import piece_prepare;
export import piece_skybox;
export import piece_monkes;
export import piece_cubes;
export import piece_spheres;
export import piece_physics_test;

export int sandbox_entry()
{
    using namespace titian;
    TitianEditor editor = {};

    // Unpackage
    open_package(PACKAGED_PREVIEW_FILE);

    // Collect pieces
    std::vector<kl::Object<SandboxPiece>> pieces = {
        new SandboxPiecePrepare(&editor),
        new SandboxPieceSkybox(&editor),
        new SandboxPieceMonkes(&editor, 15),
        new SandboxPieceCubes(&editor, 15),
        new SandboxPieceSpheres(&editor, 7),
        new SandboxPiecePhysicsTest(&editor, 9),
    };

    // Setup pieces
    for (auto& piece : pieces) {
        piece->setup_self();
    }

    // Loop
    while (editor.process_layers());
    return 0;
}
