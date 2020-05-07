[VERTEX]
#version 330 core

layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;
layout (location = " + std::to_string(GL::Normals)  + ") in vec3 in_normal;
layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;
layout (location = " + std::to_string(GL::TexCoords)     + ") in vec2 tex;
layout (location = " + std::to_string(GL::ModelMatrices) + ") in mat4 modelMatrix;
layout (location = " + std::to_string(GL::ModelMatrices + 4) + ") in mat4 normalMatrix;

uniform bool turn_to_camera;
uniform mat4 vp_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

out vec2 tex_coords;
out vec4 vert_color;
out vec4 position;
out vec4 normal;
out mat4 model_matrix;

void main() {
    position     = vec4(vertex, 1.0) * modelMatrix;
    normal       = vec4(in_normal, 1.0) * normalMatrix;
    model_matrix = modelMatrix;
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

uniform mat4 view_matrix;

uniform Light lights[" + std::to_string(SG_MAX_LIGHTS) + "];
uniform int   light_count;
uniform bool  use_lighting;
uniform bool  use_textures;
uniform sampler2D texture1;

out vec4 color;
in  vec2 tex_coords;
in  vec4 position;
in  vec4 normal;
in  vec4 vert_color;
in  mat4 model_matrix;

vec4 getOutputColor() {
    vec4 output_color = vec4(0.0);

    for (int i = 0; i < light_count; i++)
    {
        vec4 light_pos = vec4(lights[i].position, 1);
        if (lights[i].type == 1) { light_pos = light_pos - position; } // If point light

        output_color += vec4(lights[i].color.xyz, 1.0) * max(dot(normalize(light_pos), normalize(normal)), 0.0);
        if (lights[i].type == 1) { output_color = output_color / max(length(light_pos) / lights[i].intensity, 1.0); }
    }

    return vert_color * vec4(output_color.xyz, 1.0);
}

void main() {
    vec4 output_color = (use_lighting)?(getOutputColor()):(vert_color);

    if (use_textures) { output_color = output_color * texture(texture1, tex_coords); };
    color = output_color;
    //color = vec4((normal.xyz + 1) / 2, 1.0);
}