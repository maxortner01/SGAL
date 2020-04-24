#include <SGAL/SGAL.h>

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace sgal
{
    static Shader* default3D = nullptr;
    static Shader* default2D = nullptr;
    static Shader* defaultUI = nullptr;

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

    Shader& Shader::DefaultUI()
    {
        if (!defaultUI)
        {
            std::string vertex_contents = "#version 330 core\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Vertices)      + ") in vec3 vertex;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Colors)        + ") in vec4 in_color;\n";
            
            vertex_contents += "uniform vec2 screen_size;\n";
            vertex_contents += "uniform vec2 size; // size of the object\n";
            vertex_contents += "uniform mat4 transform_mat;\n";

            vertex_contents += "out vec4 vert_color;\n";
            vertex_contents += "out vec2 coords;\n";

            vertex_contents += "void main() {\n";
            vertex_contents += "    vert_color    = in_color;\n";
            vertex_contents += "    vec4 position = vec4(vertex, 1.0);\n";
            vertex_contents += "    position.x = position.x * (size.x);\n";
            vertex_contents += "    position.y = position.y * (-size.y);\n";
            vertex_contents += "    coords   = position.xy;\n";
            vertex_contents += "    position = position * transform_mat;\n";

            vertex_contents += "    position.x = (position.x / screen_size.x *  2.0) - 1.0;\n";
            vertex_contents += "    position.y = (position.y / screen_size.y * -2.0) + 1.0;\n";
            vertex_contents += "    gl_Position = vec4(position.xy, 0.0, 1.0);\n";
            vertex_contents += "}";
            
            std::string fragment_contents = "#version 330 core\n";
            
            fragment_contents += "in  vec2 coords;\n";
            fragment_contents += "in  vec4 vert_color;\n";
            fragment_contents += "out vec4 FragColor;\n";
            
            fragment_contents += "uniform vec2 size;\n";
            fragment_contents += "uniform float radius;\n";

            fragment_contents += "void main() {\n";

            // Top left corner
            fragment_contents += "    if (radius > 0 && coords.x < radius && coords.y < radius && length(coords - vec2(radius)) > radius) {\n";
            fragment_contents += "        discard;\n";
            fragment_contents += "    }\n";
            
            // Top right corner
            fragment_contents += "    if (radius > 0 && coords.x > size.x - radius && coords.y < radius && length(coords - vec2(size.x - radius, radius)) > radius) {\n";
            fragment_contents += "        discard;\n";
            fragment_contents += "    }\n";
            
            // Bottom right corner
            fragment_contents += "    if (radius > 0 && coords.x > size.x - radius && coords.y > size.y - radius && length(coords - vec2(size.x - radius, size.y - radius)) > radius) {\n";
            fragment_contents += "        discard;\n";
            fragment_contents += "    }\n";
            
            // Bottom left corner
            fragment_contents += "    if (radius > 0 && coords.x < radius && coords.y > size.y - radius && length(coords - vec2(radius, size.y - radius)) > radius) {\n";
            fragment_contents += "        discard;\n";
            fragment_contents += "    }\n";

            fragment_contents += "    FragColor = vert_color;\n";
            fragment_contents += "}\n";

            defaultUI = new Shader();
            defaultUI->fromString(vertex_contents,   Shader::Vertex);
            defaultUI->fromString(fragment_contents, Shader::Fragment);
            defaultUI->link();
        }

        return *defaultUI;
    }

    Shader& Shader::Default3D()
    {
        if (!default3D)
        {
            std::string vertex_contents = "#version 330 core\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Normals)  + ") in vec3 in_normal;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::ModelMatrices) + ") in mat4 modelMatrix;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::ModelMatrices + 4) + ") in mat4 normalMatrix;\n";
            
            vertex_contents += "uniform bool turn_to_camera;\n";
            vertex_contents += "uniform mat4 vp_matrix;\n";
            vertex_contents += "uniform mat4 view_matrix;\n";
            vertex_contents += "uniform mat4 proj_matrix;\n";
            
            vertex_contents += "out vec4 vert_color;\n";
            vertex_contents += "out vec4 position;\n";
            vertex_contents += "out vec4 normal;\n";
            vertex_contents += "out mat4 model_matrix;\n";

            vertex_contents += "void main() {\n";
            vertex_contents += "    position     = vec4(vertex, 1.0) * modelMatrix;\n";
            vertex_contents += "    normal       = vec4(in_normal, 1.0) * normalMatrix;\n";
            vertex_contents += "    model_matrix = modelMatrix;\n";
            vertex_contents += "    vert_color   = in_color;\n";
            vertex_contents += "    gl_Position  = position * vp_matrix;\n";
            vertex_contents += "}\n";
            
            std::string fragment_contents = "#version 330 core\n";

            fragment_contents += "struct Light {\n";
            fragment_contents += "    float intensity;\n";
            fragment_contents += "    vec3  position;\n";
            fragment_contents += "    vec4  color;\n";
            fragment_contents += "    int   type;\n";
            fragment_contents += "};\n";
            
            fragment_contents += "uniform mat4 view_matrix;\n";
            
            fragment_contents += "uniform Light lights[" + std::to_string(SG_MAX_LIGHTS) + "];\n";
            fragment_contents += "uniform int   light_count;\n";
            fragment_contents += "uniform bool  use_lighting;\n";

            fragment_contents += "out vec4 color;\n";
            fragment_contents += "in  vec4 position;\n";
            fragment_contents += "in  vec4 normal;\n";
            fragment_contents += "in  vec4 vert_color;\n";
            fragment_contents += "in  mat4 model_matrix;\n";

            fragment_contents += "vec4 getOutputColor() {\n";
            fragment_contents += "    vec4 output_color = vec4(0.0);\n";

            fragment_contents += "    for (int i = 0; i < light_count; i++)\n";
            fragment_contents += "    {\n";
            fragment_contents += "        vec4 light_pos = vec4(lights[i].position, 1);\n";
            fragment_contents += "        if (lights[i].type == 1) { light_pos = light_pos - position; } // If point light\n";
            
            fragment_contents += "        vec4 n_normal  = (length(normal.xyz) == 0.0)?(normalize(light_pos)):(normalize(normal));\n";

            fragment_contents += "        output_color += vec4(lights[i].color.xyz, 1.0) * max(dot(normalize(light_pos), normalize(n_normal)) * 2.0 - 1.0, 0.0) / max(length(light_pos) / lights[i].intensity, 1.0);\n";
            fragment_contents += "    }\n";

            fragment_contents += "    return vert_color * vec4(output_color.xyz, 1.0);\n";
            fragment_contents += "}\n";

            fragment_contents += "void main() {\n";
            fragment_contents += "    vec4 output_color = (use_lighting)?(getOutputColor()):(vert_color);\n";

            fragment_contents += "    color = output_color;\n";
            fragment_contents += "    //color = vec4((normal.xyz + 1) / 2, 1.0);\n";
            fragment_contents += "}\n";
            
            default3D = new Shader();
            default3D->fromString(vertex_contents,   ShaderType::Vertex);
            default3D->fromString(fragment_contents, ShaderType::Fragment);
            default3D->link();
        }

        return *default3D;
    }

    Shader& Shader::Default2D()
    {
        if (!default2D)
        {
            std::string vertex_contents = "#version 330 core\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;\n";
            vertex_contents += "layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;\n";

            vertex_contents += "out vec4 vert_color;\n";

            vertex_contents += "uniform mat4 proj_matrix;\n";

            vertex_contents += "void main() {\n";
            vertex_contents += "    vert_color   = in_color;\n";
            vertex_contents += "    gl_Position  = vec4(vertex.xy, 0.0, 1.0) * proj_matrix;\n";
            vertex_contents += "}\n";
            
            std::string fragment_contents = "#version 330 core\n";

            fragment_contents += "in  vec4 vert_color;\n";
            fragment_contents += "out vec4 color;\n";
            
            fragment_contents += "void main() {\n";
            fragment_contents += "    color = vert_color;\n";
            fragment_contents += "}\n";

            default2D = new Shader();
            default2D->fromString(vertex_contents,   Shader::Vertex);
            default2D->fromString(fragment_contents, Shader::Fragment);
            default2D->link();
        }

        return *default2D;
    }

}