#version 440 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;
layout(location = 3) in vec3 a_offset;

out VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	vec3 pos = a_position + a_offset;

    vs_out.fragPos = vec3(u_model * vec4(pos, 1.0));
    vs_out.normal = mat3(transpose(inverse(u_model))) * a_normal;  
	vs_out.texCoords = a_uv;

    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}