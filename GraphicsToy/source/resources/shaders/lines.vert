#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 outColor;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	outColor = color;
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}