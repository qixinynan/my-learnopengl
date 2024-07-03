find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

include_directories(../deps/glad/include)
include_directories(${glfw_INCLUDE_DIRS})

add_executable(Learn01 ./main.cpp ./shader.cpp ../deps/glad/include/glad/glad.h ../deps/glad/src/glad.c)

target_link_libraries(Learn01 glfw spdlog::spdlog)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/01/shaders)

file(GLOB SHADERS shaders/basic.frag shaders/basic.vert)

# 创建一个自定义目标来处理 shader 文件的复制
add_custom_target(copy_shaders ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADERS} ${CMAKE_BINARY_DIR}/01/shaders/
  DEPENDS ${SHADERS}
)

# 确保 Learn01 在 copy_shaders 目标之后构建
add_dependencies(Learn01 copy_shaders)