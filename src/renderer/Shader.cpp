#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    unsigned int vert = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int frag = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    m_id = glCreateProgram();
    glAttachShader(m_id, vert);
    glAttachShader(m_id, frag);
    glLinkProgram(m_id);

    int success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(m_id, 512, nullptr, log);
        std::cerr << "Erro ao linkar shader: " << log << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::bind() const { glUseProgram(m_id); }
void Shader::unbind() const { glUseProgram(0); }

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::compileShader(unsigned int type, const std::string& src) {
    unsigned int id = glCreateShader(type);
    const char* source = src.c_str();
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "Erro ao compilar shader: " << log << std::endl;
    }

    return id;
}