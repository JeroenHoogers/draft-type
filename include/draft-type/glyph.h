#pragma once
#include <cstdint>

namespace DraftType
{
	struct Vert
	{
		int16_t x, y;
	};

	struct Path
	{
		uint16_t offset; // offset into the vertex array (points to first path vertex)
		uint16_t size;	 // number of vertices in the path
	};

	struct ShapedGlyph
	{
		uint16_t glyphIndex;

		float xOffset;
		float yOffset;
		float advance;
	};
} // namespace DraftType
