# CMakeList.txt : CMake project for TestOpenGL, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TestOpenGL "Source Files/TestOpenGL.cpp" "Header Files/TestOpenGL.h"  "Header Files/Shader.h" "Header Files/Texture.h" "Header Files/Material.h" "Header Files/Vertex.h" "Header Files/Mesh.h" "Header Files/Primitives.h" "Source Files/Game.cpp" "Header Files/Game.h" "Header Files/libs.h" "Header Files/Camera.h" "Header Files/Model.h" "Header Files/OBJLoader.h" "Header Files/Light.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestOpenGL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(TestOpenGL
glfw
libglew_static
glm
soil2)