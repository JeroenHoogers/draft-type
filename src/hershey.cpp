#include "hershey.h"
#include <optional>

void HersheyFont::load(const std::string& input_path) {
    std::ifstream file(input_path);

    std::string line;
    if (file.is_open()) {
        m_size = 32;
        while (std::getline(file, line)) {
            Glyph glyph = parse(line);
            m_glyphs[m_size++] = glyph;
        }
        file.close();
    } else {
        printf("Unable to open file!\n");
    }
}

HersheyFont::Glyph HersheyFont::parse(const std::string& line) const {
    u_int16_t numVerts = std::stoi(line.substr(5, 3));
#if DEBUG
    u_int16_t id = std::stoi(line.substr(0, 5));
    printf("parsing glyph [%i], verts: %i --- '%s'\n", id, numVerts, line.c_str());
#endif
    int16_t left = coord(line[8]);
    int16_t right = coord(line[9]);

    if(left > right) {
        // TODO: throw error
        printf("Invalid left position %d > right pos %d!", left, right);
    }

    std::vector<Vert> verts;
    std::vector<Segment> indices;
    verts.reserve(numVerts);
    indices.reserve(numVerts);

    // construct glyph
    int pv = -1;

    uint16_t count = 0;
    numVerts--;
    for (u_int16_t i = 10; i < line.size(); i++) {
        if (line[i] == ' ' && line[i + 1] == 'R') {
            pv = -1;
            i++;
            continue;
        }

        int16_t x = coord(line[i]);
        int16_t y = coord(line[++i]);
        count++;
        if(pv >= 0)
        {
            // create segment
            indices.push_back(Segment {
                static_cast<uint16_t>(pv),              // i0
                static_cast<uint16_t>(verts.size())     // i1
            });
        }
        pv = verts.size();
        verts.push_back(Vert{ x, y });
    }

    return Glyph {
        left,               // left-bearing
        right,              // right-bearing
        std::move(verts),   // points
        std::move(indices)  // indices
    };
}
