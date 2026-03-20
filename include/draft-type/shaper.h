#pragma once
#include "hershey.h"

namespace DraftType
{
	enum class HorizontalAlign {
		Left,
		Center,
		Right
	};

	enum class VerticalAlign {
		Top,
		Middle,
		Bottom
	};

	enum class LineSpacingMode {
		Fixed,		// height of tallest character in the font
		Dynamic		// height of tallest character in the line
	};

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

		void calculateLineDims(const std::string &text, int index, float &width,
							   float &height) const;
		Bounds getBounds(const std::string &text) const;

		float letterSpacing = 0.0f;
		float lineSpacing = 5.0f;
		float scale = 1.0f;
		float angle = 0.0f;

		LineSpacingMode lineSpaceMode = LineSpacingMode::Fixed;

		HorizontalAlign hAlign = HorizontalAlign::Left;
		VerticalAlign vAlign = VerticalAlign::Bottom;

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
