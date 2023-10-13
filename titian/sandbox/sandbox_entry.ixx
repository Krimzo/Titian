export module sandbox_entry;

export import piece_skybox;

export int sandbox_entry()
{
    using namespace titian;
    TitianEditor editor = {};

    // Collect pieces
    std::vector<kl::Object<SandboxPiece>> pieces = {};
    pieces.emplace_back(new SandboxPieceSkybox(&editor));

    // Setup pieces
    for (auto& piece : pieces) {
        piece->setup_self();
    }

    // Loop
    while (editor.process_layers()) {}
    return 0;
}
