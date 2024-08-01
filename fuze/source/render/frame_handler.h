#pragma once

#include "storage/frame.h"


namespace titian {
	class FrameHandler
	{
	public:
		Frame out_frame;

		FrameHandler();

		void init();

		void prepare_frame(const Int2& size);
		void mix_frame(const Frame& frame);

	private:
		dx::ComputeShader m_clear_shader;
		dx::ComputeShader m_mix_shader;
	};
}
