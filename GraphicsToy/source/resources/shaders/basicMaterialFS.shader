#version 440 core

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
    float strength;
};

struct Camera
{
	vec3 position;
};

uniform Material u_material;
uniform Light    u_light;
uniform Camera   u_camera;
//uniform float    u_time;
uniform int      u_blinn;

void main()
{
	// Ambient lighting
    vec3 lightAmb = u_light.ambient * u_light.strength;
	vec3 ambient = lightAmb * u_material.ambient;

	// Diffuse lighting
	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(u_light.position - fs_in.fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 lightDiff = u_light.diffuse * u_light.strength;
	vec3 diffuse = lightDiff * (diff * u_material.diffuse);

	// Specular 
	vec3 viewDir = normalize(u_camera.position - fs_in.fragPos);

    float spec = 0.0;
    if (u_blinn == 1)
    {
        vec3 halfway = normalize(lightDir + viewDir);
        spec = pow(max(dot(fs_in.normal, halfway), 0.0), 2 * u_material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    }

    vec3 lightSpec = u_light.specular * u_light.strength;
    vec3 specular = lightSpec * (spec* u_material.specular);

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}