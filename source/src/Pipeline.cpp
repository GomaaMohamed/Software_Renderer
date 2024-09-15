#include "Pipeline.hpp"
#include <limits>
namespace Soft
{
    template <typename Effect>
    Pipeline<Effect>::Pipeline(uint32_t _width, uint32_t _height)
    {
        frame_buffer_ = std::make_shared<FrameBuffer>(_width, _height);
        pixels_ = new uint32_t[_width * _height];
        z_buffer_ = new float[_width * _height];
        view_port_width_ = _width;
        view_port_height_ = _height;
    }

    template <typename Effect>
    Pipeline<Effect>::~Pipeline()
    {
        if (pixels_)
        {
            delete[] pixels_;
            pixels_ = nullptr;
        }
        if(z_buffer_)
        {
            delete[] z_buffer_;
            z_buffer_ = nullptr;
        }
    }

    template <typename Effect>
    void Pipeline<Effect>::destroy()
    {
        if (pixels_)
        {
            delete[] pixels_;
            pixels_ = nullptr;
        }
        if (z_buffer_)
        {
            delete[] z_buffer_;
            z_buffer_ = nullptr;
        }
        frame_buffer_.reset();
    }

    template <typename Effect>
    void Pipeline<Effect>::set_view_port(int _x, int _y, uint32_t _width, uint32_t _height)
    {
        frame_buffer_->Resize(_width, _height);
        if (pixels_ != nullptr)
        {
            delete[] pixels_;
            pixels_ = nullptr;
        }
        if (z_buffer_ != nullptr)
        {
            delete[] z_buffer_;
            z_buffer_ = nullptr;
        }
        pixels_ = new uint32_t[_width * _height];
        z_buffer_ = new float[_width * _height];
        view_port_x_ = _x;
        view_port_y_ = _y;
        view_port_width_ = _width;
        view_port_height_ = _height;
    }

    template <typename Effect>
    void Pipeline<Effect>::clear_screen(uint32_t _color)
    {
        // clear screen
        if (pixels_)
        {
            for (size_t i = 0; i < view_port_width_ * view_port_height_; i++)
                pixels_[i] = _color;
        }
        if (z_buffer_)
        {
            for (size_t i = 0; i < view_port_width_ * view_port_height_; i++)
                z_buffer_[i] = std::numeric_limits<float>::infinity();
        }
        
    }

    template <typename Effect>
    void Pipeline<Effect>::bind_position(glm::vec3 _translation_vector)
    {
        translation_mat_ = glm::translate(glm::mat4(1.0f), _translation_vector);
    }

    template <typename Effect>
    void Pipeline<Effect>::bind_scale(glm::vec3 _scale_vector)
    {
        scale_mat_ = glm::scale(glm::mat4(1.0f), _scale_vector);
    }

    template <typename Effect>
    void Pipeline<Effect>::bind_rotation(float _angle_x, float _angle_y, float _angle_z)
    {
        rotation_mat_ = glm::rotate(glm::mat4(1.0f), _angle_x, glm::vec3(1.0f,0.0f,0.0f));
        rotation_mat_ *= glm::rotate(glm::mat4(1.0f), _angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation_mat_ *= glm::rotate(glm::mat4(1.0f), _angle_z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    template <typename Effect>
    void Pipeline<Effect>::bind_texture(std::shared_ptr<Texture2D> _texture)
    {
        effect_.fragment_shader_.texture_ = _texture;
    }

    template <typename Effect>
    void Pipeline<Effect>::draw_elements(const std::vector<Vertex> &_vertices, const std::vector<uint32_t> _indices)
    {
        const std::vector<Vertex> transformedVertices = process_vertices(_vertices);
        assemble_triangles(transformedVertices, _indices);
    }

    template <typename Effect>
    std::vector<typename Pipeline<Effect>::Vertex> Pipeline<Effect>::process_vertices(const std::vector<Vertex> &_vertices)
    {
        std::vector<Vertex> tempVertices = _vertices;
        for (auto &v : tempVertices)
        {
            glm::vec3 vertex = {v.x, v.y, v.z};
            // Get the transformation and projection matrices
            model_mat_ = translation_mat_ * scale_mat_ * rotation_mat_;
            projection_mat_ = glm::perspective(glm::radians(45.0f), (float)view_port_width_ / view_port_height_, 0.01f, 1000.0f);

            // Construct homogeneous vector
            glm::vec4 A_transformed = {vertex.x, vertex.y, vertex.z, 1.0f};

            // Apply transformations
            A_transformed = projection_mat_ * model_mat_ * A_transformed;

            v.x = A_transformed.x;
            v.y = A_transformed.y;
            v.z = A_transformed.z;
        }
        return tempVertices;
    }

    template <typename Effect>
    void Pipeline<Effect>::assemble_triangles(const std::vector<Vertex> &_transformed_vertices, const std::vector<uint32_t> &_indices)
    {
        for (int i = 0; i < _indices.size() / 3; i++)
        {
            int index1 = _indices[i * 3 + 0];
            int index2 = _indices[i * 3 + 1];
            int index3 = _indices[i * 3 + 2];

            auto vertex1 = _transformed_vertices[index1];
            auto vertex2 = _transformed_vertices[index2];
            auto vertex3 = _transformed_vertices[index3];
            generate_triangle(vertex1, vertex2, vertex3);
        }
    }

    template <typename Effect>
    void Pipeline<Effect>::generate_triangle(Vertex &_vertex1, Vertex &_vertex2, Vertex &_vertex3)
    {
        // TODO:: make operations on triangle vertices before rendering them (vs, gs)
        post_process_triangle_vertices(_vertex1, _vertex2, _vertex3);
    }

    template <typename Effect>
    void Pipeline<Effect>::post_process_triangle_vertices(Vertex &_vertex1, Vertex &_vertex2, Vertex &_vertex3)
    {
        // Convert to screen space
        convert_NDC_to_SC(_vertex1);
        convert_NDC_to_SC(_vertex2);
        convert_NDC_to_SC(_vertex3);
        // Draw triangle
        draw_triangle(_vertex1, _vertex2, _vertex3, 1);
    }

    template <typename Effect>
    void Pipeline<Effect>::convert_NDC_to_SC(Vertex &_vertex)
    {
        // const float zInv = 1.0f / _vertex.z;
        // _vertex.x = (_vertex.x * zInv + 1.0f) * 800 * 0.5f;
        // _vertex.y = (-_vertex.y * zInv + 1.0f) * 600 * 0.5f;
        const float zInv = 1.0f / _vertex.z;
        _vertex = _vertex * zInv;
        _vertex.x = (_vertex.x + 1.0f) * view_port_width_ * 0.5f;
        _vertex.y = (-_vertex.y + 1.0f) * view_port_height_ * 0.5f;
        _vertex.z = zInv;
    }

    template <typename Effect>
    void Pipeline<Effect>::draw_triangle(Vertex _vertex_a, Vertex _vertex_b, Vertex _vertex_c, uint32_t _color)
    {
        // Calculate the edge function for the whole triangle (ABC)
        int ABC = edge_function(_vertex_a, _vertex_b, _vertex_c);

        if (ABC > 0.0f) // back face culling
            return;

        // Initialise our Vertex
        Vertex P;

        // Get the bounding box of the triangle
        int minX = std::min(std::min(_vertex_a.x, _vertex_b.x), _vertex_c.x) - 1;
        int minY = std::min(std::min(_vertex_a.y, _vertex_b.y), _vertex_c.y) - 1;
        int maxX = std::max(std::max(_vertex_a.x, _vertex_b.x), _vertex_c.x) + 1;
        int maxY = std::max(std::max(_vertex_a.y, _vertex_b.y), _vertex_c.y) + 1;

        // Loop through all the pixels of the bounding box
        for (P.y = minY; P.y < maxY; P.y++)
        {
            for (P.x = minX; P.x < maxX; P.x++)
            {
                // Calculate our edge functions
                int ABP = edge_function(_vertex_a, _vertex_b, P);
                int BCP = edge_function(_vertex_b, _vertex_c, P);
                int CAP = edge_function(_vertex_c, _vertex_a, P);

                // Normalise the edge functions by dividing by the total area to get the barycentric coordinates
                float weightA = (float)BCP / ABC;
                float weightB = (float)CAP / ABC;
                float weightC = (float)ABP / ABC;

                // If all the edge functions are positive, the Vertex is inside the triangle
                if (weightA >= 0 && weightB >= 0 && weightC >= 0)
                {
                    Vertex InterpolatedVertex = _vertex_a * weightA + _vertex_b * weightB + _vertex_c * weightC;

                    const float z = 1 / InterpolatedVertex.z;
                    InterpolatedVertex = InterpolatedVertex * z;
                    // Depth test
                    if (P.x < view_port_width_ && P.x >= 0 && P.y < view_port_height_ && P.y >= 0)
                    {
                        int index = (int)P.y * view_port_width_ + (int)P.x;
                        if (index >= 0 && index < view_port_width_ * view_port_height_)
                        {
                            if (z < z_buffer_[index])
                            {
                                z_buffer_[index] = z;
                                pixels_[index] = effect_.fragment_shader_.get_color(InterpolatedVertex);
                            }
                        }
                    }
                }
            }
        }
    }

    template <typename Effect>
    void Pipeline<Effect>::present()
    {
        if (frame_buffer_)
        {
            frame_buffer_->Present(pixels_);
        }
    }

    template <typename Effect>
    int Pipeline<Effect>::edge_function(Vertex _vertex_a, Vertex _vertex_b, Vertex _vertex_c)
    {
        return (_vertex_b.x - _vertex_a.x) * (_vertex_c.y - _vertex_a.y) - (_vertex_b.y - _vertex_a.y) * (_vertex_c.x - _vertex_a.x);
    }
}