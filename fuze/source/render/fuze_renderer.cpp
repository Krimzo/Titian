#include "fuze.h"


titian::FuzeRenderer::FuzeRenderer()
{
	m_handler.init();
}

float titian::FuzeRenderer::progress() const
{
	return 0.0f;
}
