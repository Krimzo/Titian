export module node_script_parts;

export import serializable;

static std::atomic<int> id_counter = {};

export namespace titian {
    class UniqueObject
    {
    public:
        const int id = ++id_counter;

        UniqueObject()
        {}

        UniqueObject(const UniqueObject&) = delete;
        UniqueObject(const UniqueObject&&) = delete;

        void operator=(const UniqueObject&) = delete;
        void operator=(const UniqueObject&&) = delete;
    };
}

export namespace titian {
    struct FlowNodePin : public UniqueObject
    {
        enum class Type
        {
            FLOW,

            BOOL,
            NUMBER,

            CHAR,
            STRING,

            MESH,
            TEXTURE,
            MATERIAL,
            COLLIDER,

            ENTITY,
            CAMERA,
            LIGHT,
            AMBIENT_LIGHT,
            POINT_LIGHT,
            DIRECTIONAL_LIGHT,
        } type = {};

        enum class Kind
        {
            INPUT,
            OUTPUT,
        } kind = {};

        std::string name = {};

        FlowNodePin()
        {}

        ImNodesPinShape shape() const
        {
            switch (type) {
            case Type::FLOW:
                return ImNodesPinShape_TriangleFilled;

            case Type::BOOL:
            case Type::NUMBER:
            case Type::CHAR:
            case Type::STRING:
                return ImNodesPinShape_CircleFilled;

            case Type::MESH:
            case Type::TEXTURE:
            case Type::MATERIAL:
            case Type::COLLIDER:
            case Type::ENTITY:
            case Type::CAMERA:
            case Type::LIGHT:
            case Type::AMBIENT_LIGHT:
            case Type::POINT_LIGHT:
            case Type::DIRECTIONAL_LIGHT:
                return ImNodesPinShape_QuadFilled;
            }
            return ImNodesPinShape_CircleFilled;
        }

        ImColor color() const
        {
            switch (type) {
            case Type::FLOW:
                return ImColor(255, 255, 255);

            case Type::BOOL:
                return ImColor(170, 70, 50);
            case Type::NUMBER:
                return ImColor(115, 200, 100);
            case Type::CHAR:
                return ImColor(190, 115, 35);
            case Type::STRING:
                return ImColor(190, 180, 35);

            case Type::MESH:
            case Type::TEXTURE:
            case Type::MATERIAL:
            case Type::COLLIDER:
            case Type::ENTITY:
            case Type::CAMERA:
            case Type::LIGHT:
            case Type::AMBIENT_LIGHT:
            case Type::POINT_LIGHT:
            case Type::DIRECTIONAL_LIGHT:
                return ImColor(35, 150, 190);
            }
            return ImColor(255, 255, 255);
        }
    };
}

export namespace titian {
    struct FlowNode : public UniqueObject
    {
        std::string title = {};
        std::vector<kl::Object<FlowNodePin>> pins = {};

        FlowNode()
        {}

        void set_position(const kl::Float2& position)
        {
            ImNodes::SetNodeGridSpacePos(id, { position.x, -position.y });
        }

        kl::Float2 position() const
        {
            const ImVec2 result = ImNodes::GetNodeGridSpacePos(id);
            return { result.x, -result.y };
        }
    };
}

export namespace titian {
    struct FlowLink : public UniqueObject
    {
        int start_id = {};
        int end_id = {};

        FlowLink()
        {}

        FlowLink(const int start, const int end)
            : start_id(start), end_id(end)
        {}
    };
}
