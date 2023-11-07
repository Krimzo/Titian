#pragma once

#include "sandbox_piece.h"


namespace sandbox {
    class SandboxPiecePhysicsTest : public SandboxPiece
    {
    public:
        int size = 0;

        SandboxPiecePhysicsTest(TitianEditor* editor, int size);

        void setup_self() override;

    private:
        void setup_platform(Scene* scene);
        void setup_objects(Scene* scene);
    };
}
