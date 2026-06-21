#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int m_id;
    unsigned int compileShader(unsigned int type, const std::string& src);
};