#pragma once
#include "glyph.h"
#include <array>
#include <string>
#include <vector>

namespace DraftType
{
	// Used for GPU rendering
	struct GlyphInfo {
	    uint32_t indexOffset;  	// first index into the indexbuffer
	    uint32_t indexCount;   	// number of indices used by this glyph
	};

	struct GPUFont
	{
	    std::vector<Vert> vertices;
	    // flat index buffer for lines, defines line list: every 2 indices = 1 segment             Vulkan equivalent: VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	    // TODO: add support for line list with adjacency, recommended for thickness > 1 (4 indices per segment) Vulkan equivalent: VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY
	    std::vector<uint32_t> indices;
	    std::vector<GlyphInfo> glyphs;
	};

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
		const std::array<Glyph, 128> &data() const { return m_glyphs; };
		const Glyph chr(char c) const { return m_glyphs[(uint16_t)c]; };
		uint32_t height() const { return m_maxCharHeight; };
		uint16_t baseoffset() const { return BASELINE_OFFSET; };
		GPUFont generateGPUFont() const; // TODO: add topology mode parameter

	private:
		inline int16_t coord(char c) const { return c - 'R'; }

		Glyph parse(const std::string &line) const;

		std::array<Glyph, 128> m_glyphs;
		uint16_t m_size;
		uint16_t m_maxCharHeight;
	};
} // namespace DraftType
