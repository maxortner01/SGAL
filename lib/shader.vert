#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 modelMatrix;

out vec3 color;

uniform float aspectRatio;
uniform vec2 min_max;
uniform vec3 camera_vec;

// Model info
uniform float model_scale;
uniform vec3 model_rot;
uniform vec3 model_trans;

uniform bool use_lighting;

uniform vec3 base_camera_pos;

mat4 makeRotation(vec3 angles)
{
    mat4 rotation_x, rotation_y, rotation_z;
    rotation_x[0] = vec4(cos(angles.x), 0, sin(angles.x), 0);
    rotation_x[1] = vec4(0, 1, 0, 0);
    rotation_x[2] = vec4(-sin(angles.x), 0, cos(angles.x), 0);
    rotation_x[3] = vec4(0, 0, 0, 1);
    
    rotation_y[0] = vec4(1, 0, 0, 0);
    rotation_y[1] = vec4(0, cos(angles.y), -sin(angles.y), 0);
    rotation_y[2] = vec4(0, sin(angles.y),  cos(angles.y), 0);
    rotation_y[3] = vec4(0, 0, 0, 1);

    rotation_z[0] = vec4(cos(angles.z), -sin(angles.z), 0, 0);
    rotation_z[1] = vec4(sin(angles.z),  cos(angles.z), 0, 0);
    rotation_z[2] = vec4(0, 0, 1, 0);
    rotation_z[3] = vec4(0, 0, 0, 1);

    return rotation_x * rotation_y * rotation_z;
}

void main()
{
    vec4 camera_pos = vec4(0, 0, -1 + camera_vec.z, 1.0);
    mat4 camera_rot_mat = makeRotation(vec3(camera_vec.xy, 0.0));
    mat4 camera_pos_mat;
    camera_pos_mat[0] = vec4(1, 0, 0, -camera_pos.x);
    camera_pos_mat[1] = vec4(0, 1, 0, -camera_pos.y);
    camera_pos_mat[2] = vec4(0, 0, 1, -camera_pos.z);
    camera_pos_mat[3] = vec4(0, 0, 0, 1);
    mat4 camera_mat = camera_rot_mat * camera_pos_mat;

    vec3 position = (vec4(aPos, 1.0) * modelMatrix * camera_mat).xyz;

    const float fov = 3.14159 / 2.0;
    const float f = 1.0 / tan(fov / 2.0);
    mat3 projection;
    projection[0] = vec3(1.0 / (aspectRatio * abs(position.z) * f), 0, 0);
    projection[1] = vec3(0, 1.0 / (f * abs(position.z)), 0);
    projection[2] = vec3(0, 0, 1);
    position *= projection;

    // Calculate scalar for depth, -1 to 1
    float z_val = ((position.z - min_max.x) / (min_max.y - min_max.x) - 0.5) * 2.0;

    gl_Position = vec4(position.xy, z_val, 1.0);

    color = vec3(1, 1, 1) * dot(vec3(1, -0.5, 0), -normalize(vec4(normal, 1.0)).xyz) * 2;;
    //color = normal;
}

void main_old()
{
    // Rotate the point about the origin
    mat4 model_rot_mat  = makeRotation(model_rot);
    mat4 camera_rot_mat = makeRotation(vec3(camera_vec.xy, 0.0));
    vec4 model_vert_pos = (vec4(aPos * model_scale, 1.0) * model_rot_mat) + vec4(model_trans, 0.0);
    vec3 point_pos = (vec4(model_vert_pos.xyz - base_camera_pos, 1.0) * camera_rot_mat).xyz;

    float radius = 0.25;
    float tri_bottom = -radius / tan(camera_vec.y);

    // Set color
    //color = vec3(230.0 / 255.0, 252.0 / 255.0, 227.0 / 255.0);
    color = vec3(1, 1, 1);
    
    if (use_lighting)
        color *= dot(vec3(1, -0.5, 0), -normalize(vec4(normal, 1.0) * model_rot_mat).xyz) * 2;

    // Translate the point relative to the camera
    vec3 camera_pos = vec3(0, 0, -1 + camera_vec.z);
    vec3 position = point_pos - camera_pos;

    const float fov = 3.14159 / 2.0;
    const float f = 1.0 / tan(fov / 2.0);
    mat3 projection;
    projection[0] = vec3(1.0 / (aspectRatio * abs(position.z) * f), 0, 0);
    projection[1] = vec3(0, 1.0 / (f * abs(position.z)), 0);
    projection[2] = vec3(0, 0, 1);
    position *= projection;

    //position.x /= aspectRatio;

    //position.x /= tan(3.14159 / 2.0);
    //position.y /= tan(3.14159 / 2.0);

    // Calculate scalar for depth, -1 to 1
    float z_val = ((position.z - min_max.x) / (min_max.y - min_max.x) - 0.5) * 2.0;

    // Set the point and pass the color
    gl_Position = vec4(position.x, position.y, z_val, 1.0);
}