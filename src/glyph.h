#pragma once
#include <cstdint>

namespace DraftType
{
    struct Vert {
        int16_t x, y;
    };

    struct Segment
    {
        uint16_t i0;
        uint16_t i1;
    };

    struct ShapedGlyph {
        uint16_t glyphIndex;
        // uint16_t width;

        float xOffset;
        float yOffset;
        float advance;
    };
}
