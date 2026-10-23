#include <algorithm>
#include <draft-type/shaper.h>

namespace drafttype
{
	namespace
	{
		void calculateLineDims(const HersheyFont& font, const std::string& text, int index, float& width, float& height, const LayoutOptions& opts)
		{
			width = 0;
			height = 0;

			// use font height
			if (opts.lineSpaceMode == LineSpacingMode::Fixed) {
				height = font.height() * opts.scale;
			}

			bool firstGlyph = true;

			// calculate line width and height for current line
			for (size_t j = index; j < text.size(); j++) {
				const char c = text[j];

				if (c == '\n') {
					break;
				}

				const auto& glyph = font.chr(c);

				// letter spacing between glyphs, not after the final glyph.
				if (!firstGlyph) {
					width += opts.letterSpacing;
				}

				// find highest glyph
				if (opts.lineSpaceMode == LineSpacingMode::Dynamic) {
					height = std::max(height, glyph.height * opts.scale);
				}

				width += glyph.advance * opts.scale;
				firstGlyph = false;
			}
		}
	} // namespace

	Bounds measure(const HersheyFont& font, const std::string& text, const LayoutOptions& opts)
	{
		float maxWidth = 0;
		float height = 0;
		uint32_t lineCount = 0;

		bool newLine = true;
		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				float lineWidth, lineHeight;
				calculateLineDims(font, text, i, lineWidth, lineHeight, opts);
				maxWidth = std::max(maxWidth, lineWidth);

				if (opts.lineSpaceMode == LineSpacingMode::Fixed) {
					lineHeight = font.height() * opts.scale;
				}

				height += lineHeight;
				lineCount++;
				newLine = false;
			}
			if (c == '\n') {
				newLine = true;
			}
		}

		if (lineCount > 1) {
			height += (lineCount - 1) * opts.lineSpacing;
		}

		float left = 0.0f;
		float right = maxWidth;

		if (opts.horizontalAlign == HorizontalAlign::Center) {
			left = -maxWidth / 2.0f;
			right = maxWidth / 2.0f;
		} else if (opts.horizontalAlign == HorizontalAlign::Right) {
			left = -maxWidth;
			right = 0.0f;
		}

		float top = 0;
		float bottom = height;

		if (opts.verticalAlign == VerticalAlign::Middle) {
			top = -height * 0.5f;
			bottom = height * 0.5f;
		} else if (opts.verticalAlign == VerticalAlign::Bottom) {
			top = -height;
			bottom = 0.0f;
		}

		return Bounds{top, bottom, left, right};
	}

	std::vector<ShapedGlyph> layout(const HersheyFont& font, const std::string& text, const LayoutOptions& opts, bool sort)
	{
		return layout(font, text, 0, 0, opts, sort);
	}

	std::vector<ShapedGlyph> layout(const HersheyFont& font, const std::string& text, float x, float y, const LayoutOptions& opts, bool sort)
	{
		std::vector<ShapedGlyph> result;
		result.reserve(text.size());

		float lineWidth = 0;
		float lineHeight = 0;
		float baseLine = 0;

		bool newLine = true;

		Bounds bounds = measure(font, text, opts);
		float cursorX = 0;
		float cursorY = bounds.top + y;

		bool firstLine = true;
		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				// add height below baseline from last line
				cursorY += (lineHeight - baseLine);

				calculateLineDims(font, text, i, lineWidth, lineHeight, opts);

				cursorX = x; // left
				if (opts.horizontalAlign == HorizontalAlign::Center) {
					cursorX = x - (lineWidth / 2.0f);
				} else if (opts.horizontalAlign == HorizontalAlign::Right) {
					cursorX = x - lineWidth;
				}

				baseLine = lineHeight * 0.5f; // + m_font.baseoffset();
				cursorY += baseLine;
				if (!firstLine) {
					cursorY += opts.lineSpacing;
				}
				newLine = false;
				firstLine = false;
			}

			if (c == '\n') {
				newLine = true;
				continue;
			}

			// add shaped glyph to output
			const auto& glyph = font.chr(c);
			ShapedGlyph shapedGlyph{
				cursorX,				 // x offset
				cursorY,				 // y offset
				opts.scale,				 // scale
				static_cast<uint32_t>(c) // glyph index
			};
			result.push_back(std::move(shapedGlyph));

			cursorX += opts.scale * glyph.advance + opts.letterSpacing;
		}

		// sort glyphs based on glyph idx (useful for instanced GPU rendering)
		if (sort) {
			std::sort(begin(result), end(result), [](ShapedGlyph a, ShapedGlyph b) { return a.glyphIndex < b.glyphIndex; });
		}

		return result;
	}
} // namespace drafttype
