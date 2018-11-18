#version 400 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;

out vec3 passedColor;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	passedColor = a_color;
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    //gl_Position = vec4(a_pos, 1.0);
}