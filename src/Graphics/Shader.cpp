#include <SGL/SGL.h>

#include <GL/glew.h>
#include <fstream>
#include <cassert>
#include <iostream>

#define SHADE_ID ids[(int)type]

namespace SGL
{
    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        if (id)
            glDeleteProgram(id);
    }

    void Shader::setUniform(const std::string& name, const Texture& value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value.id);
    }

    void Shader::setUniform(const std::string& name, Mat4f value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value(0, 0));
    }

    void Shader::setUniform(const std::string& name, Vec3f value) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& name, Vec2f value) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }
    
    void Shader::setUniform(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    
    void Shader::setUniform(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::loadFile(const std::string& location, ShaderType type)
    {
        if (!id) id = glCreateProgram();

        std::ifstream file(location);
        assert(file);

        std::string contents;
        for (std::string line; std::getline(file, line);)
            contents += line + "\n";

        file.close();

        switch(type)
        {
        case ShaderType::Vertex:
            ids[0] = glCreateShader(GL_VERTEX_SHADER);
            break;
        
        case ShaderType::Fragment:
            ids[1] = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }

        const char* c_contents = contents.c_str();
        int length = contents.length();
        glShaderSource(SHADE_ID, 1, &c_contents, &length);
        glCompileShader(SHADE_ID);

        int success; 
        glGetShaderiv(SHADE_ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(SHADE_ID, 512, nullptr, infoLog);

            std::cout << "Error compiling ";
            switch (type)
            {
            case ShaderType::Vertex:
                std::cout << "VERTEX";
                break;
                
            case ShaderType::Fragment:
                std::cout << "FRAGMENT";
                break;
            }

            std::cout << " shader!\n" << infoLog << "\n";
        }

        glAttachShader(id, SHADE_ID);
    }

    void Shader::link() const
    {        
        assert(id);
        
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error linking program!\n" << infoLog << "\n";
        }

        for (int i = 0; i < (int)ShaderType::Count; i++)
            glDeleteShader(ids[i]);
    }

    void Shader::bind() const
    {
        glUseProgram(id);
    }

    void Shader::setDefault()
    {
        glUseProgram(0);
    }
}