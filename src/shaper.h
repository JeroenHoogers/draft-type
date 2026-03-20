#pragma once
#include "hershey.h"

class SimpleShaper
{
public:
    SimpleShaper(HersheyFont font) : m_font(font) {}

    std::vector<ShapedGlyph> layout(const std::string& text) const {
        std::vector<ShapedGlyph> result;
        result.reserve(text.size());
        float cursor = 20.0f;
        int line = 1;

        for (char c : text)
        {
            if (c == '\n') {
                cursor = 20.0f;
                line++;
                continue;
            }

            const auto& glyph = m_font.chr(c);
            ShapedGlyph shapedGlyph {
                static_cast<uint16_t>(c),                                   // glyph index
                cursor,                                                     // x offset
                line * (m_lineSpacing + 32.0f),                             // y offset  --- TODO: get height from font
                static_cast<float>(glyph.rightBearing - glyph.leftBearing)  // advance
            };
            cursor += shapedGlyph.advance + m_letterSpacing;
            result.push_back(std::move(shapedGlyph));
        }
        return result;
    }
private:
    HersheyFont m_font;

    float m_letterSpacing = 8.0f;
    float m_lineSpacing = 5.0f;
    float m_scale = 1.0f;
    float m_angle = 0.0f;
};
