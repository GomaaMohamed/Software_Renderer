############################################################################################
# Define the minimum required version of cmake
cmake_minimum_required(VERSION 3.16.3)

# Define the project
project(
    executable
    VERSION 1.0
    DESCRIPTION "This is a basic cpp project using cmake"
    HOMEPAGE_URL "https://github.com/GomaaMohamed"
    LANGUAGES CXX)

# Define standard and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

# Find gl lib
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

# Add the sources of unit testing
set(SRCS
    src/main.cpp
    src/GpuHelper.cpp
    src/Texture2D.cpp
    src/Mesh.cpp
    src/Input.cpp
    src/glad.cpp)

# Define an executable target
add_executable(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 )
target_link_options(${PROJECT_NAME} PRIVATE -g -O0)

# Add the include directries for a target
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE src
    PUBLIC include)

# Add the sources to the target
target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SRCS})

# Link with gl lib
target_link_libraries(${PROJECT_NAME} glfw glm assimp OpenGL::GL)
