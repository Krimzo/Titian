#include "klibrary.h"


kl::TextRaster::TextRaster()
{
	D2D1_FACTORY_OPTIONS options{};
	options.debugLevel = D2D1_DEBUG_LEVEL(kl::IS_DEBUG ? (D2D1_DEBUG_LEVEL_WARNING | D2D1_DEBUG_LEVEL_ERROR) : D2D1_DEBUG_LEVEL_NONE);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &options, (void**) &m_d2d1_factory) >> verify_result;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &m_write_factory) >> verify_result;
}

kl::TextFormat kl::TextRaster::create_text_format(
	const std::wstring_view& font_family,
	const DWRITE_FONT_WEIGHT font_weight,
	const DWRITE_FONT_STYLE font_style,
	const float font_size,
	const std::wstring_view& locale
) const
{
	TextFormat format{};
	m_write_factory->CreateTextFormat(
		font_family.data(), nullptr, font_weight, font_style, DWRITE_FONT_STRETCH_NORMAL,
		font_size, locale.data(), &format
	) >> verify_result;
	return format;
}

void kl::TextRaster::draw_text(const UINT target_index) const
{
	auto& target = m_d2d1_targets[target_index];
	ComRef<ID2D1SolidColorBrush> brush{};
	const D2D1_SIZE_F target_size = target->GetSize();
	D2D1_RECT_F layout_rect{};

	target->BeginDraw();
	for (const auto& text : text_data) {
		layout_rect.left = text.position.x;
		layout_rect.right = layout_rect.left + (text.rect_size.x > 0.0f ? text.rect_size.x : target_size.width);
		layout_rect.top = text.position.y;
		layout_rect.bottom = layout_rect.top + (text.rect_size.y > 0.0f ? text.rect_size.y : target_size.height);
		
		target->CreateSolidColorBrush(
			D2D1_COLOR_F{
				.r = text.color.x,
				.g = text.color.y,
				.b = text.color.z,
				.a = text.color.w,
			},
			&brush
		);

		target->DrawText(
			text.data.data(),
			(UINT) text.data.size(),
			text.format.get(),
			layout_rect,
			brush.get()
		);
	}
	target->EndDraw();
}
