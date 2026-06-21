# Forge2D

Forge2D is a lightweight 2D game engine built in C++ using OpenGL, SDL2, and Lua scripting.

## Features

- OpenGL 3.3 rendering with sprite support
- 2D orthographic camera
- Entity Component System (ECS)
- Scene management
- AABB collision detection
- Audio system (WAV/OGG support)
- Asset manager with caching
- Built-in editor with ImGui
- Lua scripting support
- Windows distribution ready

## Requirements

- Windows 10/11
- MinGW-w64 GCC 13+
- CMake 3.16+
- vcpkg

## Building

```bash
git clone https://github.com/yourusername/Forge2D
cd Forge2D
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
cmake --build .
```

## Dependencies

- SDL2
- SDL2_mixer
- OpenGL / GLAD
- GLM
- stb_image
- ImGui
- Lua 5.5

## Quick Start

### Creating an Entity

```cpp
auto& player = createEntity("player");
player.transform.position = { 400.0f, 300.0f, 0.0f };
player.transform.size     = { 100.0f, 100.0f };
player.sprite.texturePath = "player_tex";
player.hasCollider        = true;
player.collider.size      = { 100.0f, 100.0f };
```

### Loading Assets

```cpp
AssetManager::get().loadTexture("player_tex", "../assets/player.png");
AssetManager::get().loadSound("jump", "../assets/jump.wav");
```

### Lua Scripting

Attach a Lua script to any entity:

```cpp
ScriptEngine::init();
ScriptEngine::bindEntity(findEntity("player"));
ScriptEngine::loadScript("../assets/player.lua");
```

Example Lua script:

```lua
function update(dt)
    x = getPositionX()
    setPositionX(x + 100 * dt)
end
```

### Available Lua API

| Function | Description |
|---|---|
| `getPositionX()` | Returns entity X position |
| `getPositionY()` | Returns entity Y position |
| `setPositionX(x)` | Sets entity X position |
| `setPositionY(y)` | Sets entity Y position |
| `getSizeX()` | Returns entity width |
| `getSizeY()` | Returns entity height |

### Scene Management

```cpp
SceneManager::get().push(std::make_shared<GameScene>());
SceneManager::get().replace(std::make_shared<MenuScene>());
SceneManager::get().pop();
```

### Audio

```cpp
Audio::playSound("jump");
Audio::playMusic("background");
Audio::setMusicVolume(64);
```

## License

MIT License