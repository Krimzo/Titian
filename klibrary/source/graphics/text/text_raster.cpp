#include "klibrary.h"


// Init
kl::TextRaster::TextRaster()
{
	// D2D1
	D2D1_FACTORY_OPTIONS options = {};
	options.debugLevel = static_cast<D2D1_DEBUG_LEVEL>(kl::IS_DEBUG ? (D2D1_DEBUG_LEVEL_WARNING | D2D1_DEBUG_LEVEL_ERROR) : D2D1_DEBUG_LEVEL_NONE);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &options, &m_d2d1_factory) >> verify_result;

	// DirectWrite
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_write_factory) >> verify_result;
}

// Create
kl::ComPtr<IDWriteTextFormat> kl::TextRaster::create_text_format(
	const std::wstring_view& font_family,
	const DWRITE_FONT_WEIGHT font_weight,
	const DWRITE_FONT_STYLE font_style,
	const float font_size,
	const std::wstring_view& locale
) const
{
	ComPtr<IDWriteTextFormat> format{};
	m_write_factory->CreateTextFormat(
		font_family.data(), nullptr, font_weight, font_style, DWRITE_FONT_STRETCH_NORMAL,
		font_size, locale.data(), &format
	) >> verify_result;
	return format;
}

// Draw
void kl::TextRaster::render_text(const UINT target_index) const
{
	// Prep
	auto& target = m_d2d1_targets[target_index];
	const D2D1_SIZE_F target_size = target->GetSize();
	D2D1_RECT_F layout_rect = D2D1::RectF(0.0f, 0.0f, target_size.width, target_size.height);
	ComPtr<ID2D1SolidColorBrush> brush{};

	// Draw
	target->BeginDraw();
	for (auto& text : texts) {
		// Prepare
		layout_rect.left = text.position.x;
		layout_rect.top = text.position.y;
		target->CreateSolidColorBrush(
			D2D1_COLOR_F{
				.r = text.color.x,
				.g = text.color.y,
				.b = text.color.z,
				.a = text.color.w,
			},
			&brush
		);

		// Draw
		target->DrawTextW(
			text.data.c_str(),
			(UINT) text.data.size(),
			text.format.Get(),
			layout_rect,
			brush.Get()
		);
	}
	target->EndDraw();
}
