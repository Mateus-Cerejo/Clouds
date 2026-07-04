# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.10)

project (Clouds)

set (CMAKE_CXX_STANDARD 23)


include_directories (
    ${CMAKE_SOURCE_DIR}/external/GLFW/include
    ${CMAKE_SOURCE_DIR}/external/GLAD/include
    ${CMAKE_SOURCE_DIR}/external/GLM/
    ${CMAKE_SOURCE_DIR}/external/stb
)

link_directories (
    ${CMAKE_SOURCE_DIR}/external/GLFW/lib-vc2022
)


add_executable (Clouds)

target_sources (Clouds PRIVATE src/main.cpp "src/Window/Window.cpp" "external/GLAD/src/glad.c" "src/InputManager/InputManager.cpp" "src/ShaderCreator/ShaderCreator.cpp" "src/Shader/Shader.cpp" "src/DefaultShader/DefaultShader.cpp" "src/Mesh/Mesh.cpp" "src/Camera/Camera.cpp")

target_link_libraries (Clouds
    glfw3
    opengl32
)

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

# Copy assets to /bin
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(Clouds copy_assets)