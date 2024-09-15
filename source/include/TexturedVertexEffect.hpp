#ifndef TEXTURED_VERTEX_EFFECT_H_
#define TEXTURED_VERTEX_EFFECT_H_

#include "Texture2D.hpp"

class TexturedVertexEffect
{
public:
    struct Vertex
    {
        float x, y, z, u, v;

        Vertex operator+(const Vertex &_rhs) const
        {
            return {x + _rhs.x, y + _rhs.y, z + _rhs.z, u + _rhs.u, v + _rhs.v};
        }
        Vertex operator*(float rhs) const
        {
            return {x * rhs, y * rhs, z * rhs, u * rhs, v * rhs};
        }
    };
    struct FragmentShader
    {
        uint32_t get_color(const Vertex &input)
        {
            // Sample the texture using the interpolated texture coordinates
            int textureX = static_cast<int>(input.u * texture_->get_width()) % texture_->get_width();
            int textureY = static_cast<int>(input.v * texture_->get_height()) % texture_->get_height();
            uint32_t textureColor = texture_->get_pixels()[textureY * texture_->get_width() + textureX];
            return textureColor;
        }
        std::shared_ptr<Texture2D> texture_ = nullptr;
    };
    Vertex vertex_;
    FragmentShader fragment_shader_;
};

#endif