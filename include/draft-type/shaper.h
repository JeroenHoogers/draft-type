#pragma once
#include "hershey.h"

namespace drafttype
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
		Fixed,	// height of tallest character in the font
		Dynamic // height of tallest character in the line
	};

	struct Bounds
	{
		float top;
		float bottom;
		float left;
		float right;
	};

	struct LayoutOptions
	{
		float letterSpacing = 0.0f;
		float lineSpacing = 5.0f;
		float scale = 1.0f;
		float angle = 0.0f; // Not (yet) supported

		LineSpacingMode lineSpaceMode = LineSpacingMode::Fixed;

		HorizontalAlign horizontalAlign = HorizontalAlign::Left;
		VerticalAlign verticalAlign = VerticalAlign::Top;
	};

	std::vector<ShapedGlyph> layout(const HersheyFont &font, const std::string &text,
									float x, float y,
									const LayoutOptions &opts = {}, bool sort = false);

	std::vector<ShapedGlyph> layout(const HersheyFont &font, const std::string &text,
									const LayoutOptions &opts = {}, bool sort = false);

	Bounds measure(const HersheyFont &font, const std::string &text, const LayoutOptions &opts = {});
} // namespace drafttype
