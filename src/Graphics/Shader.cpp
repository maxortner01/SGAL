#include <SGAL/SGAL.h>

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace sgal
{
    Shader::Shader()
    {
        id = 0;
    }

    Shader::~Shader()
    {
        if (id) { glDeleteProgram(id); id = 0; }
    }

    void Shader::setUniform(const Light* lights, const size_t count) const
    {
        setUniform("light_count", (int)count);
        for (int i = 0; i < count; i++)
            setUniform(*(lights + i), i);
    }

    void Shader::setUniform(const Light& light, const unsigned int index) const
    {
        const std::string uniform_name = "lights[" + std::to_string(index) + "]";
        setUniform(uniform_name + ".intensity", light.intensity);
        setUniform(uniform_name + ".position",  light.position);
        setUniform(uniform_name + ".color",     light.color);
        setUniform(uniform_name + ".type",      light.type);
    }
    
    void Shader::setUniform(const std::string& name, const Texture& texture) const
    {
        setUniform(name, (int)texture.id);
    }

    void Shader::setUniform(const std::string& name, Mat4f value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value(0, 0));
    }

    void Shader::setUniform(const std::string& name, Color value) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), value.r, value.g, value.b, value.a);
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

    void Shader::setUniform(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::fromFile(const std::string& filename, ShaderType type)
    {
        if (!id) id = glCreateProgram();

        std::ifstream file(filename);
        SG_ASSERT(file, "Failed to read from file!");

        std::string contents;
        for (std::string line; std::getline(file, line);)
            contents += line + "\n";

        file.close();

        fromString(contents, type);
    }

    void Shader::fromString(const std::string& contents, ShaderType type)
    {
        if (!id) id = glCreateProgram();
        
        switch (type)
        {
        case ShaderType::Vertex:
            ids[0] = glCreateShader(GL_VERTEX_SHADER);
            break;
            
        case ShaderType::Fragment:
            ids[1] = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        };

        const char* c_str = contents.c_str();
        int length        = contents.length();

        glShaderSource(ids[type], 1, &c_str, &length);
        glCompileShader(ids[type]);

        int success;
        glGetShaderiv(ids[type], GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(ids[type], 512, nullptr, infoLog);

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

        glAttachShader(id, ids[type]);
    }

    void Shader::link() const
    {
        SG_ASSERT(id, "You must load shaders before you can link!");

        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error linking program!\n" << infoLog << "\n";
        }

        for (int i = 0; i < ShaderType::Count; i++)
            glDeleteShader(ids[i]);
    }

    void Shader::bind() const
    {
        glUseProgram(id);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    void Shader::useDefault()
    {
        glUseProgram(0);
    }

	// [SHADER] generated-code
}
