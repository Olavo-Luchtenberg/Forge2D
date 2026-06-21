cmake_minimum_required(VERSION 3.16)
project(Engine)

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

add_executable(engine
    src/main.cpp
    src/GameScene.cpp
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

target_link_libraries(engine
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    glad::glad
    glm::glm
    imgui::imgui
    ${LUA_LIBRARIES}
    dwmapi
)

target_include_directories(engine
    PRIVATE
    ${Stb_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    src
    src/vendor
)