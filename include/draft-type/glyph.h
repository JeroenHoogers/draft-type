#pragma once
#include <cstdint>
#include <vector>

namespace DraftType
{
	struct Vert
	{
		int16_t x, y;
	};

	struct Path
	{
		uint32_t offset; // offset into the vertex array (points to first path vertex)
		uint32_t size;	 // number of vertices in the path
	};

	struct ShapedGlyph
	{
		float x, y;
		float scale;
		uint32_t glyphIndex;
	};
} // namespace DraftType
