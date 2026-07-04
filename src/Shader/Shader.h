#pragma once

#include <glad/glad.h>

class Shader
{
public:
    Shader(GLuint shaderID);
    virtual void Use();

    void UseShader();
    void ClearShader();

    ~Shader();

protected:
    GLuint programID;

};