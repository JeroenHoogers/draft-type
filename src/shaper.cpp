#include <draft-type/shaper.h>

namespace DraftType
{
 	std::vector<ShapedGlyph> Shaper::layout(const std::string& text, float xoffset, float yoffset) const
    {
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
                static_cast<uint16_t>(c),					// glyph index
                cursor,			                       		// x offset
                yoffset + line * (lineSpacing + 32.0f),		// y offset  --- TODO: get height from font
                static_cast<float>(glyph.advance)  			// advance
            };
            cursor += shapedGlyph.advance + letterSpacing;
            result.push_back(std::move(shapedGlyph));
        }
        return result;
    }
}
