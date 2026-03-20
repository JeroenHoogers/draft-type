#pragma once
#include "hershey.h"

namespace DraftType
{
    class Shaper
    {
    public:
        Shaper(HersheyFont font) : m_font(font) {}

        std::vector<ShapedGlyph> layout(const std::string& text, float xoffset = 0.0f, float yoffset = 0.0f) const;
        float letterSpacing = 0.0f;
        float lineSpacing = 5.0f;
        float scale = 1.0f;
        float angle = 0.0f;

    private:
        HersheyFont m_font;
    };
}
