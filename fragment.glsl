#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;

uniform int useSolidColor;
uniform vec4 solidColor;

uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec4 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.4;
    vec4 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec4 result = (ambient + diffuse) * solidColor;
    FragColor = vec4(result);

    //if (useSolidColor == 1)
    //{
    //    FragColor = solidColor;
    //}
    //else
    //{
      //  FragColor = texture(texture1, TexCoord);
    //}
}
