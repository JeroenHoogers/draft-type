#include <draft-type.h>
#include <fstream>
#include <string>

static int width = 500;
static int height = 350;

struct Color
{
	unsigned char r, g, b;
};

void setPixel(std::vector<Color> &img, int x, int y, Color c) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	img[y * width + x] = c;
}

// Bresenham's algorithm
void drawLine(std::vector<Color> &img, int x0, int y0, int x1, int y1,
			  Color c) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;

	while (true) {
		setPixel(img, x0, y0, c);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void drawGlyph(std::vector<Color> &img, const DraftType::HersheyFont &font,
			   const DraftType::ShapedGlyph &glyph) {
	const auto &geo = font.chr(glyph.glyphIndex);

	for (const auto &path : geo.paths) {
		for (int i = path.offset; i < path.offset + path.size - 1; i++) {
			const auto v0 = geo.points[i];
			const auto v1 = geo.points[i + 1];

			int x0 = v0.x + glyph.xOffset;
			int y0 = v0.y + glyph.yOffset;
			int x1 = v1.x + glyph.xOffset;
			int y1 = v1.y + glyph.yOffset;

			drawLine(img, x0, y0, x1, y1, {255, 255, 255});
		}
	}
}

int main(int argc, char *argv[])
{
	using namespace DraftType;

	std::string fontName = "futural";
	if (argc == 2) {
		fontName = std::string(argv[1]);
	}

	printf("Loading font \"%s\"...\n", fontName.c_str());

	HersheyFont font;

	font.load("../assets/hershey-fonts/" + fontName + ".jhf");

	std::string text = " !\"#$%&'()*\n"
					   "+,-./012345\n"
					   "6789:;<=>?@\n"
					   "ABCDEFGHIJK\n"
					   "LMNOPQRSTUV\n"
					   "WXYZ[\\]^_`\n"
					   "abcdefghijk\n"
					   "lmnopqrstuv\n"
					   "wxyz{|}~\x7F";

	Shaper shaper(font);
	shaper.letterSpacing = 5.0f;
	shaper.lineSpacing = 5.0f;
	// shaper.lineSpaceMode = LineSpacingMode::Dynamic;
	shaper.hAlign = HorizontalAlign::Center;

	auto bounds = shaper.getBounds(text);

	width = (bounds.right - bounds.left);
	height = (bounds.bottom - bounds.top);

	float xPos = 10.0f;
	float yPos = 10.0f;
	if (shaper.hAlign == HorizontalAlign::Center) {
		xPos += width * 0.5;
	} else if (shaper.hAlign == HorizontalAlign::Right) {
		xPos += width;
	}

	constexpr int MARGIN = 20;
	width += MARGIN;
	height += MARGIN;

	std::vector<Color> img(width * height, {0, 0, 0});

	for (const auto &glyph : shaper.layout(text, xPos, yPos)) {
		const auto &geo = font.chr(glyph.glyphIndex);

		float cx = glyph.xOffset + glyph.advance * 0.5;
		float ty = glyph.yOffset + geo.ymin;
		float by = glyph.yOffset + geo.ymin + geo.height;

		float baseY = glyph.yOffset + font.baseoffset();
		// draw baseline
		drawLine(img, glyph.xOffset, baseY, glyph.xOffset + glyph.advance, baseY, {128, 128, 128});

		// draw height
		drawLine(img, cx, ty, cx, by, {128, 128, 128});

		drawGlyph(img, font, glyph);
	}

	// write ppm
	std::ofstream out(fontName + ".ppm");
	out << "P6\n" << width << " " << height << "\n255\n";
	out.write((char *)img.data(), img.size() * 3);

	printf("Saved %s.ppm\n", fontName.c_str());
	return 0;
}
