#pragma once
#include "hershey.h"

class SimpleShaper
{
public:
    SimpleShaper(HersheyFont font) : m_font(font) {}

    std::vector<ShapedGlyph> layout(const std::string& text, float xoffset = 0.0f, float yoffset = 0.0f) const {
        std::vector<ShapedGlyph> result;
        result.reserve(text.size());
        float cursor = xoffset;
        int line = 0;

        for (char c : text)
        {
            if (c == '\n') {
                cursor = xoffset;
                line++;
                continue;
            }

            const auto& glyph = m_font.chr(c);
            ShapedGlyph shapedGlyph {
                static_cast<uint16_t>(c),                                   // glyph index
                cursor - glyph.leftBearing,                                 // x offset
                yoffset + line * (lineSpacing + 32.0f),                     // y offset  --- TODO: get height from font
                static_cast<float>(glyph.rightBearing - glyph.leftBearing)  // advance
            };
            cursor += shapedGlyph.advance + letterSpacing;
            result.push_back(std::move(shapedGlyph));
        }
        return result;
    }

    float letterSpacing = 0.0f;
    float lineSpacing = 5.0f;
    float scale = 1.0f;
    float angle = 0.0f;

private:
    HersheyFont m_font;
};
