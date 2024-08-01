#pragma once

#include "track/track.h"
#include "render/frame_handler.h"


namespace titian {
	class FuzeRenderer
	{
	public:
		FuzeRenderer();

		float progress() const;

	private:
		FrameHandler m_handler;
	};
}
