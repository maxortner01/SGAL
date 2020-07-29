[VERTEX]
#version 330 core

layout (location = " + std::to_string(GL::Vertices)  + ") in vec3 vertex;
layout (location = " + std::to_string(GL::Colors)    + ") in vec4 in_color;
layout (location = " + std::to_string(GL::TexCoords) + ") in vec2 tex;

uniform vec2 screen_size;
uniform vec2 size; // size of the object
uniform mat4 transform_mat;

out vec4 vert_color;
out vec2 coords;
out vec2 tex_coords;

void main() {
    vert_color    = in_color;
    vec4 position = vec4(vertex, 1.0);

    position.x = position.x * (size.x);
    position.y = position.y * (-size.y);

    coords   = position.xy;
    position = position * transform_mat;

    tex_coords = tex;
    position.x = (position.x / screen_size.x *  2.0) - 1.0;
    position.y = (position.y / screen_size.y * -2.0) + 1.0;

    gl_Position = vec4(position.xy, 0.0, 1.0);
}

[FRAGMENT]
#version 330 core

uniform sampler2D texture1;
uniform bool      use_textures;

in  vec2 tex_coords;
in  vec2 coords;
in  vec4 vert_color;
out vec4 FragColor;

uniform vec2 size;
uniform float radius;

void main() {
    // Top left corner
    if (radius > 0 && coords.x < radius && coords.y < radius && length(coords - vec2(radius)) > radius) {
        discard;
    }

    // Top right corner
    if (radius > 0 && coords.x > size.x - radius && coords.y < radius && length(coords - vec2(size.x - radius, radius)) > radius) {
        discard;
    }

    // Bottom right corner
    if (radius > 0 && coords.x > size.x - radius && coords.y > size.y - radius && length(coords - vec2(size.x - radius, size.y - radius)) > radius) {
        discard;
    }

    // Bottom left corner
    if (radius > 0 && coords.x < radius && coords.y > size.y - radius && length(coords - vec2(radius, size.y - radius)) > radius) {
        discard;
    }
    
    if (use_textures) FragColor = texture(texture1, tex_coords) * vert_color;
    else              FragColor = vert_color;
}