#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int useSolidColor;
uniform vec4 solidColor;

void main()
{
    if (useSolidColor == 1)
    {
        FragColor = solidColor;
    }
    else
    {
        FragColor = mix(
            texture(texture1, TexCoord),
            texture(texture2, TexCoord),
            0.5
        );
    }
}