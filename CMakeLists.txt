cmake_minimum_required(VERSION 3.9)

project(Apex VERSION 1.1.8)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the build type to Release if not provided
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# If you want to disable debug symbols explicitly for the Release build, you can do:
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

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
add_executable(Apex main.cpp)

# Link the executable against the ImGui library and GLFW/OpenGL libraries
target_link_libraries(Apex
        imgui
        glfw3
        GL
        X11
        Xinerama
        Xi
        udev
        SOIL
        X11
        Xtst
)

# Set the include directories for the executable
target_include_directories(Apex PUBLIC
        /usr/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Features
        ${CMAKE_CURRENT_SOURCE_DIR}/Drawings
        ${CMAKE_CURRENT_SOURCE_DIR}/Misc
        ${CMAKE_CURRENT_SOURCE_DIR}/SDK
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings
        ${CMAKE_CURRENT_SOURCE_DIR}/Math
)

file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Configs)