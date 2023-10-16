export module texture;

export import serializable;

export namespace titian {
	class Texture : public Serializable
	{
	public:
		using Data = kl::Image;

		Data data_buffer = {};
		kl::dx::Texture graphics_buffer = nullptr;

		kl::dx::TargetView target_view = nullptr;
		kl::dx::DepthView depth_view = nullptr;
		kl::dx::ShaderView shader_view = nullptr;
		kl::dx::AccessView access_view = nullptr;

		Texture(kl::Object<kl::GPU>& gpu)
			: m_gpu(gpu)
		{}

		~Texture() override
		{}

		void serialize(Serializer* serializer) const override
		{
			const kl::Int2 size = data_buffer.size();
			serializer->write_object<kl::Int2>(size);

			const kl::Color* data = static_cast<const kl::Color*>(data_buffer);
			serializer->write_array<kl::Color>(data, size.x * size.y);

			serializer->write_object<bool>(m_is_cube);
		}

		void deserialize(const Serializer* serializer) override
		{
			const kl::Int2 size = serializer->read_object<kl::Int2>();
			data_buffer.resize(size);

			kl::Color* data = static_cast<kl::Color*>(data_buffer);
			serializer->read_array<kl::Color>(data, size.x * size.y);

			serializer->read_object<bool>(m_is_cube);
			if (m_is_cube) {
				load_as_cube();
			}
			else {
				load_as_2D(false, false);
			}
			create_shader_view(nullptr);
		}

		void load_as_2D(bool has_unordered_access, bool is_target)
		{
			graphics_buffer = m_gpu->create_texture(data_buffer, has_unordered_access, is_target);
			m_is_cube = false;
		}

		bool load_as_cube()
		{
			if (data_buffer.width() % 4 || data_buffer.height() % 3) {
				return false;
			}

			const int part_size = data_buffer.width() / 4;
			const kl::Image box_sides[6] = {
				data_buffer.rectangle(kl::Int2(1, 1) * part_size, kl::Int2(2, 2) * part_size),
				data_buffer.rectangle(kl::Int2(3, 1) * part_size, kl::Int2(4, 2) * part_size),
				data_buffer.rectangle(kl::Int2(0, 1) * part_size, kl::Int2(1, 2) * part_size),
				data_buffer.rectangle(kl::Int2(2, 1) * part_size, kl::Int2(3, 2) * part_size),
				data_buffer.rectangle(kl::Int2(1, 0) * part_size, kl::Int2(2, 1) * part_size),
				data_buffer.rectangle(kl::Int2(1, 2) * part_size, kl::Int2(2, 3) * part_size),
			};

			graphics_buffer = m_gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
			m_is_cube = true;
			return true;
		}

		void create_target_view(const kl::dx::TargetViewDescriptor* descriptor)
		{
			target_view = m_gpu->create_target_view(graphics_buffer, descriptor);
		}

		void  create_depth_view(const kl::dx::DepthViewDescriptor* descriptor)
		{
			depth_view = m_gpu->create_depth_view(graphics_buffer, descriptor);
		}

		void create_shader_view(const kl::dx::ShaderViewDescriptor* descriptor)
		{
			shader_view = m_gpu->create_shader_view(graphics_buffer, descriptor);
		}

		void create_access_view(const kl::dx::AccessViewDescriptor* descriptor)
		{
			access_view = m_gpu->create_access_view(graphics_buffer, descriptor);
		}

	private:
		kl::Object<kl::GPU> m_gpu = nullptr;
		bool m_is_cube = false;
	};
}
