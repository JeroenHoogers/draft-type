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
		VerticalAlign verticalAlign = VerticalAlign::Bottom;
	};

	class Shaper
	{

	public:
		Shaper() = delete;

		static std::vector<ShapedGlyph> layout(const HersheyFont &font, const std::string &text,
											   float xoffset = 0.0f,
											   float yoffset = 0.0f,
											   const LayoutOptions &opts = {});

		static Bounds measure(const HersheyFont &font, const std::string &text, const LayoutOptions &opts = {});

	private:
		static void calculateLineDims(const HersheyFont &font, const std::string &text, int index, float &width, float &height, const LayoutOptions &opts);
	};
} // namespace DraftType
