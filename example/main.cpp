#include <draft-type.h>
#include <string>
#include <fstream>

const int width = 500;
const int height = 350;


struct Color {
    unsigned char r, g, b;
};

void setPixel(std::vector<Color>& img, int x, int y, Color c) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    img[y * width + x] = c;
}

// Bresenham's algorithm
void drawLine(std::vector<Color>& img, int x0, int y0, int x1, int y1, Color c) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(img, x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

void drawGlyph(std::vector<Color>& img, const DraftType::HersheyFont& font, const DraftType::ShapedGlyph& glyph)
{
    const auto& geo = font.chr(glyph.glyphIndex);

    for (const auto& path : geo.paths)
    {
        for (int i = path.offset; i < path.offset + path.size - 1; i++) {
        const auto v0 = geo.points[i];
            const auto v1 = geo.points[i+1];

            int x0 = v0.x + glyph.xOffset;
            int y0 = v0.y + glyph.yOffset;
            int x1 = v1.x + glyph.xOffset;
            int y1 = v1.y + glyph.yOffset;

            drawLine(img, x0, y0, x1, y1, {255, 255, 255});
        }
    }
}

int main()
{
    DraftType::HersheyFont font;
    std::string fontName = "futural";

    font.load("../assets/hershey-fonts/" + fontName +".jhf");

    std::string text =
        " !\"#$%&'()*\n"
        "+,-./012345\n"
        "6789:;<=>?@\n"
        "ABCDEFGHIJK\n"
        "LMNOPQRSTUV\n"
        "WXYZ[\\]^_`\n"
        "abcdefghijk\n"
        "lmnopqrstuv\n"
        "wxyz{|}~\x7F";

    std::vector<Color> img(width * height, {0, 0, 0});
    DraftType::Shaper shaper(font);
    shaper.letterSpacing = 5.0f;

    for (const auto& glyph : shaper.layout(text, 10.0f, 20.0f))
    {
        drawGlyph(img, font, glyph);
    }

    // write ppm
    std::ofstream out(fontName + ".ppm");
    out << "P6\n" << width << " " << height << "\n255\n";
    out.write((char*)img.data(), img.size() * 3);

    return 0;
}
