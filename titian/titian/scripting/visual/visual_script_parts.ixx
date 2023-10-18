export module visual_script_parts;

export import klib;

static std::atomic<int> id_counter = {};

export namespace titian {
    struct VisualPin
    {
        enum class Type
        {
            // Flow
            FLOW,
            BOOL,

            // Numbers
            INT,
            FLOAT,

            // Text
            CHAR,
            STRING,

            // Components
            MESH,
            TEXTURE,
            MATERIAL,
            COLLIDER,

            // Scene
            ENTITY,
            CAMERA,
            LIGHT,
            AMBIENT_LIGHT,
            POINT_LIGHT,
            DIRECTIONAL_LIGHT,
        } type = {};

        enum class Kind
        {
            OUTPUT,
            INPUT,
        } kind = {};

        const int id = ++id_counter;
        std::string name = {};

        VisualPin()
        {}
    };
}

export namespace titian {
    struct VisualNode
    {
        enum class Type
        {
            OBJECT,
            ACTION,
            COMMENT,
        } type = {};

        const int id = ++id_counter;
        std::string title = {};
        kl::Color color = {};
        std::vector<VisualPin> pins = {};

        VisualNode()
        {}
    };
}

export namespace titian {
    struct VisualLink
    {
        const int id = ++id_counter;
        int start_pin_id = 0;
        int end_pin_id = 0;

        VisualLink()
        {}
    };
}
