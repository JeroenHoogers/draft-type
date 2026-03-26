#include <cstdint>
#include <draft-type/hershey.h>
#include <fstream>
#include <stdexcept>

namespace DraftType
{
	void HersheyFont::load(const std::string &path) {
		std::ifstream file(path);

		std::string line;
		m_maxCharHeight = 0;
		if (file.is_open()) {
			m_size = 32;
			while (std::getline(file, line)) {
#if DEBUG
				printf("parsing '%c'\n", (char)m_size);
#endif
				Glyph glyph = parse(line);
				m_glyphs[m_size++] = glyph;
				m_maxCharHeight = std::max(m_maxCharHeight, glyph.height);
			}
			file.close();
		} else {
			throw std::runtime_error("Font file not found: \"" + path + "\"");
		}
	}

	// Generates font buffers for GPU rendering
	GPUFont HersheyFont::generateGPUFont() const {
		GPUFont gpuBuffers {};

		uint32_t vertexOffset = 0;
		uint32_t indexOffset = 0;
		for (const Glyph& glyph : m_glyphs)
		{
			// append vertices
			gpuBuffers.vertices.insert(gpuBuffers.vertices.end(), glyph.points.begin(), glyph.points.end());

			uint32_t glyphIndexOffset = indexOffset;

			// create index buffer for Line List topology
			for(const Path& path : glyph.paths) {
				if (path.size <= 0) {
					continue;
				}
				for (int i = 0; i < static_cast<int>(path.size) - 1; i++) {
					gpuBuffers.indices.push_back(vertexOffset + i + path.offset);
					gpuBuffers.indices.push_back(vertexOffset + i + path.offset + 1);
				}
				indexOffset += (static_cast<int>(path.size) - 1) * 2;
			}

			// TODO: create index buffer for Line List w\ Adjacency topology

			gpuBuffers.glyphs.push_back(
				GlyphInfo {
					glyphIndexOffset,										// indexOffset
					static_cast<uint32_t>(indexOffset - glyphIndexOffset)	// indexCount
				}
			);

			vertexOffset += glyph.points.size();
		}

		return gpuBuffers;
	}

	HersheyFont::Glyph HersheyFont::parse(const std::string &line) const {
		u_int16_t numVerts = std::stoi(line.substr(5, 3));
		int16_t left = coord(line[8]);
		int16_t right = coord(line[9]);
		// u_int16_t id = std::stoi(line.substr(0, 5));
#if DEBUG
		printf("parsing glyph [%i], advance: %i verts: %i --- '%s'\n", id,
			   right - left, numVerts, line.c_str());
#endif

		if (left > right) {
			throw std::runtime_error("Invalid left position " + std::to_string(left) + " > right pos " + std::to_string(right) + "!");
		}

		std::vector<Vert> verts;
		std::vector<Path> paths;
		verts.reserve(--numVerts);

		// construct glyph
		uint16_t j = 0;
		std::string pathsStr = line.substr(10);
		int16_t top = INT16_MIN;
		int16_t bottom = INT16_MAX;

		for (u_int16_t i = 0; i < numVerts; i++) {
			if (pathsStr[i * 2] == ' ' && pathsStr[i * 2 + 1] == 'R') {
				paths.push_back(Path{
					j,										// offset
					static_cast<uint16_t>(verts.size() - j) // size
				});
				j = verts.size();
				continue;
			}

			int16_t x = -left + coord(pathsStr[i * 2]);
			int16_t y = coord(pathsStr[i * 2 + 1]);

			top = std::max(top, y);
			bottom = std::min(bottom, y);

			verts.push_back(Vert{x, y});
		}

		paths.push_back(Path {
			j,										// offset
			static_cast<uint16_t>(verts.size() - j) // size
		});

		return Glyph {
			static_cast<uint16_t>(right - left), // advance
			static_cast<uint16_t>(top - bottom), // height
			bottom,								 // ymin
			std::move(verts),					 // points
			std::move(paths)					 // paths
		};
	}
} // namespace DraftType
