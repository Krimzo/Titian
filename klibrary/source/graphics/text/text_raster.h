#pragma once

#include "apis/apis.h"


namespace kl {
	class TextRaster
	{
	public:
		static inline constexpr UINT BUFFER_COUNT = 2;

		struct Text
		{
			ComPtr<IDWriteTextFormat> format{};
			kl::Float4 color{ 1.0f };
			kl::Float2 position{};
			std::wstring data{};
		};

	protected:
		ComPtr<ID2D1Factory> m_d2d1_factory{};
		ComPtr<IDWriteFactory> m_write_factory{};
		ComPtr<ID2D1RenderTarget> m_d2d1_targets[BUFFER_COUNT] = {};

	public:
		std::vector<Text> texts{};

		// Init
		TextRaster();

		// Copy
		TextRaster(const TextRaster&) = delete;
		TextRaster(TextRaster&&) = delete;

		void operator=(const TextRaster&) = delete;
		void operator=(TextRaster&&) = delete;

		// Create
		ComPtr<IDWriteTextFormat> create_text_format(
			const std::wstring_view& font_family,
			DWRITE_FONT_WEIGHT font_weight,
			DWRITE_FONT_STYLE font_style,
			float font_size,
			const std::wstring_view& locale = L"en-us"
		) const;

		// Draw
		void render_text(UINT target_index) const;
	};
}
