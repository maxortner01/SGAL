#include <SGAL/SGAL.h>

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace sgal
{
    static Shader* default3D = nullptr;


    Shader::Shader()
    {
        id = 0;
    }

    Shader::~Shader()
    {
        if (id) glDeleteProgram(id);
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

    Shader& Shader::Default3D()
    {
        if (!default3D)
        {
            std::string vertex_contents = "#version 330 core\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::ModelMatrices) + ") in mat4 modelMatrix;\n";
            vertex_contents += "uniform mat4 view_matrix;\n";
            vertex_contents += "uniform mat4 proj_matrix;\n";
            vertex_contents += "void main() {\n";
            vertex_contents += "    gl_Position = vec4(vertex, 1.0) * modelMatrix * view_matrix * proj_matrix;\n";
            vertex_contents += "}\n";
            
            std::string fragment_contents = "#version 330 core\n";
            fragment_contents += "out vec4 color;\n";
            fragment_contents += "void main() {\n";
            fragment_contents += "    color = vec4(1, 1, 1, 1);\n";
            fragment_contents += "}\n";

            default3D = new Shader();
            default3D->fromString(vertex_contents,   ShaderType::Vertex);
            default3D->fromString(fragment_contents, ShaderType::Fragment);
            default3D->link();
        }

        return *default3D;
    }

}