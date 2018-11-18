#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

out vec3 fragPos;
out vec3 outNormal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    fragPos = vec3(u_model * vec4(position, 1.0));
    outNormal = mat3(transpose(inverse(u_model))) * normal;  
   
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}