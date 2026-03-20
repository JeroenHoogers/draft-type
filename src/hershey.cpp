#include <cstdint>
#include <draft-type/hershey.h>
#include <fstream>

namespace DraftType
{
	void HersheyFont::load(const std::string &input_path) {
		std::ifstream file(input_path);

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
			printf("Unable to open file!\n");
		}
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
			// TODO: throw error
			printf("Invalid left position %d > right pos %d!", left, right);
			return {};
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
