#version 330 core

out vec4 FragColor;

// Material: apenas a parte especular fica aqui.
// A cor ambiente e difusa de cada objeto vem de solidColor.
struct Material {
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;

uniform int useSolidColor;
uniform vec4 solidColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // cor do objeto (usada como material ambiente e difuso)
    vec3 objectColor = solidColor.rgb;

    // ambient
    vec3 ambient = light.ambient * objectColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * objectColor);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, solidColor.a);

    //if (useSolidColor == 1)
    //{
    //    FragColor = solidColor;
    //}
    //else
    //{
      //  FragColor = texture(texture1, TexCoord);
    //}
}
