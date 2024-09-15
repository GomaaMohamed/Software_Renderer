#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>
#include "GpuHelper.hpp"
#include <filesystem>
#include <memory>
#include "Pipeline.cpp"
#include "Mesh.hpp"
#include "Texture2D.hpp"
#include "ColoredVertexEffect.hpp"
#include "TexturedVertexEffect.hpp"
#include "Input.hpp"

// Callback function to be called at window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Renderes holders
std::shared_ptr<Soft::Pipeline<TexturedVertexEffect>> tRenderer;
std::shared_ptr<Soft::Pipeline<ColoredVertexEffect>> cRenderer;

int main()
{
    // Initialize GLFW
    int glfwState = glfwInit();
    if (!glfwState)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Software Renderer", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set callbacks for window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GpuLayer we created to access framebuffer
    GpuLayer::Init(window);

    // Create renderes
    tRenderer = std::make_shared<Soft::Pipeline<TexturedVertexEffect>>();
    cRenderer = std::make_shared<Soft::Pipeline<ColoredVertexEffect>>();

    // Create models and textures
    Soft::Mesh mesh("./cube.fbx");
    std::shared_ptr<Texture2D> tex =std::make_shared<Texture2D>("./wood.jpg");
    // std::vector<TexturedVertexEffect::Vertex> vertices1 = mesh.get_vertices();
    // std::vector<uint32_t> indecis1 = mesh.get_indices();
    float side = 1.0f * 0.5f;
    std::vector<TexturedVertexEffect::Vertex> vertices1 = {
        {-side, -side, -side, 0.0f, 1.0f},
        {side, -side, -side, 1.0f, 1.0f},
        {-side, side, -side, 0.0f, 0.0f},
        {side, side, -side, 1.0f, 0.0f},
        {-side, -side, side, 1.0f, 1.0f},
        {side, -side, side, 0.0f, 1.0f},
        {-side, side, side, 1.0f, 0.0f},
        {side, side, side, 0.0f, 0.0f}};
    std::vector<ColoredVertexEffect::Vertex> vertices2 = {
        {-side, -side, -side, 0.5f, 1.0f, 0.3f}, 
        {side, -side, -side, 1.0f, 0.3f, 0.1f},
        {-side, side, -side, 0.0f, 0.5f, 0.7f},
        {side, side, -side, 1.0f, 0.5f, 0.5f},
        {-side, -side, side, 1.0f, 0.0f, 0.3f},
        {side, -side, side, 0.0f, 1.0f, 1.0f},
        {-side, side, side, 0.3f, 0.0f, 0.8f},
        {side, side, side, 0.0f, 1.0f, 0.3f}};

    std::vector<uint32_t> indecis =
        {
            0, 2, 1, 2, 3, 1,
            1, 3, 5, 3, 7, 5,
            2, 6, 3, 3, 6, 7,
            4, 5, 7, 4, 7, 6,
            0, 4, 2, 2, 4, 6,
            0, 1, 4, 1, 5, 4};

    // Inputs
    float thetaX = 0.0f;
    float thetaY = 0.0f;
    float thetaZ = 0.0f;
    glm::vec3 Pos = {0.0f, 0.0f, -3.0f};
    Soft::Input::set_working_window(window);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        cRenderer->clear_screen(RGBA_COLOR(0, 0, 0, 0));

        // Handle keyboard input
        if (Soft::Input::key_pressed(Soft::Key::Q))
            thetaX += 0.1f;
        if (Soft::Input::key_pressed(Soft::Key::W))
            thetaY += 0.1f;
        if (Soft::Input::key_pressed(Soft::Key::E))
            thetaZ += 0.1f;

        if (Soft::Input::key_pressed(Soft::Key::Up))
            Pos.z += 0.1f;
        if (Soft::Input::key_pressed(Soft::Key::Down))
            Pos.z -= 0.1f;
        if (Soft::Input::key_pressed(Soft::Key::Right))
            Pos.x += 0.1f;
        if (Soft::Input::key_pressed(Soft::Key::Left))
            Pos.x -= 0.1f;

        // Update and draw cube
        tRenderer->bind_position(Pos);
        tRenderer->bind_scale(glm::vec3(0.2f, 0.2f, 0.2f));
        tRenderer->bind_rotation(thetaX,thetaY,thetaZ);
        tRenderer->bind_texture(tex);

        cRenderer->bind_position(glm::vec3(0.7f, 0.0f, -3.0f));
        cRenderer->bind_scale(glm::vec3(1.0f, 1.0f, 1.0f));
        cRenderer->bind_rotation(thetaX, thetaY, thetaZ);

        tRenderer->draw_elements(vertices1, indecis);
        cRenderer->draw_elements(vertices2, indecis);

        tRenderer->present();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cRenderer->destroy();
    // Terminate GLFW
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    if (cRenderer)
    {
        cRenderer->set_view_port(0, 0, width, height);
    }
}

//   std::vector<TexturedVertexEffect::Vertex> vertices = {
//   {-side, -side, -side, 0.0f, 1.0f},
//   {side, -side, -side,  1.0f, 1.0f},
//   {-side, side, -side,  0.0f, 0.0f},
//   {side, side, -side,   1.0f, 0.0f},
//   {-side, -side, side,  1.0f, 1.0f},
//   {side, -side, side,   0.0f, 1.0f},
//   {-side, side, side,   1.0f, 0.0f},
//   {side, side, side,    0.0f, 0.0f}};
