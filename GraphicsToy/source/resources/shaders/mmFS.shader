#version 400

in vec2 textureCoords;

out vec4 color;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

void main()
{
	color = mix(texture(u_texture0, textureCoords), texture(u_texture1, textureCoords), 0.5);
}