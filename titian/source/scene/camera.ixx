export module camera;

export import entity;

export namespace titian {
    class Camera : public Entity
    {
    public:
        float aspect_ratio = 1.7778f;
        float field_of_view = 75.0f;

        float near_plane = 0.01f;
        float far_plane = 500.0f;

        float sensitivity = 0.1f;
        float speed = 2.0f;

        kl::Color background = {};
        kl::Object<Texture> skybox = nullptr;

        Camera(physx::PxPhysics* physics, const bool dynamic)
            : Entity(physics, dynamic)
        {}

        ~Camera() override
        {}

        void serialize(kl::File* file) const override
        {
            Entity::serialize(file);
            file->write(m_forward);
            file->write(m_up);
            file->write(aspect_ratio);
            file->write(field_of_view);
            file->write(near_plane);
            file->write(far_plane);
            file->write(sensitivity);
            file->write(speed);
            file->write(background);
        }

        void deserialize(const kl::File* file) override
        {
            Entity::deserialize(file);
            file->read(m_forward);
            file->read(m_up);
            file->read(aspect_ratio);
            file->read(field_of_view);
            file->read(near_plane);
            file->read(far_plane);
            file->read(sensitivity);
            file->read(speed);
            file->read(background);
        }

        void update_aspect_ratio(const kl::Int2& size)
        {
            aspect_ratio = size.x / (float) size.y;
        }

        void set_forward(const kl::Float3& dir)
        {
            m_forward = normalize(dir);
        }

        void set_up(const kl::Float3& dir)
        {
            m_up = normalize(dir);
        }

        kl::Float3 forward() const
        {
            return m_forward;
        }

        kl::Float3 right() const
        {
            return cross(m_up, m_forward);
        }

        kl::Float3 up() const
        {
            return m_up;
        }

        void move_forward(const float delta_time)
        {
            set_position(position() + m_forward * (speed * delta_time));
        }

        void move_back(const float delta_time)
        {
            set_position(position() - m_forward * (speed * delta_time));
        }

        void move_right(const float delta_time)
        {
            set_position(position() + right() * (speed * delta_time));
        }

        void move_left(const float delta_time)
        {
            set_position(position() - right() * (speed * delta_time));
        }

        void move_up(const float delta_time)
        {
            set_position(position() + m_up * (speed * delta_time));
        }

        void move_down(const float delta_time)
        {
            set_position(position() - m_up * (speed * delta_time));
        }

        void rotate(const kl::Float2& mouse_pos, const kl::Float2& frame_center, const float vertical_angle_limit = 85.0f)
        {
            const kl::Float2 rotation = (mouse_pos - frame_center) * sensitivity;
            const kl::Float3 forward_vert = kl::rotate(m_forward, right(), rotation.y);
            if (std::abs(angle(forward_vert, m_up) - 90.0f) <= vertical_angle_limit) {
                set_forward(forward_vert);
            }
            set_forward(kl::rotate(m_forward, m_up, rotation.x));
        }

        kl::Float4x4 view_matrix() const
        {
            const kl::Float3 position = this->position();
            return kl::Float4x4::look_at(position, position + m_forward, m_up);
        }

        kl::Float4x4 projection_matrix() const
        {
            return kl::Float4x4::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
        }

        kl::Float4x4 matrix() const
        {
            return projection_matrix() * view_matrix();
        }
        
    private:
        kl::Float3 m_forward = { 0.0f, 0.0f, 1.0f };
        kl::Float3 m_up = { 0.0f, 1.0f, 0.0f };
    };
}
