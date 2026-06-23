cmake_minimum_required(VERSION 3.16)
project(Forge2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Lua REQUIRED)

set(ENGINE_SOURCES
    src/core/Window.cpp
    src/core/Input.cpp
    src/core/Scene.cpp
    src/core/SceneManager.cpp
    src/core/Physics.cpp
    src/core/Audio.cpp
    src/core/AssetManager.cpp
    src/core/ScriptEngine.cpp
    src/editor/Editor.cpp
    src/renderer/Shader.cpp
    src/renderer/Texture.cpp
    src/renderer/Camera.cpp
    src/vendor/imgui/imgui_impl_sdl2.cpp
    src/vendor/imgui/imgui_impl_opengl3.cpp
)

set(ENGINE_LIBS
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    glad::glad
    glm::glm
    imgui::imgui
    ${LUA_LIBRARIES}
    dwmapi
)

set(ENGINE_INCLUDES
    ${Stb_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    src
    src/vendor
    .
)

add_executable(engine
    src/main.cpp
    src/GameScene.cpp
    ${ENGINE_SOURCES}
)
target_link_libraries(engine PRIVATE ${ENGINE_LIBS})
target_include_directories(engine PRIVATE ${ENGINE_INCLUDES})

add_executable(platformer
    examples/platformer/main.cpp
    examples/platformer/PlatformerScene.cpp
    ${ENGINE_SOURCES}
)
target_link_libraries(platformer PRIVATE ${ENGINE_LIBS})
target_include_directories(platformer PRIVATE ${ENGINE_INCLUDES})