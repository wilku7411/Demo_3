#version 400

in vec3 outNormal;
in vec3 fragPos;

out vec4 fragColor;

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
};

uniform vec3 u_cameraPos;
uniform Material u_material;
uniform Light u_light;

uniform vec3 u_lightPos_worldspace;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;
uniform float u_ambientStrength;
uniform float u_specularStrength;
uniform float u_shininess;

void main()
{    	
	// ambient
	vec3 ambient = u_ambientStrength * u_lightColor;

	// diffuse 
	vec3 norm = normalize(outNormal);
	vec3 lightDir = normalize(u_lightPos_worldspace - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColor;
	
	// specular
	vec3 viewDir = normalize(u_viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
	vec3 specular = u_specularStrength * spec * u_lightColor; 

	// result color
    vec3 result = (ambient + diffuse + specular) * u_objectColor;

	// result fragment color
	fragColor = vec4(result, 1.0);

}