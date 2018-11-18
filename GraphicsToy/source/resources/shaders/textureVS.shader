#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 0) in vec3 aColor;
layout(location = 0) in vec2 aTexCoord;

out vec3 vs_out_color;
out vec2 vs_out_texCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	vs_out_color = aColor;
	vs_out_texCoord = vec2(aTexCoord.x, aTexCoord.y);
}