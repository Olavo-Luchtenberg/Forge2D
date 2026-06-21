#include "Window.h"
#include "Input.h"
#include "editor/Editor.h"
#include <glad/glad.h>
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

Window::~Window() {
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erro SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width, m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        return false;
    }

    m_context = SDL_GL_CreateContext(m_window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, m_width, m_height);
    return true;
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

void Window::pollEvents(bool& running) {
    Input::update();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Editor::processEvent(&event);
        Input::processEvent(event);
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
    }
}