#include <draft-type/shaper.h>

namespace DraftType
{
	void Shaper::calculateLineDims(const std::string &text, int index, float &width, float &height) const {
		uint16_t lineHeight = 0;
		uint32_t lineWidth = 0;

		// calculate line width and height for current line
		for (size_t j = index; j < text.size(); j++) {
			const char c = text[j];
			const auto &glyph = m_font.chr(c);
			lineWidth += glyph.advance + letterSpacing;
			lineHeight = std::max(lineHeight, glyph.height);

			if (c == '\n') {
				break;
			}
		}

		width = lineWidth * scale;
		height = lineHeight * scale;
	}

	Bounds Shaper::getBounds(const std::string &text) const {
		float maxWidth = 0;
		float cursorY = 0;
		bool newLine = true;
		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				float lineWidth;
				float lineHeight;
				calculateLineDims(text, i, lineWidth, lineHeight);
				maxWidth = std::max(maxWidth, lineWidth);

				if (lineSpaceMode == LineSpacingMode::Fixed) {
					lineHeight = m_font.height();
				}

				cursorY += lineHeight * scale + lineSpacing;
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

	std::vector<ShapedGlyph> Shaper::layout(const std::string &text, float xoffset, float yoffset) const {
		std::vector<ShapedGlyph> result;
		result.reserve(text.size());
		float cursorX = 0;
		float cursorY = yoffset;

		float lineWidth = 0;
		float lineHeight = 0;
		float baseLine = 0;

		bool newLine = true;

		for (size_t i = 0; i < text.size(); i++) {
			const char c = text[i];
			if (newLine) {
				// add height below baseline from last line
				cursorY += (lineHeight - baseLine) * scale;

				calculateLineDims(text, i, lineWidth, lineHeight);

				cursorX = xoffset;
				if (hAlign == HorizontalAlign::Center) {
					cursorX = xoffset - (lineWidth / 2.0f) * scale;
				} else if (hAlign == HorizontalAlign::Right) {
					cursorX = xoffset - lineWidth * scale;
				}

				if (lineSpaceMode == LineSpacingMode::Fixed) {
					lineHeight = m_font.height();
				}
				baseLine = lineHeight * 0.5f; // + m_font.baseoffset();
				cursorY += baseLine * scale + lineSpacing;

				newLine = false;
			}

			if (c == '\n') {
				newLine = true;
				continue;
			}

			// add shaped glyph to output
			const auto &glyph = m_font.chr(c);
			ShapedGlyph shapedGlyph{
				static_cast<uint16_t>(c),		  // glyph index
				cursorX,						  // x offset
				cursorY,						  // y offset
				static_cast<float>(glyph.advance) // advance
			};
			cursorX += shapedGlyph.advance + letterSpacing;
			result.push_back(std::move(shapedGlyph));
		}

		return result;
	}
} // namespace DraftType
