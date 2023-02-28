cmake_minimum_required(VERSION 3.9)

project(T_TApe VERSION 1.1.8)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files for ImGui
add_library(imgui STATIC
  imgui/imgui.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_impl_opengl3.cpp
  imgui/imgui_impl_glfw.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
)

# Set the include directories for ImGui
target_include_directories(imgui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(/usr/local/lib)

# Add the executable target
add_executable(T_TApe Main.cpp)

# Link the executable against the ImGui library and GLFW/OpenGL libraries
target_link_libraries(T_TApe
  imgui
  glfw3
  GL
  X11
  Xinerama
  Xi
  udev
)

# Set the include directories for the executable
target_include_directories(T_TApe PUBLIC
  /usr/local/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/Math
  ${CMAKE_CURRENT_SOURCE_DIR}/RenderEngine
  ${CMAKE_CURRENT_SOURCE_DIR}/Features
  ${CMAKE_CURRENT_SOURCE_DIR}/Game
  ${CMAKE_CURRENT_SOURCE_DIR}/Game/Enums
  ${CMAKE_CURRENT_SOURCE_DIR}/UI
)