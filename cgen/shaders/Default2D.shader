[VERTEX]
#version 330 core
layout (location = " + std::to_string(GL::Vertices) + ") in vec3 vertex;
layout (location = " + std::to_string(GL::Colors)   + ") in vec4 in_color;

out vec4 vert_color;

uniform mat4 proj_matrix;

void main() {
    vert_color   = in_color;
    gl_Position  = vec4(vertex.xy, 0.0, 1.0) * proj_matrix;
}

[FRAGMENT]
#version 330 core

in  vec4 vert_color;
out vec4 color;

void main() {
    color = vert_color;
}