#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ColoredVertexEffect.hpp"
#include "Texture2D.hpp"
#include "GpuHelper.hpp"
#include "PipelineBase.cpp"

#define RGBA_COLOR(r, g, b, a) ((r) | (g << 8) | (b << 16) | (a << 24))

namespace Soft
{
    template <typename Effect>
    class Pipeline : public PipelineBase
    {
        typedef typename Effect::Vertex Vertex;
    public:
        Pipeline(uint32_t _width = 800, uint32_t _height = 600);
        virtual ~Pipeline();
        void destroy();
        void set_view_port(int _x, int _y, uint32_t _width, uint32_t _height);
        void clear_screen(uint32_t _color);
        void bind_position(glm::vec3 _translation_vector);
        void bind_scale(glm::vec3 _scale_vector);
        void bind_rotation(float _angle_x, float _angle_y, float _angle_z);
        void bind_texture(std::shared_ptr<Texture2D> _texture);
        void draw_elements(const std::vector<Vertex> &_vertices, const std::vector<uint32_t> _indices);
        void present();

    private:
        // Shared attributes for all pipelines
        // static std::shared_ptr<FrameBuffer> frame_buffer_;
        // static uint32_t *pixels_;
        // static float *z_buffer_;
        // static int view_port_x_, view_port_y_;
        // static uint32_t view_port_width_, view_port_height_;
        // Standalone attributes for each pipeline
        glm::mat4 translation_mat_;
        glm::mat4 scale_mat_;
        glm::mat4 rotation_mat_;
        glm::mat4 model_mat_;
        glm::mat4 projection_mat_;
        Effect effect_;
        // Local methods
        std::vector<Vertex> process_vertices(const std::vector<Vertex> &_vertices);
        void assemble_triangles(const std::vector<Vertex> &_transformed_vertices, const std::vector<uint32_t> &_indices);
        void generate_triangle(Vertex &_vertex1, Vertex &_vertex2, Vertex &_vertex3);
        void post_process_triangle_vertices(Vertex &_vertex1, Vertex &_vertex2, Vertex &_vertex3);
        void convert_NDC_to_SC(Vertex &_vertex);
        void draw_triangle(Vertex _vertex_a, Vertex _vertex_b, Vertex _vertex_c, uint32_t _color);
        int edge_function(Vertex _vertex_a, Vertex _vertex_b, Vertex _vertex_c);
    };
} 

#endif