#include "Frame.hpp"
#include "Logger.hpp"
#include "Map.hpp"
#include "StorageManager.hpp"
#include <fstream>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

template <>
Frame StorageManager::load(const std::filesystem::path &path)
try {
    Vector<uint32_t> imageSize;
    png_uint_32 color_type;
    png_uint_32 bit_depth;
    Frame frame;
    png_bytep *row_pointer = nullptr;

    FILE *fp = fopen(path.c_str(), "rb");
    if (!fp) throw std::runtime_error("can't open file");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) throw std::runtime_error("error preparing loading png");

    png_infop info = png_create_info_struct(png);
    if (!info) throw std::runtime_error("error preparing loading png");

    if (setjmp(png_jmpbuf(png))) throw std::runtime_error("error preparing loading png");
    png_init_io(png, fp);
    png_read_info(png, info);

    imageSize.x = png_get_image_width(png, info);
    imageSize.y = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointer = (png_bytep *)malloc(sizeof(png_bytep) * imageSize.y);
    for (unsigned y = 0; y < imageSize.y; y++)
        row_pointer[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    png_read_image(png, row_pointer);

    frame.create(imageSize);
    for (unsigned y = 0; y < imageSize.y; y++) {
        png_bytep row = row_pointer[y];
        for (unsigned x = 0; x < imageSize.x; x++) {
            png_bytep px = &(row[x * 4]);
            frame.setPixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
        }
        free(row);
    }
    free(row_pointer);
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    return frame;
} catch (const std::runtime_error &re) {
    logger->err("LOADER_PNG") << re.what() << ", using panic texture";

    Frame panic;
    panic.create({64, 64}, Color::Red);
    return panic;
}

template <>
Map StorageManager::load(const std::filesystem::path &path)
try {
    Map cur;
    std::ifstream file(path);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (!file.is_open()) { throw std::runtime_error("Can't open file"); }
    cur.map = std::string(std::istreambuf_iterator<char>(file), {});
    file.close();

    cur.size.x = cur.map.find_first_of('\n');
    if (cur.map.size() < cur.size.x) { throw std::runtime_error("Bad format"); };

    cur.map.erase(std::remove(cur.map.begin(), cur.map.end(), '\n'), cur.map.end());

    if (cur.map.size() % cur.size.x != 0) { throw std::runtime_error("Not a Cube"); }
    cur.size.y = cur.map.size() / cur.size.x;
    return cur;
} catch (const std::runtime_error &re) {
    logger->err("LOAD_MAP") << re.what() << ", using empty map";

    return Map();
}
