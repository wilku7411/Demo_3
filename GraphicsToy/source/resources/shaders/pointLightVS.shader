#version 400

layout (location = 0) in vec3 position;

uniform MVP
{
	mat4 u_projection;
	mat4 u_view;
	mat4 u_model;
};

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}