#define STB_IMAGE_IMPLEMENTATION
#include "Texture2D.hpp"
#include "stb_image.h"
#include <stdexcept>

#define RGBA_TO_UNIT32(r, g, b, a) ((r) | (g << 8) | (b << 16) | (a << 24))

Texture2D::Texture2D(const std::string &_path, bool _flip):pixels_(nullptr),width_(0),height_(0),loaded_(false)
{
    // Set flip vertically
    stbi_set_flip_vertically_on_load(_flip);

    // Extract tecture parameters
    int w, h, bpp;
    uint8_t *img = stbi_load(_path.c_str(), &w, &h, &bpp, 3);
    int buffer_size = w * h * bpp;

    // Check if the texture was extracted or not
    if (!img)
        throw std::runtime_error("failed to load image!");

    // allocate an array to save the extracted pixls
    pixels_ = new uint32_t[w*h]; // new uint32_t[w * h];
    width_ = w;
    height_ = h;
    for (size_t i = 0; i < w * h * bpp / 3; i++)
    {
        int r = img[i * 3 + 0];
        int g = img[i * 3 + 1];
        int b = img[i * 3 + 2];

        pixels_[i] = RGBA_TO_UNIT32(r, g, b, 1);
    }

    // Free memory of the image
    stbi_image_free(img);
    loaded_ = true;
    stbi_set_flip_vertically_on_load(false);
}

Texture2D::~Texture2D()
{
    pixels_ = nullptr;
    width_ = 0;
    height_ = 0;
}

void Texture2D::set_pixels(const std::string &_path, bool _flip)
{
    // Check if there is another texture
    if (pixels_!= nullptr)
    {
        delete [] pixels_;
    }

    // Set flip vertically
    stbi_set_flip_vertically_on_load(_flip);

    // Extract tecture parameters
    int w, h, bpp;
    uint8_t *img = stbi_load(_path.c_str(), &w, &h, &bpp, 3);
    int buffer_size = w * h * bpp;

    // Check if the texture was extracted or not
    if (!img)
        throw std::runtime_error("failed to load image!");

    // allocate an array to save the extracted pixls
    pixels_ = new uint32_t[w * h]; // new uint32_t[w * h];
    width_ = w;
    height_ = h;
    for (size_t i = 0; i < w * h * bpp / 3; i++)
    {
        int r = img[i * 3 + 0];
        int g = img[i * 3 + 1];
        int b = img[i * 3 + 2];

        pixels_[i] = RGBA_TO_UNIT32(r, g, b, 1);
    }

    // Free memory of the image
    stbi_image_free(img);
    loaded_ = true;
    stbi_set_flip_vertically_on_load(false);
}
uint32_t* Texture2D::get_pixels()
{
    return pixels_;
}
uint32_t Texture2D::get_width()
{
    return width_;
}
uint32_t Texture2D::get_height()
{
    return height_;
}
bool Texture2D::is_loaded()
{
    return loaded_;
}