[VERTEX]
#version 330 core

layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;
layout (location = " + std::to_string(GL::Normals)  + ") in vec3 in_normal;
layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;
layout (location = " + std::to_string(GL::TexCoords)     + ") in vec2 tex;
layout (location = " + std::to_string(GL::ModelMatrices) + ") in mat4 modelMatrix;
layout (location = " + std::to_string(GL::ModelMatrices + 4) + ") in mat4 normalMatrix;

uniform mat4 vp_matrix;

out vec2 tex_coords;
out vec4 vert_color;
out vec4 position;
out vec4 normal;

void main() {
    position     = vec4(vertex, 1.0) * modelMatrix;
    normal       = vec4(in_normal, 1.0) * normalMatrix;
    vert_color   = in_color;
    tex_coords   = tex;
    gl_Position  = position * vp_matrix;
}

[FRAGMENT]
#version 330 core

struct Light {
    float intensity;
    vec3  position;
    vec4  color;
    int   type;
};

uniform Light lights[" + std::to_string(SG_MAX_LIGHTS) + "];
uniform int   light_count;
uniform bool  use_lighting;
uniform bool  use_textures;
uniform sampler2D texture1;

in  vec2 tex_coords;
in  vec4 vert_color;
in  vec4 position;
in  vec4 normal;
out vec4 color;

vec4 getOutputColor() {
    // Initialize color to black
    vec4 output_color = vec4(0.0);

    for (int i = 0; i < light_count; i++)
    {
        // Get the light position
        vec4 light_pos = vec4(lights[i].position, 1.0);
        vec4 add_color = vec4(0.0);
        
        // If the type is not directional, move its position relative to the camera
        if (lights[i].type != " + std::to_string(Light::Directional) + ") 
        {
            light_pos = light_pos - position;
        }

        // Add the normal/light directional dot-product multiplied by the light color to the color
        add_color += vec4(lights[i].color.xyz, 1.0) * ((max(dot(normalize(light_pos), normalize(normal)), 0.5) - 0.5) * 2.0);

        // If the type is a point-light, add the fall-off
        if (lights[i].type == " + std::to_string(Light::Point) + ") 
        {
            add_color = add_color / max(length(light_pos) / lights[i].intensity, 1.0);
        }

        // Add the lights color to the output
        output_color += add_color;
    }

    // Return the final color
    return vert_color * vec4(output_color.xyz, 1.0);
}

void main() {
    // If use_lighting, call the function, otherwise ignore lighting
    vec4 output_color = (use_lighting)?(getOutputColor()):(vert_color);

    // If we're using textures, multiply the color by the texture color
    if (use_textures) 
    {
        output_color = output_color * texture(texture1, tex_coords);
    }

    // set the final color
    color = output_color;
    //color = vec4((normal.xyz + 1) / 2, 1.0);
}