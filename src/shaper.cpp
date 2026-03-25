#include <draft-type/shaper.h>

namespace DraftType
{
	void calculateLineDims(const HersheyFont &font, const std::string &text, int index, float &width, float &height, const LayoutOptions &opts) {
		uint16_t lineHeight = 0;
		uint32_t lineWidth = 0;

		// calculate line width and height for current line
		for (size_t j = index; j < text.size(); j++) {
			const char c = text[j];
			const auto &glyph = font.chr(c);
			lineWidth += glyph.advance * opts.scale + opts.letterSpacing;
			lineHeight = std::max(lineHeight, glyph.height);

			if (c == '\n') {
				break;
			}
		}

		width = lineWidth;
		height = lineHeight * opts.scale;
	}

	Bounds measure(const HersheyFont &font, const std::string &text, const LayoutOptions &opts) {
		float maxWidth = 0;
		float cursorY = 0;
		bool newLine = true;
		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				float lineWidth;
				float lineHeight;
				calculateLineDims(font, text, i, lineWidth, lineHeight, opts);
				maxWidth = std::max(maxWidth, lineWidth);

				if (opts.lineSpaceMode == LineSpacingMode::Fixed) {
					lineHeight = font.height() * opts.scale;
				}

				cursorY += lineHeight + opts.lineSpacing;
				newLine = false;
			}
			if (c == '\n') {
				newLine = true;
			}
		}

		return Bounds{
			0, cursorY, // top - bottom
			0, maxWidth // left - right
		};
	}

	std::vector<ShapedGlyph> layout(const HersheyFont &font, const std::string &text, float x, float y, const LayoutOptions &opts) {
		std::vector<ShapedGlyph> result;
		result.reserve(text.size());
		float cursorX = 0;
		float cursorY = y;

		float lineWidth = 0;
		float lineHeight = 0;
		float baseLine = 0;

		bool newLine = true;

		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				// add height below baseline from last line
				cursorY += (lineHeight - baseLine) * opts.scale;

				calculateLineDims(font, text, i, lineWidth, lineHeight, opts);

				cursorX = x;
				if (opts.horizontalAlign == HorizontalAlign::Center) {
					cursorX = x - (lineWidth / 2.0f);
				} else if (opts.horizontalAlign == HorizontalAlign::Right) {
					cursorX = x - lineWidth;
				}

				if (opts.lineSpaceMode == LineSpacingMode::Fixed) {
					lineHeight = font.height();
				}
				baseLine = lineHeight * 0.5f; // + m_font.baseoffset();
				cursorY += baseLine * opts.scale + opts.lineSpacing;

				newLine = false;
			}

			if (c == '\n') {
				newLine = true;
				continue;
			}

			// add shaped glyph to output
			const auto &glyph = font.chr(c);
			ShapedGlyph shapedGlyph {
				static_cast<uint16_t>(c), // glyph index
				cursorX,				  // x offset
				cursorY,				  // y offset
				opts.scale				  // scale
			};
			result.push_back(std::move(shapedGlyph));

			cursorX += opts.scale * glyph.advance + opts.letterSpacing;
		}
		return result;
	}
} // namespace DraftType
