#ifndef MESH_H_
#define MESH_H_

#include <string>    
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <TexturedVertexEffect.hpp>

#define INDICES_PER_TRIANGLE 3

namespace Soft
{
    class Mesh
    {
    public:
        Mesh(const std::string path);
        ~Mesh()=default;
        uint32_t get_indices_count();
        const std::vector<TexturedVertexEffect::Vertex> &get_vertices();
        const std::vector<uint32_t> &get_indices();

    private:
        std::vector<TexturedVertexEffect::Vertex> vertices_;
        std::vector<uint32_t> indices_;
    };
}

#endif