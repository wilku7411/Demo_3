#core 440

layout(location = 0) a_pos;
layout(location = 1) a_normal;

out vec3 out_normal;
out vec3 out_fragPos;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float u_time;
uniform vec3 u_cameraPos;

void main()
{
    float speed = 8.0;
    vec3 displacedPos = in_position;
    displacedPos.z = (1.0 / 2.0) *sin(u_time * (in_position.x + speed));
    displacedPos.z -= cos(u_time * (in_position.y + speed));
    vec3 normal = vec3(0.0, 0.0, 1.0);

    vec3 out_normal = mat3(transpose(inverse(u_model))) * normal;
    vec3 out_fragPos = vec3(u_model * vec4(displacedPos, 1.0));

    vec3 color = vec3(0.3, 0.3, 0.8);
    vec3 lightPos_worldspace = vec3(5.0, 5.0, 5.0);
    vec3 lightColor = vec3(0.5, 0.5, 0.5);

    vec3 norm = normalize(out_normal);
    vec3 lightDir = normalize(lightPos_worldspace - out_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    //vec3 viewPos = vec3(25.0, 12.0, 2.0);
    vec3 viewPos = u_cameraPos;
    vec3 viewDir = normalize(viewPos - out_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = 1.0 * spec * lightColor;

    passed_result = (diffuse + specular) * color;
    //passed_result = diffuse * color;

    gl_Position = u_projection * u_view * u_model * vec4(displacedPos, 1.0);
}