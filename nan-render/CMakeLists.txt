set(TARGET NanRender)
set(NAN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/nan-render)
set(NAN_BINARY_DIR ${CMAKE_BINARY_DIR}/nan-render)

find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(include)
include_directories(../deps/glad/include)
include_directories(../deps/stb/)


add_executable(${TARGET}
  ../deps/glad/src/glad.c
  ../deps/stb/stb_image.cpp
  src/main.cpp
  src/nan/application.cpp
  src/nan/camera.cpp
  src/nan/game.cpp
  src/nan/gameobject.cpp
  src/nan/shader.cpp
  src/nan/texture.cpp
  src/nan/sprite.cpp
  src/nan/object.cpp
  src/nan/editor/editor.cpp
  src/nan/editor/maineditor.cpp
)

target_link_libraries(${TARGET} ${SDL2_LIBRARIES} imgui spdlog::spdlog glm::glm) 

file(MAKE_DIRECTORY ${NAN_BINARY_DIR}/resources)

file(GLOB SHADERS shaders/basic.frag shaders/basic.vert)
file(GLOB_RECURSE RESOURCES resources/*)

add_custom_target(nan_copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${NAN_SOURCE_DIR}/resources ${NAN_BINARY_DIR}/resources/
  DEPENDS ${RESOURCES}
)

add_dependencies(${TARGET} nan_copy_resources)
