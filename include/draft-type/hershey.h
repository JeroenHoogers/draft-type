#pragma once
#include <string>
#include <vector>
#include <array>
#include "glyph.h"

namespace DraftType
{
    // Hershey font specification: https://paulbourke.net/dataformats/hershey/
    class HersheyFont
    {
    public:
        struct Glyph {
            uint16_t advance;
            std::vector<Vert> points;
            std::vector<Path> paths;
        };

        void load(const std::string& input_path);
        const std::array<Glyph, 256>& data() const { return m_glyphs; };
        const Glyph chr(char c) const { return m_glyphs[(uint16_t)c]; };
        uint16_t size() const { return m_size; };

    private:
        inline int16_t coord(char c) const {
            return c - 'R';
        }

        Glyph parse(const std::string& line) const;

        std::array<Glyph, 256> m_glyphs;
        uint16_t m_size;
    };
}
