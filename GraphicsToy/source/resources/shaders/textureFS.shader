#version 440 core
out vec4 fragColor;

in vec3 vs_out_color;
in vec2 vs_out_texCoord;

uniform sampler texture1;

void main()
{
	fragColor = texture(texture1, vs_out_texCoord);
}