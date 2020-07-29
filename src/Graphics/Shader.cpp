#include <SGAL/SGAL.h>

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace sgal
{
    Shader::Shader()
    {
        id = 0;
        std::memset(ids, 0, sizeof(unsigned int) * ShaderType::Count);
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
	
	const std::string& Shader::getSource(ShaderType type) const
	{
		return source[type];
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

        case ShaderType::Compute:
            ids[2] = glCreateShader(GL_COMPUTE_SHADER);
            break;

		// Case if GL_GLSL run function and seperate string contents
        };

		source[type] = contents;

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

            case ShaderType::Compute:
                std::cout << "COMPUTE";
                break;
            }

            std::cout << " shader!\n" << infoLog << "\n";
        }

        glAttachShader(id, ids[type]);
    }

    void Shader::link() const
    {
        SG_ASSERT(id, "You must load shaders before you can link!");
        
        for (int i = 0; i < ShaderType::Compute && ids[ShaderType::Compute]; i++)
            SG_ASSERT(!ids[i], "There must be no other shaders with a compute shader!");

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
	static Shader* default2d = nullptr;
	static Shader* default3d = nullptr;
	static Shader* defaultui = nullptr;

	Shader& Shader::Default2D()
	{
		if (!default2d)		{

			std::string vertex_contents;
			vertex_contents += "#version 330 core\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;\n";
			vertex_contents += "\n";
			vertex_contents += "out vec4 vert_color;\n";
			vertex_contents += "\n";
			vertex_contents += "uniform mat4 proj_matrix;\n";
			vertex_contents += "\n";
			vertex_contents += "void main() {\n";
			vertex_contents += "    vert_color   = in_color;\n";
			vertex_contents += "    gl_Position  = vec4(vertex.xy, 0.0, 1.0) * proj_matrix;\n";
			vertex_contents += "}\n";
			vertex_contents += "\n";

			std::string fragment_contents;
			fragment_contents += "#version 330 core\n";
			fragment_contents += "\n";
			fragment_contents += "in  vec4 vert_color;\n";
			fragment_contents += "out vec4 color;\n";
			fragment_contents += "\n";
			fragment_contents += "void main() {\n";
			fragment_contents += "    color = vert_color;\n";
			fragment_contents += "}\n";

			default2d = new Shader();
			default2d->fromString(vertex_contents,     Shader::Vertex);
			default2d->fromString(fragment_contents,   Shader::Fragment);
			default2d->link();

		}
		return *default2d;
	}

	Shader& Shader::Default3D()
	{
		if (!default3d)		{

			std::string vertex_contents;
			vertex_contents += "#version 330 core\n";
			vertex_contents += "\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Normals)  + ") in vec3 in_normal;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::TexCoords)     + ") in vec2 tex;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::ModelMatrices) + ") in mat4 modelMatrix;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::ModelMatrices + 4) + ") in mat4 normalMatrix;\n";
			vertex_contents += "\n";
			vertex_contents += "uniform mat4 vp_matrix;\n";
			vertex_contents += "\n";
			vertex_contents += "out vec2 tex_coords;\n";
			vertex_contents += "out vec4 vert_color;\n";
			vertex_contents += "out vec4 position;\n";
			vertex_contents += "out vec4 normal;\n";
			vertex_contents += "\n";
			vertex_contents += "void main() {\n";
			vertex_contents += "    position     = vec4(vertex, 1.0) * modelMatrix;\n";
			vertex_contents += "    normal       = vec4(in_normal, 1.0) * normalMatrix;\n";
			vertex_contents += "    vert_color   = in_color;\n";
			vertex_contents += "    tex_coords   = tex;\n";
			vertex_contents += "    gl_Position  = position * vp_matrix;\n";
			vertex_contents += "}\n";
			vertex_contents += "\n";

			std::string fragment_contents;
			fragment_contents += "#version 330 core\n";
			fragment_contents += "\n";
			fragment_contents += "struct Light {\n";
			fragment_contents += "    float intensity;\n";
			fragment_contents += "    vec3  position;\n";
			fragment_contents += "    vec4  color;\n";
			fragment_contents += "    int   type;\n";
			fragment_contents += "};\n";
			fragment_contents += "\n";
			fragment_contents += "uniform Light lights[" + std::to_string(SG_MAX_LIGHTS) + "];\n";
			fragment_contents += "uniform int   light_count;\n";
			fragment_contents += "uniform bool  use_lighting;\n";
			fragment_contents += "uniform bool  use_textures;\n";
			fragment_contents += "uniform sampler2D texture1;\n";
			fragment_contents += "\n";
			fragment_contents += "in  vec2 tex_coords;\n";
			fragment_contents += "in  vec4 vert_color;\n";
			fragment_contents += "in  vec4 position;\n";
			fragment_contents += "in  vec4 normal;\n";
			fragment_contents += "out vec4 color;\n";
			fragment_contents += "\n";
			fragment_contents += "vec4 getOutputColor() {\n";
			fragment_contents += "    // Initialize color to black\n";
			fragment_contents += "    vec4 output_color = vec4(0.0);\n";
			fragment_contents += "\n";
			fragment_contents += "    for (int i = 0; i < light_count; i++)\n";
			fragment_contents += "    {\n";
			fragment_contents += "        // Get the light position\n";
			fragment_contents += "        vec4 light_pos = vec4(lights[i].position, 1.0);\n";
			fragment_contents += "        vec4 add_color = vec4(0.0);\n";
			fragment_contents += "        \n";
			fragment_contents += "        // If the type is not directional, move its position relative to the camera\n";
			fragment_contents += "        if (lights[i].type != " + std::to_string(Light::Directional) + ") \n";
			fragment_contents += "        {\n";
			fragment_contents += "            light_pos = light_pos - position;\n";
			fragment_contents += "        }\n";
			fragment_contents += "\n";
			fragment_contents += "        // Add the normal/light directional dot-product multiplied by the light color to the color\n";
			fragment_contents += "        add_color += vec4(lights[i].color.xyz, 1.0) * max(dot(normalize(light_pos), normalize(normal)), 0.0);\n";
			fragment_contents += "\n";
			fragment_contents += "        // If the type is a point-light, add the fall-off\n";
			fragment_contents += "        if (lights[i].type == " + std::to_string(Light::Point) + ") \n";
			fragment_contents += "        {\n";
			fragment_contents += "            add_color = add_color / max(length(light_pos) / lights[i].intensity, 1.0);\n";
			fragment_contents += "        }\n";
			fragment_contents += "\n";
			fragment_contents += "        // Add the lights color to the output\n";
			fragment_contents += "        output_color += add_color;\n";
			fragment_contents += "    }\n";
			fragment_contents += "\n";
			fragment_contents += "    // Return the final color\n";
			fragment_contents += "    return vert_color * vec4(output_color.xyz, 1.0);\n";
			fragment_contents += "}\n";
			fragment_contents += "\n";
			fragment_contents += "void main() {\n";
			fragment_contents += "    // If use_lighting, call the function, otherwise ignore lighting\n";
			fragment_contents += "    vec4 output_color = (use_lighting)?(getOutputColor()):(vert_color);\n";
			fragment_contents += "\n";
			fragment_contents += "    // If we're using textures, multiply the color by the texture color\n";
			fragment_contents += "    if (use_textures) \n";
			fragment_contents += "    {\n";
			fragment_contents += "        output_color = output_color * texture(texture1, tex_coords);\n";
			fragment_contents += "    }\n";
			fragment_contents += "\n";
			fragment_contents += "    // set the final color\n";
			fragment_contents += "    color = output_color;\n";
			fragment_contents += "    //color = vec4((normal.xyz + 1) / 2, 1.0);\n";
			fragment_contents += "}\n";

			default3d = new Shader();
			default3d->fromString(vertex_contents,     Shader::Vertex);
			default3d->fromString(fragment_contents,   Shader::Fragment);
			default3d->link();

		}
		return *default3d;
	}

	Shader& Shader::DefaultUI()
	{
		if (!defaultui)		{

			std::string vertex_contents;
			vertex_contents += "#version 330 core\n";
			vertex_contents += "\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Vertices)  + ") in vec3 vertex;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::Colors)    + ") in vec4 in_color;\n";
			vertex_contents += "layout (location = " + std::to_string(GL::TexCoords) + ") in vec2 tex;\n";
			vertex_contents += "\n";
			vertex_contents += "uniform vec2 screen_size;\n";
			vertex_contents += "uniform vec2 size; // size of the object\n";
			vertex_contents += "uniform mat4 transform_mat;\n";
			vertex_contents += "\n";
			vertex_contents += "out vec4 vert_color;\n";
			vertex_contents += "out vec2 coords;\n";
			vertex_contents += "out vec2 tex_coords;\n";
			vertex_contents += "\n";
			vertex_contents += "void main() {\n";
			vertex_contents += "    vert_color    = in_color;\n";
			vertex_contents += "    vec4 position = vec4(vertex, 1.0);\n";
			vertex_contents += "\n";
			vertex_contents += "    position.x = position.x * (size.x);\n";
			vertex_contents += "    position.y = position.y * (-size.y);\n";
			vertex_contents += "\n";
			vertex_contents += "    coords   = position.xy;\n";
			vertex_contents += "    position = position * transform_mat;\n";
			vertex_contents += "\n";
			vertex_contents += "    tex_coords = tex;\n";
			vertex_contents += "    position.x = (position.x / screen_size.x *  2.0) - 1.0;\n";
			vertex_contents += "    position.y = (position.y / screen_size.y * -2.0) + 1.0;\n";
			vertex_contents += "\n";
			vertex_contents += "    gl_Position = vec4(position.xy, 0.0, 1.0);\n";
			vertex_contents += "}\n";
			vertex_contents += "\n";

			std::string fragment_contents;
			fragment_contents += "#version 330 core\n";
			fragment_contents += "\n";
			fragment_contents += "uniform sampler2D texture1;\n";
			fragment_contents += "uniform bool      use_textures;\n";
			fragment_contents += "\n";
			fragment_contents += "in  vec2 tex_coords;\n";
			fragment_contents += "in  vec2 coords;\n";
			fragment_contents += "in  vec4 vert_color;\n";
			fragment_contents += "out vec4 FragColor;\n";
			fragment_contents += "\n";
			fragment_contents += "uniform vec2 size;\n";
			fragment_contents += "uniform float radius;\n";
			fragment_contents += "\n";
			fragment_contents += "void main() {\n";
			fragment_contents += "    // Top left corner\n";
			fragment_contents += "    if (radius > 0 && coords.x < radius && coords.y < radius && length(coords - vec2(radius)) > radius) {\n";
			fragment_contents += "        discard;\n";
			fragment_contents += "    }\n";
			fragment_contents += "\n";
			fragment_contents += "    // Top right corner\n";
			fragment_contents += "    if (radius > 0 && coords.x > size.x - radius && coords.y < radius && length(coords - vec2(size.x - radius, radius)) > radius) {\n";
			fragment_contents += "        discard;\n";
			fragment_contents += "    }\n";
			fragment_contents += "\n";
			fragment_contents += "    // Bottom right corner\n";
			fragment_contents += "    if (radius > 0 && coords.x > size.x - radius && coords.y > size.y - radius && length(coords - vec2(size.x - radius, size.y - radius)) > radius) {\n";
			fragment_contents += "        discard;\n";
			fragment_contents += "    }\n";
			fragment_contents += "\n";
			fragment_contents += "    // Bottom left corner\n";
			fragment_contents += "    if (radius > 0 && coords.x < radius && coords.y > size.y - radius && length(coords - vec2(radius, size.y - radius)) > radius) {\n";
			fragment_contents += "        discard;\n";
			fragment_contents += "    }\n";
			fragment_contents += "    \n";
			fragment_contents += "    if (use_textures) FragColor = texture(texture1, tex_coords) * vert_color;\n";
			fragment_contents += "    else              FragColor = vert_color;\n";
			fragment_contents += "}\n";

			defaultui = new Shader();
			defaultui->fromString(vertex_contents,     Shader::Vertex);
			defaultui->fromString(fragment_contents,   Shader::Fragment);
			defaultui->link();

		}
		return *defaultui;
	}

}
