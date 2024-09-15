#ifndef NORMALED_VERTEX_EFFECT_H_
#define NORMALED_VERTEX_EFFECT_H_

#include <random>
#include "Texture2D.hpp"

class NormaledVertexEffect
{
public:
    struct Vertex
    {
        float x, y, z, u, v, nx, ny, nz = 0.0f;
        Vertex operator+(const Vertex &_rhs) const
        {
            return {x + _rhs.x, y + _rhs.y, z + _rhs.z, u + _rhs.u, v + _rhs.v, nx + _rhs.nx, ny + _rhs.ny, nz + _rhs.nz};
        }
        Vertex operator+(float _rhs) const
        {
            return {x + _rhs, y + _rhs, z + _rhs, u + _rhs, v + _rhs, nx + _rhs, ny + _rhs, nz + _rhs};
        }
        void operator+=(const Vertex &_rhs)
        {
            x += _rhs.x;
            y += _rhs.y;
            z += _rhs.z;
            u += _rhs.u;
            v += _rhs.v;
            nx += _rhs.nx;
            ny += _rhs.ny;
            nz += _rhs.nz;
        }
        void operator+=(float _rhs)
        {
            x += _rhs;
            y += _rhs;
            z += _rhs;
            u += _rhs;
            v += _rhs;
            nx += _rhs;
            ny += _rhs;
            nz += _rhs;
        }

        Vertex operator-(const Vertex &_rhs) const
        {
            return {x - _rhs.x, y - _rhs.y, z - _rhs.z, u - _rhs.u, v - _rhs.v, nx - _rhs.nx, ny - _rhs.ny, nz - _rhs.nz};
        }

        Vertex operator-(float _rhs) const
        {
            return {x - _rhs, y - _rhs, z - _rhs, u - _rhs, v - _rhs, nx - _rhs, ny - _rhs, nz - _rhs};
        }

        void operator-=(const Vertex &_rhs)
        {
            x -= _rhs.x;
            y -= _rhs.y;
            z -= _rhs.z;
            u -= _rhs.u;
            v -= _rhs.v;
            nx -= _rhs.nx;
            ny -= _rhs.ny;
            nz -= _rhs.nz;
        }

        void operator-=(float _rhs)
        {
            x -= _rhs;
            y -= _rhs;
            z -= _rhs;
            u -= _rhs;
            v -= _rhs;
            nx -= _rhs;
            ny -= _rhs;
            nz -= _rhs;
        }

        Vertex operator*(float _rhs) const
        {
            return {x * _rhs, y * _rhs, z * _rhs, u * _rhs, v * _rhs, nx * _rhs, ny * _rhs, nz * _rhs};
        }

        Vertex operator*(const Vertex &_rhs) const
        {
            return {x * _rhs.x, y * _rhs.y, z * _rhs.z, u * _rhs.u, v * _rhs.v, nx * _rhs.nx, ny * _rhs.ny, nz * _rhs.nz};
        }

        Vertex operator/(float _rhs) const
        {
            return {x / _rhs, y / _rhs, z / _rhs, u / _rhs, v / _rhs, nx / _rhs, ny / _rhs, nz / _rhs};
        }

        Vertex operator/(const Vertex &_rhs) const
        {
            return {x / _rhs.x, y / _rhs.y, z / _rhs.z, u / _rhs.u, v / _rhs.v, nx / _rhs.nx, ny / _rhs.ny, nz / _rhs.nz};
        }
    };

    struct FragmentShader
    {
        uint32_t get_color(const Vertex &_input)
        {
            // Sample the texture using the interpolated texture coordinates
            int textureX = static_cast<int>(_input.u * texture_->get_width()- 1) % texture_->get_width();
            int textureY = static_cast<int>(_input.v * texture_->get_height() - 1) % texture_->get_height();
            uint32_t textureColor = texture_->get_pixels()[textureY * texture_->get_width() + textureX];
            return 0xFFFFFFFFFF;
        }
        std::shared_ptr<Texture2D> texture_ = nullptr;
    };
    FragmentShader fragmet_shader_;
};

#endif