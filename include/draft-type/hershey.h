#pragma once
#include "glyph.h"
#include <array>
#include <string>
#include <vector>

namespace DraftType
{
	// Hershey font specification: https://paulbourke.net/dataformats/hershey/
	class HersheyFont
	{
	public:
		HersheyFont() = default;
		HersheyFont(const std::string& path) {
			load(path);
		}

		constexpr static const uint16_t BASELINE_OFFSET = 10;

		struct Glyph
		{
			uint16_t advance;
			uint16_t height;
			int16_t ymin;
			std::vector<Vert> points;
			std::vector<Path> paths;
		};

		void load(const std::string &path);
		const std::array<Glyph, 256> &data() const { return m_glyphs; };
		const Glyph chr(char c) const { return m_glyphs[(uint16_t)c]; };
		uint32_t height() const { return m_maxCharHeight; };
		uint16_t baseoffset() const { return BASELINE_OFFSET; };

	private:
		inline int16_t coord(char c) const { return c - 'R'; }

		Glyph parse(const std::string &line) const;

		std::array<Glyph, 256> m_glyphs;
		uint16_t m_size;
		uint16_t m_maxCharHeight;
	};
} // namespace DraftType
