#ifndef COLORED_VERTEX_EFFECT_H_
#define COLORED_VERTEX_EFFECT_H_

#include <random>

class ColoredVertexEffect
{
public:
    struct Vertex
    {
        float x, y, z, r, g, b;
        Vertex operator+(const Vertex &_rhs) const
        {
            return {x + _rhs.x, y + _rhs.y, z + _rhs.z, r + _rhs.r, g + _rhs.g, b + _rhs.b};
        }
        Vertex operator*(float _rhs) const
        {
            return {x * _rhs, y * _rhs, z * _rhs, r * _rhs, g * _rhs, b * _rhs};
        }
    };
    struct FragmentShader
    {
        uint32_t get_color(const Vertex &input)
        {
            int r = input.r * 255;
            int g = input.g * 255;
            int b = input.b * 255;
            return ((r) | (g << 8) | (b << 16) | (1 << 24));
        }
    };
    Vertex vertex_;
    FragmentShader fragment_shader_;
};

#endif