#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include <filesystem>
#include <string>
#include <memory>
#include <vector>

class Texture2D
{
public:
    Texture2D(const std::string &_path, bool _flip = true);
    virtual ~Texture2D();
    void set_pixels(const std::string &_path, bool _flip = true);
    uint32_t* get_pixels();
    uint32_t get_width();
    uint32_t get_height();
    bool is_loaded();

private:
    uint32_t *pixels_;
    uint32_t width_;
    uint32_t height_;
    bool loaded_;
};

#endif