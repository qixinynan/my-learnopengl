find_package(SDL2 REQUIRED)
add_library(imgui STATIC
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_impl_opengl3.cpp
  imgui_impl_sdl2.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui.cpp
)
message("Hello World ${SDL2_INCLUDE_DIRS}")
include_directories(${SDL2_INCLUDE_DIRS})
target_include_directories(imgui PUBLIC .)