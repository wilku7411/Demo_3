#version 440

in vec3 out_vertexPos;

in vec3 passed_result;
in float passed_transparency;

out vec4 out_color;

void main()
{
	out_color = vec4(passed_result.rg * 1.0 * passed_transparency, passed_result.b , passed_transparency);
}