#pragma once
#include "hershey.h"

namespace DraftType
{
	enum class HorizontalAlign { Left, Center, Right };
	enum class VerticalAlign { Top, Middle, Bottom };

	struct Bounds
	{
		float top;
		float bottom;
		float left;
		float right;
	};

	class Shaper
	{
	public:
		Shaper(HersheyFont font) : m_font(font) {}
		Shaper(HersheyFont &&font) : m_font(std::move(font)) {}

		std::vector<ShapedGlyph> layout(const std::string &text,
										float xoffset = 0.0f,
										float yoffset = 0.0f) const;
		float letterSpacing = 0.0f;
		float lineSpacing = 5.0f;
		float scale = 1.0f;
		float angle = 0.0f;
		HorizontalAlign hAlign = HorizontalAlign::Left;
		VerticalAlign vAlign = VerticalAlign::Bottom;

		inline Bounds getBounds(const std::string &text) const {
			float maxX = 0;
			float cursorX = 0;
			float cursorY = m_font.height() * 2;

			for (char c : text) {
				if (c == '\n') {
					cursorX = 0;
					cursorY += m_font.height();
					continue;
				}

				const auto &glyph = m_font.chr(c);
				cursorX += glyph.advance + letterSpacing;
				maxX = std::max(maxX, cursorX);
			}

			return Bounds{
				0,		 // top
				cursorY, // bottom
				0,		 // left
				maxX	 // right
			};
		}

	private:
		inline float calculateTextWidth(const std::string &text) const {
			float width = 0.0f;
			for (char c : text) {
				width += (m_font.chr(c).advance + letterSpacing) * scale;
			}
			return width;
		}

		HersheyFont m_font;
	};
} // namespace DraftType
